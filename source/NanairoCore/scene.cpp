/*!
  \file scene.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene.hpp"
// Standard C++ library
#include <functional>
#include <future>
#include <utility>
// Qt
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
#include "NanairoCommon/keyword.hpp"
#include "Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Scene::Scene(System& system,
             const SceneSettings& settings,
             const std::function<void (const QString&)>& message_sender) noexcept
{
  initialize(system, settings, message_sender);
}

/*!
  \details
  No detailed.
  */
void Scene::initialize(System& system, 
                       const SceneSettings& settings,
                       const std::function<void (const QString&)>& message_sender) noexcept
{
  using zisc::cast;

  auto& thread_pool = system.threadPool();

  // Create a camera
  std::function<void ()> initialize_camera{
  [this, &system, &settings, message_sender]()
  {
    // Film
    film_ = new Film{system, settings};
    // Camera
    const auto object_id = QString{keyword::object} + "/0";
    camera_ = makeCameraModel(settings, object_id);
    camera_->setFilm(film_.get());
    const auto transformation = makeTransformationMatrix(settings, object_id);
    camera_->transform(transformation);
    
    const auto byte = zisc::toMegaByte(film().spectraBuffer().bufferMemorySize());
    const auto byte_string = QLocale{QLocale::English}.toString(byte, 'f', 3);
    const auto message = 
        QStringLiteral("    Spectra image buffer size: %1 MB.").arg(byte_string);
    message_sender(message);
  }};
  auto camera_result = thread_pool.enqueue(std::move(initialize_camera));

  // Create a world
  world_ = new World{system, settings, message_sender};
  
  camera_result.get();
}

} // namespace nanairo
