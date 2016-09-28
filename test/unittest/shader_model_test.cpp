/*!
  \file shader_model_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "shader_model_test.hpp"
// Standard C++ library
#include <iostream>
#include <tuple>
// Qt
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QtGlobal>
// GoogleTestk
#include "gtest/gtest.h"
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
#include "NanairoCore/Material/Texture/texture.hpp"
#include "NanairoCore/Material/Texture/value_texture.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

/*!
  */
nanairo::UniquePointer<nanairo::Texture> makeTestValueTexture(
    const nanairo::System& system,
    const nanairo::Float value)
{
  // Make value texture json
  auto json_text = QStringLiteral("{\"Value\":%1}");
  json_text = json_text.arg(QString::number(value));
  // Parse json
  QJsonObject settings;
  {
    QJsonParseError parse_result;
    const auto texture_document = QJsonDocument::fromJson(json_text.toLatin1(),
                                                          &parse_result);
    if (parse_result.error != QJsonParseError::NoError) {
      qFatal("Parsing test value json failed.");
    }
    settings = texture_document.object();
  }
  return nanairo::UniquePointer<nanairo::Texture>{
      new nanairo::ValueTexture{system, settings}};
}

constexpr double kError = 0.01;
constexpr int kLoopCountBase = 1'000;

/*!
  \details
  No detailed.
  */
void testBxdfSampling(
    const nanairo::SurfaceModel& surface,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name)
{
  std::cout << "  Test BxDF sampling." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = kError;
  constexpr uint in_vec_loop = kLoopCountBase;
  constexpr uint out_vec_loop = kLoopCountBase * 1000;

  const auto& normal = intersection.normal();
  for (uint i = 0; i < in_vec_loop; ++i) {
    CompensatedSummation<Float> p{0.0};
    // Sample a incident direction
    auto sampled_vin = sampleDirectionOnHemisphere<0>(-normal, sampler);
    const auto& vin = sampled_vin.direction();
    // Create a BxDF
    auto bxdf = surface.makeBxdf(intersection.textureCoordinate(),
                                 intersection.isReverseFace(),
                                 wavelengths,
                                 memory_pool);
    for (uint o = 0; o < out_vec_loop; ++o) {
      // Sample a reflection direction
      auto sampled_vout = sampleDirectionOnHemisphere<0>(normal, sampler);
      const auto& vout = sampled_vout.direction();
      // Test sampling
      auto result =
          bxdf->evaluateRadianceAndPdf(&vin, &vout, normal, wavelengths);
      const auto& f1 = std::get<0>(result);
      const auto& pdf1 = std::get<1>(result);
      const auto f2 =
          bxdf->evaluateRadiance(&vin, &vout, normal, wavelengths);
      const auto pdf2 =
          bxdf->evaluatePdf(&vin, &vout, normal, wavelengths);
      ASSERT_NEAR(f1.intensity(0), f2.intensity(0), error)
          << bxdf_name << ": Radiance evaluation test failed.";
      ASSERT_NEAR(pdf1, pdf2, error)
          << bxdf_name << ": PDF evaluation test failed.";
      p.add(pdf1 * sampled_vout.inversePdf());
    }
    // Test pdf
    const Float pdf = p.get() / cast<Float>(out_vec_loop);
    ASSERT_NEAR(1.0, pdf, error)
        << bxdf_name << ": PDF test failed.";
    memory_pool.reset();
  }
}

/*!
  \details
  No detailed.
  */
void testBxdfImportanceSampling(
    const nanairo::SurfaceModel& surface,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name)
{
  std::cout << "  Test BxDF importance sampling." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = kError;
  constexpr uint in_vec_loop = kLoopCountBase * kLoopCountBase;

  const auto& normal = intersection.normal();
  for (uint i = 0; i < in_vec_loop; ++i) {
    // Sample a incident direction
    auto sampled_vin = sampleDirectionOnHemisphere<0>(-normal, sampler);
    const auto& vin = sampled_vin.direction();
    // Create a BxDF
    auto bxdf = surface.makeBxdf(intersection.textureCoordinate(),
                                 intersection.isReverseFace(),
                                 wavelengths,
                                 memory_pool);
    // Test importance sampling
    const auto result1 = bxdf->sample(&vin, normal, wavelengths, sampler);
    const auto& sampled_vout = std::get<0>(result1);
    const auto& weight = std::get<1>(result1);
    const auto& vout = sampled_vout.direction();
    const auto result2 = 
        bxdf->evaluateRadianceAndPdf(&vin, &vout, normal, wavelengths);
    const auto& f = std::get<0>(result2);
    const auto& pdf = std::get<1>(result2);
    const Float cos_theta_no = dot(normal, vout);
//    ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_no),
//                "cos theta_{no} must be [0, 1].");
    const Float w = f.intensity(0) * cos_theta_no / pdf;
    ASSERT_NEAR(weight.intensity(0), w, error)
        << bxdf_name << ": Importance sampling test failed.";

    memory_pool.reset();
  }
}

