/*!
  \file scene.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene.hpp"
// Standard C++ library
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
#include "Geometry/transformation.hpp"
#include "Utility/scene_value.hpp"
#include "NanairoCommon/keyword.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Scene::Scene(System& system, const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void Scene::initialize(System& system, const QJsonObject& settings) noexcept
{
  using zisc::cast;

  // Create a camera
  const auto object_settings_list = SceneValue::toArray(settings, keyword::object);
  const auto camera_settings = SceneValue::toObject(object_settings_list[0]);
  auto make_camera = [this, &system, &camera_settings]()
  {
    // Film
    film_ = new Film{system, camera_settings};
    // Camera
    camera_ = CameraModel::makeModel(camera_settings);
    camera_->setFilm(film_.get());
    // Transformation
    const auto transformation_settings =
        SceneValue::toArray(camera_settings, keyword::transformation);
    const auto transformation =
        Transformation::makeTransformation(transformation_settings);
    camera_->transform(transformation);
  };
  auto& thread_pool = system.threadPool();
  auto camera_result = thread_pool.enqueue<void>(make_camera);

  // Create a world
  world_ = new World{system, settings};

  // Wait for the initialization completion
  camera_result.get();
}

} // namespace nanairo
