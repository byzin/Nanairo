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
#include <vector>
// Zisc
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "sampled_spectra.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"
#include "NanairoCore/Geometry/point.hpp"

namespace nanairo {

/*!
  */
SampleStatistics::SampleStatistics(System& system) noexcept :
    sample_{&system.dataMemoryManager()},
    sample_squared_{&system.dataMemoryManager()},
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
    auto& sample_p = getSample(position);
    const uint index = sample_p.getIndex(sample.wavelength(i));
    const Float v = sample.intensity(i);
    sample_p.add(index, v);

    // Variance
    if (isEnabled(Type::kVariance)) {
      auto& sample_squared_p = getSampleSquared(position);
      const Float v_squared = zisc::power<2>(v);
      sample_squared_p.add(index, v_squared);
    }
  }
}

/*!
  */
void SampleStatistics::clear() noexcept
{
  ZISC_ASSERT(isEnabled(Type::kExpectedValue), "A sample isn't able to be added.");
  // Sample table
  {
    for (auto& sample_p : sampleTable())
      sample_p->fill(0.0);
  }
  // Sample squared table
  if (isEnabled(Type::kVariance)) {
    for (auto& sample_p : sampleSquaredTable()) {
      sample_p->fill(0.0);
    }
  }
}

/*!
  */
void SampleStatistics::initialize(System& system) noexcept
{
  const std::size_t size = resolution_[0] * resolution_[1];
  auto init_distribution_table =
  [&system, size](zisc::pmr::vector<SpectralDistributionPointer>& table)
  {
    for (std::size_t i = 0; i < size; ++i) {
      table.emplace_back(SpectralDistribution::makeDistribution(
          system.colorMode(),
          &system.dataMemoryManager(),
          true));
    }
  };
  if (isEnabled(Type::kExpectedValue)) {
    sample_.reserve(size);
    init_distribution_table(sample_);
  }
  if (isEnabled(Type::kVariance)) {
    sample_squared_.reserve(size);
    init_distribution_table(sample_squared_);
  }
}

} // namespace nanairo
