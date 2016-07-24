/*!
  \file camera_model_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
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
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/CameraModel/film.hpp"
#include "NanairoCore/CameraModel/pinhole_camera.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
// Test
#include "sensor_test.hpp"
#include "test.hpp"

TEST(CameraModelTest, PinholeTest)
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
  auto& memory_pool = system->globalMemoryPool();
  // Camera
  QString prefix{"PinholeTest"};
  auto key = prefix + "/" + keyword::jittering;
  settings.setBooleanValue(key, true);
  auto p = prefix + "/" + keyword::pinholeCamera;
  key = p + "/" + keyword::angleOfView;
  settings.setRealValue(key, 40.0);
  PinholeCamera camera{settings, prefix};
  // Film
  Film film{*system, settings};
  camera.setFilm(&film);
  const auto transformation = makeIdentityMatrix();
  camera.transform(transformation);
  // Wavelength
  WavelengthSamples<1> wavelengths;
  wavelengths[0] = kShortestWavelength;
  wavelengths.setPrimaryWavelength(0);


  const char* sensor_name = "PinholeCamera";
  testSensorSampling(camera, wavelengths, 
                     sampler, memory_pool, sensor_name);
  testSensorEnergyConservation(camera, wavelengths, 
                               sampler, memory_pool, sensor_name);
  testSensorImportanceSampling(camera, wavelengths, 
                               sampler, memory_pool, sensor_name);
}
