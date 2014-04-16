/*!
  \file surface_model_test.cpp
  \author Sho Ikeda
  
  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <iostream>
#include <memory>
#include <tuple>
// Qt
#include <QString>
// GoogleTest
#include "gtest/gtest.h"
// ZIsc
#include "zisc/compensated_summation.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/rough_conductor_surface.hpp"
#include "NanairoCore/Material/SurfaceModel/smooth_diffuse_surface.hpp"
#include "NanairoCore/Material/Texture/texture.hpp"
#include "NanairoCore/Material/Texture/value_texture.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
// Test
#include "shader_model_test.hpp"
#include "test.hpp"

TEST(SurfaceModelTest, SmoothDiffuseSurfaceTest)
{
  using namespace nanairo;
  using zisc::cast;
  // Initialize
  SceneSettings settings;
  settings.open("unit_test.nana");
  settings.clear();
  // System
  auto system = makeTestSystem(512, 512, false, settings);
  auto& memory_pool = system->globalMemoryPool();
  auto& sampler = system->globalSampler();
  // Texture
  auto prefix = QString{"Texture"};
  auto p = prefix + "/" + keyword::valueTexture;
  auto key = p + "/" + keyword::value;
  settings.setRealValue(key, 1.0);
  const auto texture = ValueTexture{*system, settings, prefix};
  std::vector<const Texture*> texture_list;
  texture_list.push_back(&texture);
  // Diffuse surface
  prefix = QString{"Surface"};
  p = prefix + "/" + keyword::smoothDiffuseSurface;
  key = p + "/" + keyword::reflectanceIndex;
  settings.setIntValue(key, 0);
  const auto surface = SmoothDiffuseSurface{settings, prefix, texture_list};
  // Intersection point
  const Point3 point{0.0, 0.0, 0.0};
  const Vector3 normal{0.0, 1.0, 0.0};
  const IntersectionInfo intersection{point, normal, nullptr, false};
  // Wavelengths
  WavelengthSamples<1> wavelengths;
  wavelengths[0] = kShortestWavelength;
  wavelengths.setPrimaryWavelength(0);

  constexpr char brdf_name[] = "Lambert BRDF";
  testBxdfSampling(surface, intersection, wavelengths, 
                   sampler, memory_pool, brdf_name);
  testBxdfHelmholtzReciprocity(surface, intersection, wavelengths, 
                               sampler, memory_pool, brdf_name);
  testBxdfEnergyConservation(surface, intersection, wavelengths, 
                             sampler, memory_pool, brdf_name);
  testBxdfImportanceSampling(surface, intersection, wavelengths, 
                             sampler, memory_pool, brdf_name);
}

void testRoughConductorSurface(const nanairo::Float roughness)
{
  using namespace nanairo;
  using zisc::cast;
  // Initialize
  SceneSettings settings;
  settings.open("unit_test.nana");
  settings.clear();
  // System
  auto system = makeTestSystem(512, 512, false, settings);
  auto& memory_pool = system->globalMemoryPool();
  auto& sampler = system->globalSampler();
  // Roughness texture
  auto prefix = QString{"Texture"};
  auto p = prefix + "/" + keyword::valueTexture;
  auto key = p + "/" + keyword::value;
  settings.setRealValue(key, roughness);
  const auto texture = ValueTexture{*system, settings, prefix};
  std::vector<const Texture*> texture_list;
  texture_list.push_back(&texture);
  // Rough conductor surface
  prefix = QString{"Surface"};
  p = prefix + "/" + keyword::roughConductorSurface;
  key = p + "/" + keyword::outerRefractiveIndex + 
            "/" + keyword::spectraFilePath;
  settings.setStringValue(key, "resources/spectrum/dielectric/Air.csv");
  key = p + "/" + keyword::innerRefractiveIndex +
            "/" + keyword::spectraFilePath;
  settings.setStringValue(key, "resources/spectrum/conductor/Ag refractive index.csv");
  key = p + "/" + keyword::innerExtinction +
            "/" + keyword::spectraFilePath;
  settings.setStringValue(key, "resources/spectrum/conductor/Ag extinction coefficient.csv");
  key = p + "/" + keyword::roughnessIndex;
  settings.setIntValue(key, 0);
  const auto surface = RoughConductorSurface{settings, prefix, texture_list};
  // Intersection point
  const Point3 point{0.0, 0.0, 0.0};
  const Vector3 normal{0.0, 1.0, 0.0};
  const IntersectionInfo intersection{point, normal, nullptr, false};
  // Wavelengths
  WavelengthSamples<1> wavelengths;
  wavelengths[0] = kShortestWavelength;
  wavelengths.setPrimaryWavelength(0);

  constexpr char brdf_name[] = "GGX conductor";
  testBxdfSampling(surface, intersection, wavelengths, 
                   sampler, memory_pool, brdf_name);
  testBxdfHelmholtzReciprocity(surface, intersection, wavelengths, 
                               sampler, memory_pool, brdf_name);
  testBxdfEnergyConservation(surface, intersection, wavelengths, 
                             sampler, memory_pool, brdf_name);
  testBxdfImportanceSampling(surface, intersection, wavelengths, 
                             sampler, memory_pool, brdf_name);
}

TEST(SurfaceModelTest, RoughConductorSurfaceTest)
{
  using namespace nanairo;
  using zisc::cast;

  for (int i = 1; i <= 10; ++i) {
    const auto roughness = cast<Float>(i) / cast<Float>(10);
    std::cout << "Roughness: " << roughness << std::endl;
    testRoughConductorSurface(roughness);
  }
}
