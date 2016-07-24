/*!
  \file sampling_wavelengths_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
// GoogleTest
#include "gtest/gtest.h"
// Test
#include "test.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
#include "NanairoCore/Utility/value.hpp"

constexpr nanairo::uint kSampleSize = nanairo::kWavelengthSampleSize;

// Forward declaration
template <nanairo::uint N>
std::array<nanairo::uint, nanairo::kSpectraSize> testWavelengthDistribution(
    const nanairo::WavelengthSampler<kSampleSize>& wavelength_sampler,
    nanairo::Sampler& sampler);

void testUniformWavelengthSampler(
    const nanairo::WavelengthSampler<kSampleSize>& wavelength_sampler);

/*!
  \details
  No detailed.
  */
template <nanairo::uint N>
std::array<nanairo::uint, nanairo::kSpectraSize> testWavelengthDistribution(
    const nanairo::WavelengthSampler<kSampleSize>& wavelength_sampler,
    nanairo::Sampler& sampler)
{
  using namespace nanairo;
  using zisc::cast;

  std::array<uint, kSpectraSize> wavelength_distribution;
  wavelength_distribution.fill(0);

  for (uint i = 0; i < N; ++i) {
    const auto sampled_wavelengths = wavelength_sampler(sampler);
    const auto& wavelengths = sampled_wavelengths.wavelengths();
    for (uint index = 0; index < kSampleSize; ++index) {
      const auto spectra_index = getIndex(wavelengths[index]);
      ++wavelength_distribution[spectra_index];
    }
  }
  return wavelength_distribution;
}

/*!
  \details
  No detailed.
  */
void testUniformWavelengthSampler(
    const nanairo::WavelengthSampler<kSampleSize>& wavelength_sampler)
{
  using namespace nanairo;
  using zisc::cast;
  // Initialize test scene
  SceneSettings settings;
  settings.open("unit_test.nana");
  settings.clear();
  // Initialize test system
  auto system = makeTestSystem(512, 512, false, settings);
  auto& sampler = system->globalSampler();

  // Distribution test
  constexpr uint n = 1000000;
  constexpr uint loop_count = kSpectraSize * n;
  constexpr uint expectation = kSampleSize * n;
  constexpr uint error = expectation / 1000;
  const auto wavelength_distribution = 
      testWavelengthDistribution<loop_count>(wavelength_sampler, sampler);
  for (uint i = 0; i < kSpectraSize; ++i) {
    const auto w_count = wavelength_distribution[i];
    const auto wavelength = getWavelength(i);
    EXPECT_GT(w_count, expectation - error) 
        << "The wavelength (" << wavelength << ") is less than the expectation value.";
    EXPECT_LT(w_count, expectation + error) 
        << "The wavelength (" << wavelength << ") is greater than the expectation value.";
  }
}

TEST(SamplingWavelengthsTest, RegularSamplingTest)
{
  using namespace nanairo;
  using zisc::cast;
  // Initialize sampler
  WavelengthSampler<kSampleSize> wavelength_sampler{
      sampleWavelengthsRegularly<kSampleSize>};
  testUniformWavelengthSampler(wavelength_sampler);
}

TEST(SamplingWavelengthsTest, RandomSamplingTest)
{
  using namespace nanairo;
  using zisc::cast;
  // Initialize sampler
  WavelengthSampler<kSampleSize> wavelength_sampler{
      sampleWavelengthsRandomly<kSampleSize>};
  testUniformWavelengthSampler(wavelength_sampler);
}

TEST(SamplingWavelengthsTest, StratifiedSamplingTest)
{
  using namespace nanairo;
  using zisc::cast;
  // Initialize sampler
  WavelengthSampler<kSampleSize> wavelength_sampler{
      sampleStratifiedWavelengths<kSampleSize>};
  testUniformWavelengthSampler(wavelength_sampler);
}
