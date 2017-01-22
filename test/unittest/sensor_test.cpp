/*!
  \file sensor_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "sensor_test.hpp"
// Standard C++ library
#include <iostream>
#include <tuple>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Sensor/sensor.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

using nanairo::uint;

constexpr double kError = 0.01;
constexpr uint kLoopCount = 1'000'000'00;

/*!
  \details
  No detailed.
  */
void testSensorSampling(
    const nanairo::CameraModel& camera,
    const nanairo::WavelengthSamples& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* sensor_name)
{
  std::cout << "  Test sensor sampling." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = kError;
  constexpr uint vec_loop = kLoopCount;

  const auto& normal = camera.normal();
  CompensatedSummation<Float> p{0.0};
  for (uint i = 0; i < vec_loop; ++i) {
    const auto sampled_vout = SampledDirection::sampleOnHemisphere<0>(normal, sampler);
    const auto& vout = sampled_vout.direction();
    uint x = 0,
         y = 0;
    const bool is_hit = camera.getPixelLocation(-vout, &x, &y);
    if (is_hit) {
      const auto sensor = camera.makeSensor(x, y, wavelengths, memory_pool);
      const auto result = sensor->evalRadianceAndPdf(nullptr, &vout,
                                                     normal, wavelengths);
      const auto& f1 = std::get<0>(result);
      const auto& pdf1 = std::get<1>(result);
      const auto f2 =
          sensor->evalRadiance(nullptr, &vout, normal, wavelengths);
      const auto pdf2 =
          sensor->evalPdf(nullptr, &vout, normal, wavelengths);
      ASSERT_NEAR(f1.intensity(0), f2.intensity(0), error)
          << sensor_name << ": Radiance evaluation test failed.";
      ASSERT_NEAR(pdf1, pdf2, error)
          << sensor_name << ": PDF evaluation test failed.";
      p.add(pdf1 * sampled_vout.inversePdf());
    }
    memory_pool.reset();
  }
  // Test pdf
  const Float pdf = p.get() / cast<Float>(vec_loop);
  ASSERT_NEAR(1.0, pdf, error)
      << sensor_name << ": PDF test failed.";
}

/*!
  \details
  No detailed.
  */
void testSensorEnergyConservation(
    const nanairo::CameraModel& camera,
    const nanairo::WavelengthSamples& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* sensor_name)
{
  std::cout << "  Test sensor energy conservation." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = kError;
  constexpr uint vec_loop = kLoopCount;

  const auto& normal = camera.normal();
  CompensatedSummation<Float> e{0.0};
  for (uint i = 0; i < vec_loop; ++i) {
    const auto sampled_vout = SampledDirection::sampleOnHemisphere<0>(normal, sampler);
    const auto& vout = sampled_vout.direction();
    uint x = 0,
         y = 0;
    const bool is_hit = camera.getPixelLocation(-vout, &x, &y);
    if (is_hit) {
      const auto sensor =
          camera.makeSensor(x, y, wavelengths, memory_pool);
      const auto f =
          sensor->evalRadiance(nullptr, &vout, normal, wavelengths);
      // Positive intensity test
      ASSERT_TRUE(0.0 <= f.intensity(0))
          << sensor_name << ": Positive intensity test failed.";
      const Float cos_theta_no = dot(normal, vout);

      const Float inverse_pdf = sampled_vout.inversePdf();
      const auto energy = f.intensity(0) * cos_theta_no * inverse_pdf;
      e.add(energy);
    }
    memory_pool.reset();
  }
  // Test Energy conservation of BxDF
  const Float energy = e.get() / cast<Float>(vec_loop);
  ASSERT_TRUE((0.0 <= (energy + error)) && ((energy - error) <= 1.0))
      << sensor_name << ": Energy conservation test failed. "
      << "Energy = " << energy << ".";
}

/*!
  \details
  No detailed.
  */
void testSensorImportanceSampling(
    const nanairo::CameraModel& camera,
    const nanairo::WavelengthSamples& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* sensor_name)
{
  std::cout << "  Test sensor importance sampling." << std::endl;

  using namespace nanairo;
  using namespace zisc;

  constexpr Float error = kError;

  const auto& normal = camera.normal();
  CompensatedSummation<Float> p{0.0};

  for (uint y = 0; y < camera.heightResolution(); ++y) {
    for (uint x = 0; x < camera.widthResolution(); ++x) {
      const auto sensor = camera.makeSensor(x, y, wavelengths, memory_pool);
      // Test importance sampling
      const auto result1 = sensor->sample(nullptr, normal, wavelengths, sampler);
      const auto& sampled_vout = std::get<0>(result1);
      const auto& weight = std::get<1>(result1);
      const auto& vout = sampled_vout.direction();
      const auto result2 = sensor->evalRadianceAndPdf(nullptr, &vout,
                                                      normal, wavelengths);
      const auto& f = std::get<0>(result2);
      const auto& pdf = std::get<1>(result2);
      const Float cos_theta_no = dot(normal, vout);
      const Float w = f.intensity(0) * cos_theta_no / pdf;
      ASSERT_NEAR(weight.intensity(0), w, error)
          << sensor_name << ": Importance sampling test failed.";

      memory_pool.reset();
    }
  }
}
