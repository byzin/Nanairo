/*!
  \file sample_statistics.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "sample_statistics.hpp"
// Standard C++ library
#include <bitset>
#include <limits>
#include <vector>
// Zisc
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampled_spectra.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Denoiser/bayesian_collaborative_denoiser.hpp"
#include "NanairoCore/Denoiser/denoiser.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"

namespace nanairo {

/*!
  */
SampleStatistics::SampleStatistics(System& system) noexcept :
    sample_{&system.dataMemoryManager()},
    prev_sample_{&system.dataMemoryManager()},
    sample_squared_{&system.dataMemoryManager()},
    histogram_{&system.dataMemoryManager()},
    covariance_factor_{&system.dataMemoryManager()},
    denoised_sample_{&system.dataMemoryManager()},
    resolution_{system.imageResolution()},
    flag_{system.sampleStatisticsFlag()}
{
  initialize(system);
}

/*!
  */
void SampleStatistics::addSample(const Index2d position,
                                 const SampledSpectra& sample) noexcept
{
  ZISC_ASSERT(isEnabled(Type::kExpectedValue), "A sample isn't able to be added.");

  for (uint i = 0; i < sample.size(); ++i) {
    // Expected value
    const uint pixel_index = getIndex(position);
    auto& sample_p = sampleTable()[pixel_index];

    const uint si = sample_p->getIndex(sample.wavelength(i));
    const Float s = sample.intensity(i);
    sample_p->add(si, s);
  }
}

/*!
  */
void SampleStatistics::clear() noexcept
{
  ZISC_ASSERT(isEnabled(Type::kExpectedValue), "A sample isn't able to be added.");

  {
    // Sample
    for (auto& sample_p : sampleTable())
      sample_p->fill(0.0);
  }

  if (isEnabled(Type::kVariance)) {
    // Prev sample
    for (auto& sample_p : prevSampleTable())
      sample_p->fill(0.0);

    // Sample squared
    for (auto& sample_p : sampleSquaredTable())
      sample_p->fill(0.0);
  }

  if (isEnabled(Type::kBayesianCollaborativeValues)) {
    // Histogram
    for (auto& sample_p : histogramTable())
      sample_p->fill(0.0);

    // Covariance matrix factor
    for (auto& factor : covarianceFactorTable())
      factor.set(0.0);
  }

  if (isEnabled(Type::kDenoisedExpectedValue)) {
    // Denoised sample
    for (auto& sample_p : denoisedSampleTable())
      sample_p->fill(0.0);
  }
}

/*!
  */
void SampleStatistics::update(
    System& system,
    const WavelengthSamples& wavelengths,
    const uint32 /* cycle */) noexcept
{
  auto update_info = [this, &system, &wavelengths](const uint task_id)
  {
    // Set the calculation range
    const auto range = system.calcTaskRange(sampleTable().size(), task_id);
    for (auto pixel_index = range[0]; pixel_index < range[1]; ++pixel_index) {
      if (isEnabled(Type::kVariance))
        updateSampleSquared(wavelengths, pixel_index);

      if (isEnabled(Type::kBayesianCollaborativeValues)) {
        updateHistogram(system, wavelengths, pixel_index);
        updateCovarianceFactor(wavelengths, pixel_index);
      }

      if (isEnabled(Type::kVariance))
        updatePrevSample(wavelengths, pixel_index);
    }
  };

  {
    auto& threads = system.threadManager();
    auto& work_resource = system.globalMemoryManager();
    constexpr uint start = 0;
    const uint end = threads.numOfThreads();
    auto result = threads.enqueueLoop(update_info, start, end, &work_resource);
    result.wait();
  }
}

/*!
  */
void SampleStatistics::initialize(System& system) noexcept
{
  const std::size_t size = resolution_[0] * resolution_[1];
  auto init_distribution_table =
  [&system](const std::size_t s,
            const bool is_compensated,
            zisc::pmr::vector<SpectralDistributionPointer>& table)
  {
    for (std::size_t i = 0; i < s; ++i) {
      table.emplace_back(SpectralDistribution::makeDistribution(
          system.colorMode(),
          &system.dataMemoryManager(),
          is_compensated));
    }
  };

  if (isEnabled(Type::kExpectedValue)) {
    sample_.reserve(size);
    init_distribution_table(size, true, sample_);
  }

  if (isEnabled(Type::kVariance)) {
    prev_sample_.reserve(size);
    init_distribution_table(size, false, prev_sample_);

    sample_squared_.reserve(size);
    init_distribution_table(size, true, sample_squared_);
  }

  if (isEnabled(Type::kBayesianCollaborativeValues)) {
    const uint32 bins = (system.colorMode() == RenderingColorMode::kRgb)
        ? zisc::cast<const RgbBcDenoiser*>(&system.denoiser())->histogramBins()
        : zisc::cast<const SpectraBcDenoiser*>(&system.denoiser())->histogramBins();

    const std::size_t s = size * bins;
    histogram_.reserve(s);
    init_distribution_table(s, false, histogram_);
  }

  if (isEnabled(Type::kBayesianCollaborativeValues)) {
    const std::size_t s = size * numOfCovarianceFactors();
    covariance_factor_.resize(s);
  }

  if (isEnabled(Type::kDenoisedExpectedValue)) {
    denoised_sample_.reserve(size);
    init_distribution_table(size, false, denoised_sample_);
  }
}

