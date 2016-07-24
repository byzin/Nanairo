/*!
  \file scene.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene.hpp"
// Standard C++ library
#include <functional>
#include <future>
#include <utility>
// Qt
#include <QJsonObject>
#include <QString>
#include <QLocale>
// Zisc
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
#include "zisc/unit.hpp"
// Nanairo
#include "world.hpp"
#include "system.hpp"
#include "CameraModel/camera_model.hpp"
#include "CameraModel/film.hpp"
#include "LinearAlgebra/transformation.hpp"
#include "Utility/scene_value.hpp"
#include "NanairoCommon/keyword.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Scene::Scene(System& system,
             const QJsonObject& settings,
             const std::function<void (const QString&)>& message_sender) noexcept
{
  initialize(system, settings, message_sender);
}

/*!
  \details
  No detailed.
  */
void Scene::initialize(System& system, 
                       const QJsonObject& settings,
                       const std::function<void (const QString&)>& message_sender) noexcept
{
  using zisc::cast;

  // Create a camera
  const auto object_settings_list = arrayValue(settings, keyword::object);
  const auto camera_settings = objectValue(object_settings_list[0]);
  auto make_camera = [this, &system, &camera_settings, message_sender]()
  {
    // Film
    film_ = new Film{system, camera_settings};
    // Camera
    camera_ = makeCameraModel(camera_settings);
    camera_->setFilm(film_.get());
    // Transformation
    const auto transformation_settings = arrayValue(camera_settings, 
                                                    keyword::transformation);
    const auto transformation = makeTransformationMatrix(transformation_settings);
    camera_->transform(transformation);
    
    const auto byte = zisc::toMegaByte(film().spectraBuffer().bufferMemorySize());
    const auto byte_string = QLocale{QLocale::English}.toString(byte, 'f', 3);
    const auto message = 
        QStringLiteral("    Spectra image buffer size: %1 MB.").arg(byte_string);
    message_sender(message);
  };
  auto& thread_pool = system.threadPool();
  auto camera_result = thread_pool.enqueue<void>(make_camera);

  // Create a world
  world_ = new World{system, settings, message_sender};
  
  // Wait for the initialization completion
  camera_result.get();
}

} // namespace nanairo
