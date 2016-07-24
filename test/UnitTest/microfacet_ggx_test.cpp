/*!
  \file microfacet_ggx_test.cpp
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
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Material/SurfaceModel/microfacet_ggx.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
// Test
#include "test.hpp"

// Forward declaration
void testGgxDEvaluation(const nanairo::Float roughness, nanairo::Sampler& sampler);

void testGgxDEvaluation(const nanairo::Float roughness, nanairo::Sampler& sampler)
{
  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = 0.01;
  constexpr uint m_vec_loop = 10000000;

  const Vector3 normal{0.0, 0.0, 1.0};

  CompensatedSummation<Float> p{0.0};
  for (uint i = 0; i < m_vec_loop; ++i) {
    // Sample a microfacet normal direction
    auto sampled_m_normal = 
        sampleDirectionOnHemisphere<0>(normal, sampler);
    const auto& m_normal = sampled_m_normal.direction();
    // Evaluate GGX D
    const auto cos_theta_nm = dot(normal, m_normal);
    const auto d = evaluateGgxD(roughness, cos_theta_nm);
    const auto weight = (cos_theta_nm * d) * sampled_m_normal.inversePdf();
    p.add(weight);
  }
  // Test the distribution of microfacet normals
  const auto d = p.get() / cast<Float>(m_vec_loop);
  ASSERT_NEAR(1.0, d, error) 
      << "GGX D evaluation test failed: roughness = " << roughness << ".";
}

TEST(MicrofacetGgxTest, GgxDEvaluationTest)
{
  using namespace nanairo;
  using zisc::cast;

  // Initialize
  SceneSettings settings;
  settings.open("unit_test.nana");
  settings.clear();
  // System
  auto system = makeTestSystem(512, 512, false, settings);
  auto& sampler = system->globalSampler();

  // Test
  for (int i = 1; i <= 10; ++i) {
    const auto roughness = cast<Float>(i) / cast<Float>(10);
    std::cout << "Roughness: " << roughness << std::endl;
    testGgxDEvaluation(roughness, sampler);
  }
}