/*!
  */
void SampleStatistics::updateCovarianceFactor(
    const WavelengthSamples& wavelengths,
    const std::size_t pixel_index) noexcept
{
  const auto& sample_p = sampleTable()[pixel_index];
  const auto& prev_sample_p = prevSampleTable()[pixel_index];
  auto factors = &covarianceFactorTable()[numOfCovarianceFactors() * pixel_index];

  for (uint i = 0; i < wavelengths.size() - 1; ++i) {
    const auto w_a = wavelengths[i];
    const uint si_a = sample_p->getIndex(w_a);
    const Float s_a = sample_p->get(si_a) - prev_sample_p->get(si_a);

    const uint base_index = getFactorIndex(si_a);
    for (uint j = i + 1; j < wavelengths.size(); ++j) {
      const auto w_b = wavelengths[j];
      const uint si_b = sample_p->getIndex(w_b);
      const Float s_b = sample_p->get(si_b) - prev_sample_p->get(si_b);

      const uint offset = base_index + ((si_b - si_a) - 1);
      factors[offset].add(s_a * s_b);
    }
  }
}

/*!
  */
void SampleStatistics::updateHistogram(
    const System& system,
    const WavelengthSamples& wavelengths,
    const std::size_t pixel_index) noexcept
{
  const auto& sample_p = sampleTable()[pixel_index];
  const auto& prev_sample_p = prevSampleTable()[pixel_index];

  const uint32 bins = (system.colorMode() == RenderingColorMode::kRgb)
      ? zisc::cast<const RgbBcDenoiser*>(&system.denoiser())->histogramBins()
      : zisc::cast<const SpectraBcDenoiser*>(&system.denoiser())->histogramBins();

  for (uint i = 0; i < wavelengths.size(); ++i) {
    const auto w = wavelengths[i];
    const uint si = sample_p->getIndex(w);

    constexpr Float e = std::numeric_limits<Float>::epsilon();
    const auto& tone_map = system.toneMappingOperator();
    Float s = sample_p->get(si) - prev_sample_p->get(si);
    s = tone_map.tonemap(s);
    s = zisc::clamp(s, 0.0, 1.0 - e);
    s = zisc::cast<Float>(bins - 1) * s;

    const uint h_low = zisc::cast<uint>(s);
    const Float a = s - zisc::cast<Float>(h_low);
    ZISC_ASSERT(zisc::isInBounds(h_low, 0u, bins - 1), "The low is out of bounds.");
    ZISC_ASSERT(zisc::isInBounds(a, 0.0, 1.0), "The a is out of bounds.");
    // Histogram low
    {
      auto& histogram = histogramTable()[pixel_index * bins + h_low];
      histogram->add(si, 1.0 - a);
    }
    // Histogram high
    {
      auto& histogram = histogramTable()[pixel_index * bins + h_low + 1];
      histogram->add(si, a);
    }
  }
}

/*!
  */
void SampleStatistics::updatePrevSample(
    const WavelengthSamples& wavelengths,
    const std::size_t pixel_index) noexcept
{
  const auto& sample_p = sampleTable()[pixel_index];
  const auto& prev_sample_p = prevSampleTable()[pixel_index];

  for (uint i = 0; i < wavelengths.size(); ++i) {
    const auto w = wavelengths[i];
    const uint si = sample_p->getIndex(w);
    const Float s = sample_p->get(si);
    prev_sample_p->set(si, s);
  }
}

/*!
  */
void SampleStatistics::updateSampleSquared(
    const WavelengthSamples& wavelengths,
    const std::size_t pixel_index) noexcept
{
  const auto& sample_p = sampleTable()[pixel_index];
  const auto& prev_sample_p = prevSampleTable()[pixel_index];
  auto& sample_squared_p = sampleSquaredTable()[pixel_index];

  for (uint i = 0; i < wavelengths.size(); ++i) {
    const auto w = wavelengths[i];
    const uint si = sample_p->getIndex(w);
    const Float s = sample_p->get(si) - prev_sample_p->get(si);
    sample_squared_p->add(si, zisc::power<2>(s));
  }
}

} // namespace nanairo
