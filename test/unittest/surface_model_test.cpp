/*!
  \file surface_model_test.cpp
  \author Sho Ikeda
  
  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <iostream>
#include <memory>
#include <tuple>
// Qt
#include <QByteArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QtGlobal>
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
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"
// Test
#include "shader_model_test.hpp"
#include "test.hpp"

namespace {

template <typename SurfaceType>
nanairo::UniquePointer<nanairo::SurfaceModel> makeTestSurface(
    const QString& json_path,
    const std::vector<const nanairo::Texture*> texture_list)
{
  // Open surface json
  QFile json_file{json_path};
  const bool is_open = json_file.open(QIODevice::ReadOnly | QIODevice::Text);
  if (!is_open) {
    qCritical("Test resource file \"%s\" open failed.", qUtf8Printable(json_path));
  }
  //
  QJsonObject settings;
  {
    QJsonParseError parse_result;
    const auto surface_document = QJsonDocument::fromJson(json_file.readAll(),
                                                          &parse_result);
    if (parse_result.error != QJsonParseError::NoError) {
      qFatal("Parsing test surface json failed.");
    }
    settings = surface_document.object();
  }
  return nanairo::UniquePointer<nanairo::SurfaceModel>{
      new SurfaceType{settings, texture_list}};
}

} // namespace

TEST(SurfaceModelTest, SmoothDiffuseSurfaceTest)
{
  using namespace nanairo;
  using zisc::cast;

  // System
  auto system = makeTestSystem(512, 512, false);
  auto& memory_pool = system->globalMemoryPool();
  auto& sampler = system->globalSampler();

  // Texture
  auto texture = makeTestValueTexture(*system, 1.0);
  std::vector<const Texture*> texture_list;
  texture_list.push_back(texture.get());

  // Diffuse surface
  const auto json_path = QStringLiteral(":/test/test_smooth_diffuse_surface.json");
  const auto surface = ::makeTestSurface<SmoothDiffuseSurface>(json_path,
                                                               texture_list);

  // Intersection point
  const Point3 point{0.0, 0.0, 0.0};
  const Vector3 normal{0.0, 1.0, 0.0};
  const IntersectionInfo intersection{point, normal, nullptr, false};

  // Wavelengths
  WavelengthSamples<1> wavelengths;
  wavelengths[0] = kShortestWavelength;
  wavelengths.setPrimaryWavelength(0);

  constexpr char brdf_name[] = "Lambert BRDF";
  testBxdfSampling(*surface, intersection, wavelengths, 
                   sampler, memory_pool, brdf_name);
  testBxdfHelmholtzReciprocity(*surface, intersection, wavelengths, 
                               sampler, memory_pool, brdf_name);
  testBxdfEnergyConservation(*surface, intersection, wavelengths, 
                             sampler, memory_pool, brdf_name);
  testBxdfImportanceSampling(*surface, intersection, wavelengths, 
                             sampler, memory_pool, brdf_name);
}

TEST(SurfaceModelTest, RoughConductorSurfaceTest)
{
  using namespace nanairo;
  using zisc::cast;

  // System
  auto system = makeTestSystem(512, 512, false);
  auto& memory_pool = system->globalMemoryPool();
  auto& sampler = system->globalSampler();

  for (int i = 1; i <= 10; ++i) {
    const auto roughness = cast<Float>(i) / cast<Float>(10);
    std::cout << "Roughness: " << roughness << std::endl;

    // Roughness texture
    auto texture = makeTestValueTexture(*system, 1.0);
    std::vector<const Texture*> texture_list;
    texture_list.push_back(texture.get());

    // Rough conductor surface
    const auto json_path = QStringLiteral(":/test/test_rough_conductor_surface.json");
    const auto surface = ::makeTestSurface<RoughConductorSurface>(json_path,
                                                                  texture_list);

    // Intersection point
    const Point3 point{0.0, 0.0, 0.0};
    const Vector3 normal{0.0, 1.0, 0.0};
    const IntersectionInfo intersection{point, normal, nullptr, false};

    // Wavelengths
    WavelengthSamples<1> wavelengths;
    wavelengths[0] = kShortestWavelength;
    wavelengths.setPrimaryWavelength(0);

    constexpr char brdf_name[] = "GGX conductor";
    testBxdfSampling(*surface, intersection, wavelengths, 
                     sampler, memory_pool, brdf_name);
    testBxdfHelmholtzReciprocity(*surface, intersection, wavelengths, 
                                 sampler, memory_pool, brdf_name);
    testBxdfEnergyConservation(*surface, intersection, wavelengths, 
                               sampler, memory_pool, brdf_name);
    testBxdfImportanceSampling(*surface, intersection, wavelengths, 
                               sampler, memory_pool, brdf_name);
  }
}
