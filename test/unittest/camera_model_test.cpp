///*!
//  \file camera_model_test.cpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2018 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//// Standard C++ library
//#include <memory>
//#include <tuple>
//#include <utility>
//// Qt
//#include <QByteArray>
//#include <QFile>
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QString>
//#include <QtGlobal>
//// GoogleTest
//#include "gtest/gtest.h"
//// ZIsc
//#include "zisc/compensated_summation.hpp"
//#include "zisc/utility.hpp"
//// Nanairo
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/system.hpp"
//#include "NanairoCore/CameraModel/camera_model.hpp"
//#include "NanairoCore/CameraModel/film.hpp"
//#include "NanairoCore/CameraModel/pinhole_camera.hpp"
//#include "NanairoCore/Data/wavelength_samples.hpp"
//#include "NanairoCore/Geometry/transformation.hpp"
//// Test
//#include "sensor_test.hpp"
//#include "shader_model_test.hpp"
//#include "test.hpp"
//
//namespace  {
//
//std::tuple<std::unique_ptr<nanairo::PinholeCamera>, std::unique_ptr<nanairo::Film>>
//makeTestPinholeCamera(const nanairo::System& system)
//{
//  // Open system json
//  const auto json_path = QStringLiteral(":/test/test_pinhole_camera.json");
//  QFile json_file{json_path};
//  const bool is_open = json_file.open(QIODevice::ReadOnly | QIODevice::Text);
//  if (!is_open) {
//    qCritical("Test resource file \"%s\" open failed.", qUtf8Printable(json_path));
//  }
//  QJsonObject settings;
//  {
//    QJsonParseError parse_result;
//    const auto camera_document = QJsonDocument::fromJson(json_file.readAll(),
//                                                         &parse_result);
//    if (parse_result.error != QJsonParseError::NoError) {
//      qFatal("Parsing test pinhole camera json failed.");
//    }
//    settings = camera_document.object();
//  }
//
//  auto camera = std::make_unique<nanairo::PinholeCamera>(settings);
//  auto film = std::make_unique<nanairo::Film>(system, settings);
//  return std::make_tuple(std::move(camera), std::move(film));
//}
//
//} // namespace
//
//TEST(CameraModelTest, PinholeTest)
//{
//  using zisc::cast;
//
//  // System
//  auto system = makeTestSystem(512, 512, false);
//  auto& sampler = system->globalSampler();
//  auto& memory_pool = system->globalMemoryPool();
//
//  // Camera
//  auto camera_set = ::makeTestPinholeCamera(*system);
//  auto& camera = *std::get<0>(camera_set);
//  auto& film = *std::get<1>(camera_set);
//  camera.setFilm(&film);
//  camera.transform(nanairo::Transformation::makeIdentity());
//
//  // Wavelength
//  const auto wavelengths = makeTestWavelengthSamples();
//
//  const char* sensor_name = "PinholeCamera";
//  testSensorSampling(camera, wavelengths,
//                     sampler, memory_pool, sensor_name);
//  testSensorEnergyConservation(camera, wavelengths,
//                               sampler, memory_pool, sensor_name);
//  testSensorImportanceSampling(camera, wavelengths,
//                               sampler, memory_pool, sensor_name);
//}
