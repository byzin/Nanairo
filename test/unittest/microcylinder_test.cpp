/*!
  \file microcylinder_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <iostream>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/compensated_summation.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Material/SurfaceModel/microcylinder.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
// Test
#include "test.hpp"

namespace  {

template <typename Function>
void testMicrocylinderPdf(Function eval_pdf,
                          const nanairo::Float gamma,
                          nanairo::Sampler& sampler)
{
  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = 0.02;
  constexpr uint vin_loop = 1'000;
  constexpr uint vout_loop = 1'000'000;

  const Vector3 normal{0.0, 0.0, 1.0};

  for (uint i = 0; i < vin_loop; ++i) {
    // Sample a microfacet normal direction
    const auto sampled_vin = SampledDirection::sampleOnHemisphere<0>(normal,
                                                                     sampler);
    const auto& vin = sampled_vin.direction();
    const Float theta_i = zisc::asin(vin[0]);
    const Float phi_i = zisc::atan(vin[1] / vin[2]);
    CompensatedSummation<Float> p{0.0};
    for (uint o = 0; o < vout_loop; ++o) {
      const auto sampled_vout = SampledDirection::sampleOnHemisphere<0>(normal,
                                                                        sampler);
      const auto& vout = sampled_vout.direction();
      const Float theta_o = zisc::asin(vout[0]);
      const Float phi_o = zisc::atan(vout[1] / vout[2]);
      const Float pdf = eval_pdf(theta_i, phi_i, theta_o, phi_o, gamma);
      p.add(pdf * sampled_vout.inversePdf());
    }
    const Float pdf = p.get() / cast<Float>(vout_loop);
    EXPECT_NEAR(1.0, pdf, error)
        << "The pdf of the surface reflection test failed: gamma = " << gamma;
  }
}

} // namespace

TEST(MicrocylinderTest, SurfacePdfTest)
{
  using nanairo::Float;
  using zisc::cast;

  // System
  auto system = makeTestSystem(512, 512, false);
  auto& sampler = system->globalSampler();

  const Float gamma_list[] = {12.0, 24.0, 5.0, 10.0, 18.0, 32.0,
                              2.5, 30.0, 60.0, 4.0, 6.0};
  auto eval_pdf = [](const Float theta_i, const Float phi_i,
                     const Float theta_o, const Float phi_o,
                     const Float gamma)
  {
    return nanairo::Microcylinder::evalSurfaceAnglesPdf(theta_i, phi_i,
                                                        theta_o, phi_o,
                                                        gamma);
  };

  // Test
  for (const Float gamma : gamma_list) {
    std::cout << "Gamma: " << gamma << std::endl;
    ::testMicrocylinderPdf(eval_pdf, gamma, sampler);
  }
}

TEST(MicrocylinderTest, VolumePdfTest)
{
  using nanairo::Float;
  using zisc::cast;

  // System
  auto system = makeTestSystem(512, 512, false);
  auto& sampler = system->globalSampler();

  const Float gamma_list[] = {24.0, 10.0, 32.0, 5.0, 60.0, 8.0, 12.0};
  const Float kd_list[] = {0.3, 0.2, 0.3, 0.1, 0.7, 0.1, 0.1};
  constexpr uint n = sizeof(kd_list) / sizeof(kd_list[0]);

  // Test
  for (uint i = 0; i < n; ++i) {
    const Float gamma = gamma_list[i];
    const Float k_d = kd_list[i];

    auto eval_pdf = [k_d](const Float theta_i, const Float phi_i,
                          const Float theta_o, const Float phi_o,
                          const Float g)
    {
      return nanairo::Microcylinder::evalVolumeAnglesPdf(theta_i, phi_i,
                                                         theta_o, phi_o,
                                                         k_d, g);
    };

    std::cout << "Gamma: " << gamma << std::endl;
    ::testMicrocylinderPdf(eval_pdf, gamma, sampler);
  }
}
