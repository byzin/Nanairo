/*!
  \file shader_model_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Material/TextureModel/value_texture.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

/*!
  */
nanairo::UniquePointer<nanairo::TextureModel> makeTestValueTexture(
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
  return nanairo::UniquePointer<nanairo::TextureModel>{
      new nanairo::ValueTexture{system, settings}};
}

/*!
  */
nanairo::WavelengthSamples makeTestWavelengthSamples()
{
  nanairo::WavelengthSamples wavelengths;
  wavelengths[0] = nanairo::CoreConfig::shortestWavelength();
  wavelengths[1] = nanairo::CoreConfig::shortestWavelength();
  wavelengths[2] = nanairo::CoreConfig::shortestWavelength();
  wavelengths.setPrimaryWavelength(0);
  return wavelengths;
}

/*!
  \details
  No detailed.
  */
void testBxdfSampling(
    const nanairo::SurfaceModel& surface,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name,
    const bool compare_reflectance)
{
  std::cout << "  Test BxDF sampling." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = 0.02;
  constexpr uint in_vec_loop = 1'000;
  constexpr uint out_vec_loop = 1'000'000;

  uint success_count = 0,
       failure_count = 0;

  const auto& normal = intersection.normal();
  for (uint i = 0; i < in_vec_loop; ++i) {
    CompensatedSummation<Float> p{0.0};
    // Sample a incident direction
    auto sampled_vin = SampledDirection::sampleOnHemisphere<0>(-normal, sampler);
    const auto& vin = sampled_vin.direction();
    // Create a BxDF
    auto bxdf = surface.makeBxdf(intersection.textureCoordinate(),
                                 intersection.isReverseFace(),
                                 wavelengths,
                                 sampler,
                                 memory_pool);
    for (uint o = 0; o < out_vec_loop; ++o) {
      // Sample a reflection direction
      auto sampled_vout = SampledDirection::sampleOnHemisphere<0>(normal, sampler);
      const auto& vout = sampled_vout.direction();
      // Test sampling
      auto result =
          bxdf->evalRadianceAndPdf(&vin, &vout, normal, wavelengths);
      const auto& f1 = std::get<0>(result);
      const auto& pdf1 = std::get<1>(result);
      const auto f2 =
          bxdf->evalRadiance(&vin, &vout, normal, wavelengths);
      const auto pdf2 =
          bxdf->evalPdf(&vin, &vout, normal, wavelengths);
      if (compare_reflectance) {
        ASSERT_NEAR(f1.intensity(0), f2.intensity(0), error)
            << bxdf_name << ": Radiance evaluation test failed.";
      }
      ASSERT_NEAR(pdf1, pdf2, error)
          << bxdf_name << ": PDF evaluation test failed.";
      p.add(pdf1 * sampled_vout.inversePdf());
    }
    // Test pdf
    const Float pdf = p.get() / cast<Float>(out_vec_loop);
    if(zisc::isInClosedBounds(pdf, 1.0 - error, 1.0 + error))
      ++success_count;
    else
      ++failure_count;
    memory_pool.reset();
  }
  std::cout << "    PDF test success: " << success_count << std::endl
            << "             failure: " << failure_count << std::endl;
}

/*!
  \details
  No detailed.
  */