/*!
  \details
  No detailed.
  */
void testBxdfEnergyConservation(
    const nanairo::SurfaceModel& surface,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name)
{
  std::cout << "  Test BxDF energy conservation." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = kError;
  constexpr uint in_vec_loop = kLoopCountBase;
  constexpr uint out_vec_loop = kLoopCountBase * 1000;

  const auto& normal = intersection.normal();
  for (uint i = 0; i < in_vec_loop; ++i) {
    CompensatedSummation<Float> e{0.0};
    // Sample a incident direction
    auto sampled_vin = sampleDirectionOnHemisphere<0>(-normal, sampler);
    const auto& vin = sampled_vin.direction();
    // Create a BxDF
    auto bxdf = surface.makeBxdf(intersection.textureCoordinate(),
                                 intersection.isReverseFace(),
                                 wavelengths,
                                 memory_pool);
    for (uint o = 0; o < out_vec_loop; ++o) {
      // Sample a reflection direction
      auto sampled_vout = sampleDirectionOnHemisphere<0>(normal, sampler);
      const auto& vout = sampled_vout.direction();
      // Sample energy of a BxDF
      const auto f = 
          bxdf->evaluateRadiance(&vin, &vout, normal, wavelengths);
      // Positive inensity test
      ASSERT_TRUE(0.0 <= f.intensity(0))
          << bxdf_name << ": Positive intensity test failed.";
      const Float cos_theta_no = dot(normal, vout);
//      ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_no),
//                  "cos theta_{no} must be [0, 1].");
      const Float inverse_pdf = sampled_vout.inversePdf();
      const auto energy = f.intensity(0) * cos_theta_no * inverse_pdf;
      e.add(energy);
    }
    // Test Energy Conservation of BxDF
    const Float energy = e.get() / cast<Float>(out_vec_loop);
    ASSERT_TRUE((0.0 <= (energy + error)) && ((energy - error) <= 1.0))
        << bxdf_name << ": Energy Conservation test failed. "
        << "Energy = " << energy << ".";
    memory_pool.reset();
  }
}

/*!
  \details
  No detailed.
  */
void testBxdfHelmholtzReciprocity(
    const nanairo::SurfaceModel& surface,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name)
{
  std::cout << "  Test BxDF Helmholtz reciprocity." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = kError;
  constexpr uint in_vec_loop = kLoopCountBase;
  constexpr uint out_vec_loop = kLoopCountBase * 100;

  const auto& normal = intersection.normal();
  for (uint i = 0; i < in_vec_loop; ++i) {
    // Sample a incident direction
    auto sampled_vin = sampleDirectionOnHemisphere<0>(-normal, sampler);
    const auto& vin = sampled_vin.direction();
    for (uint o = 0; o < out_vec_loop; ++o) {
      // Sample a reflection direction
      auto sampled_vout = sampleDirectionOnHemisphere<0>(normal, sampler);
      const auto& vout = sampled_vout.direction();
      // Create a BxDF1
      auto bxdf1 = surface.makeBxdf(intersection.textureCoordinate(),
                                    intersection.isReverseFace(),
                                    wavelengths,
                                    memory_pool);
      const auto f1 = 
          bxdf1->evaluateRadiance(&vin, &vout, normal, wavelengths);
      // Create a BxDF2
      const auto rvin = -vin;
      const auto rvout = -vout;
      auto bxdf2 = surface.makeBxdf(intersection.textureCoordinate(),
                                    intersection.isReverseFace(),
                                    wavelengths,
                                    memory_pool);
      const auto f2 = 
          bxdf2->evaluateRadiance(&rvout, &rvin, normal, wavelengths);
      // Test Helmholtz Reciprocity of BxDF
      ASSERT_NEAR(f1.intensity(0), f2.intensity(0), error)
          << bxdf_name << ": Helmholtz Reciprocity test failed.";
      memory_pool.reset();
    }
  }
}
