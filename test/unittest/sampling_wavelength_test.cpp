///*!
//  \file sampling_wavelengths_test.cpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2019 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//// Standard C++ library
//#include <array>
//#include <memory>
//// GoogleTest
//#include "gtest/gtest.h"
//// Test
//#include "test.hpp"
//// Nanairo
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/system.hpp"
//#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
//#include "NanairoCore/Sampling/wavelength_sampler.hpp"
//#include "NanairoCore/Utility/value.hpp"
//
//constexpr nanairo::uint kSampleSize = nanairo::CoreConfig::wavelengthSampleSize();
//
//namespace {
//
///*!
//  \details
//  No detailed.
//  */
//template <nanairo::uint N, typename WavelengthSampler>
//auto testWavelengthDistribution(const WavelengthSampler& wavelength_sampler,
//                                nanairo::Sampler& sampler)
//{
//  using namespace nanairo;
//  using zisc::cast;
//
//  std::array<uint, CoreConfig::spectraSize()> wavelength_distribution;
//  wavelength_distribution.fill(0);
//
//  for (uint i = 0; i < N; ++i) {
//    const auto sampled_wavelengths = wavelength_sampler(sampler);
//    const auto& wavelengths = sampled_wavelengths.wavelengths();
//    for (uint index = 0; index < kSampleSize; ++index) {
//      const auto spectra_index = getIndex(wavelengths[index]);
//      ++wavelength_distribution[spectra_index];
//    }
//  }
//  return wavelength_distribution;
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <typename WavelengthSampler>
//void testUniformWavelengthSampler(const WavelengthSampler& wavelength_sampler)
//{
//  using namespace nanairo;
//  using zisc::cast;
//
//  // Initialize test system
//  auto system = makeTestSystem(512, 512, false);
//  auto& sampler = system->globalSampler();
//
//  // Distribution test
//  constexpr uint n = 1'000'000'0;
//  constexpr uint loop_count = CoreConfig::spectraSize() * n;
//  constexpr uint expectation = kSampleSize * n;
//  constexpr uint error = kSampleSize * 1'000;
//  const auto wavelength_distribution =
//      testWavelengthDistribution<loop_count>(wavelength_sampler, sampler);
//  for (uint i = 0; i < CoreConfig::spectraSize(); ++i) {
//    const auto w = getWavelength(i);
//    const auto w_count = wavelength_distribution[i];
//    EXPECT_GT(w_count, expectation - error)
//        << "The wavelength (" << w << ") is less than the expectation value.";
//    EXPECT_LT(w_count, expectation + error)
//        << "The wavelength (" << w << ") is greater than the expectation value.";
//  }
//}
//
//} // namespace
//
//TEST(SamplingWavelengthsTest, RegularSamplingTest)
//{
//  using namespace nanairo;
//  auto wavelength_sampler = [](Sampler& sampler)
//  {
//    using WSampler = WavelengthSampler;
//    return WSampler::sampleRegularly(sampler);
//  };
//  testUniformWavelengthSampler(wavelength_sampler);
//}
//
//TEST(SamplingWavelengthsTest, RandomSamplingTest)
//{
//  using namespace nanairo;
//  auto wavelength_sampler = [](Sampler& sampler)
//  {
//    using WSampler = WavelengthSampler;
//    return WSampler::sampleRandomly(sampler);
//  };
//  testUniformWavelengthSampler(wavelength_sampler);
//}
//
//TEST(SamplingWavelengthsTest, StratifiedSamplingTest)
//{
//  using namespace nanairo;
//  auto wavelength_sampler = [](Sampler& sampler)
//  {
//    using WSampler = WavelengthSampler;
//    return WSampler::sampleStratified(sampler);
//  };
//  testUniformWavelengthSampler(wavelength_sampler);
//}