void testBxdfImportanceSampling(
    const nanairo::SurfaceModel& surface,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name)
{
  std::cout << "  Test BxDF importance sampling." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = 0.01;
  constexpr uint in_vec_loop = 10'000'000;

  const auto& normal = intersection.normal();
  for (uint i = 0; i < in_vec_loop; ++i) {
    // Sample a incident direction
    auto sampled_vin = SampledDirection::sampleOnHemisphere<0>(-normal, sampler);
    const auto& vin = sampled_vin.direction();
    // Create a BxDF
    auto bxdf = surface.makeBxdf(intersection.textureCoordinate(),
                                 intersection.isReverseFace(),
                                 wavelengths,
                                 sampler,
                                 memory_pool);
    // Test importance sampling
    const auto result1 = bxdf->sample(&vin, normal, wavelengths, sampler);
    const auto& sampled_vout = std::get<0>(result1);
    const auto& weight = std::get<1>(result1);
    const auto& vout = sampled_vout.direction();
    const Float cos_no = dot(normal, vout);
    if (zisc::isInClosedBounds(cos_no, 0.0, 1.0)) {
      const auto result2 = 
          bxdf->evalRadianceAndPdf(&vin, &vout, normal, wavelengths);
      const auto& f = std::get<0>(result2);
      const auto& pdf = std::get<1>(result2);
      const Float w = f.intensity(0) * cos_no / pdf;
      ASSERT_NEAR(weight.intensity(0), w, error)
          << bxdf_name << ": Importance sampling test failed.";
    }
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
    const nanairo::WavelengthSamples& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name)
{
  std::cout << "  Test BxDF energy conservation." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = 0.01;
  constexpr uint in_vec_loop = 1'000;
  constexpr uint out_vec_loop = 1'000'000;

  uint success_count = 0,
       failure_count = 0;

  const auto& normal = intersection.normal();
  for (uint i = 0; i < in_vec_loop; ++i) {
    CompensatedSummation<Float> e{0.0};
    // Sample a incident direction
    auto sampled_vin = SampledDirection::sampleOnHemisphere<0>(-normal, sampler);
    const auto& vin = sampled_vin.direction();
    // Create a BxDF
    auto bxdf = surface.makeBxdf(intersection.textureCoordinate(),
                                 intersection.isReverseFace(),
                                 wavelengths,
                                 sampler,
                                 memory_pool);
    for (uint o = 0; o < out_vec_loop; ++o) {
      // Sample a reflection direction
      auto sampled_vout = SampledDirection::sampleOnHemisphere<0>(normal, sampler);
      const auto& vout = sampled_vout.direction();
      // Sample energy of a BxDF
      const auto f = 
          bxdf->evalRadiance(&vin, &vout, normal, wavelengths);
      // Positive inensity test
      ASSERT_TRUE(0.0 <= f.intensity(0))
          << bxdf_name << ": Positive intensity test failed.";
      const Float cos_no = dot(normal, vout);
      ZISC_ASSERT(zisc::isInClosedBounds(cos_no, 0.0, 1.0),
                  "The cos theta(no) isn't [0, 1].");
      const Float inverse_pdf = sampled_vout.inversePdf();
      const auto energy = f.intensity(0) * cos_no * inverse_pdf;
      e.add(energy);
    }
    // Test Energy Conservation of BxDF
    const Float energy = e.get() / cast<Float>(out_vec_loop);
    if(zisc::isInClosedBounds(energy, 0.0, 1.0 + error))
      ++success_count;
    else
      ++failure_count;
    memory_pool.reset();
  }
  std::cout << "    Energy conservation test success: " << success_count << std::endl
            << "                             failure: " << failure_count << std::endl;
}

/*!
  \details
  No detailed.
  */
void testBxdfHelmholtzReciprocity(
    const nanairo::SurfaceModel& surface,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name)
{
  std::cout << "  Test BxDF Helmholtz reciprocity." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = 0.01;
  constexpr uint in_vec_loop = 1'000;
  constexpr uint out_vec_loop = 1'000'000;

  const auto& normal = intersection.normal();
  for (uint i = 0; i < in_vec_loop; ++i) {
    // Sample a incident direction
    auto sampled_vin = SampledDirection::sampleOnHemisphere<0>(-normal, sampler);
    const auto& vin = sampled_vin.direction();
    for (uint o = 0; o < out_vec_loop; ++o) {
      // Sample a reflection direction
      auto sampled_vout = SampledDirection::sampleOnHemisphere<0>(normal, sampler);
      const auto& vout = sampled_vout.direction();
      // Create a BxDF1
      auto bxdf1 = surface.makeBxdf(intersection.textureCoordinate(),
                                    intersection.isReverseFace(),
                                    wavelengths,
                                    sampler,
                                    memory_pool);
      const auto f1 = 
          bxdf1->evalRadiance(&vin, &vout, normal, wavelengths);
      // Create a BxDF2
      const auto rvin = -vin;
      const auto rvout = -vout;
      auto bxdf2 = surface.makeBxdf(intersection.textureCoordinate(),
                                    intersection.isReverseFace(),
                                    wavelengths,
                                    sampler,
                                    memory_pool);
      const auto f2 = 
          bxdf2->evalRadiance(&rvout, &rvin, normal, wavelengths);
      // Test Helmholtz Reciprocity of BxDF
      ASSERT_NEAR(f1.intensity(0), f2.intensity(0), error)
          << bxdf_name << ": Helmholtz Reciprocity test failed.";
      memory_pool.reset();
    }
  }
}
