/*!
  \file scene.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene.hpp"
// Standard C++ library
#include <future>
#include <utility>
// Zisc
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/unit.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "world.hpp"
#include "system.hpp"
#include "CameraModel/camera_model.hpp"
#include "CameraModel/film.hpp"
#include "Geometry/transformation.hpp"
#include "Setting/object_model_setting_node.hpp"
#include "Setting/scene_setting_node.hpp"
#include "Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Scene::Scene(System& system, const SettingNodeBase* settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void Scene::initialize(System& system, const SettingNodeBase* settings) noexcept
{
  const auto scene_settings = castNode<SceneSettingNode>(settings);
  auto& data_resource = system.dataMemoryManager();

  // Create a camera
  const auto camera_settings = scene_settings->cameraSettingNode();
  initializeCamera(system, camera_settings);

  // Create a world
  world_ = zisc::UniqueMemoryPointer<World>::make(&data_resource, system, settings);
}

/*!
  */
void Scene::initializeCamera(System& system, const SettingNodeBase* settings) noexcept
{
  const auto object_settings = castNode<ObjectModelSettingNode>(settings);
  auto& data_resource = system.dataMemoryManager();

  // Film
  film_ = zisc::UniqueMemoryPointer<Film>::make(&data_resource,
                                                system,
                                                object_settings->objectSettingNode());
  // Camera
  camera_ = CameraModel::makeCamera(system, object_settings->objectSettingNode());
  camera_->setFilm(film_.get());
  // Transformation
  const auto& transformation_list = object_settings->transformationList();
  if (0 < transformation_list.size()) {
    const auto transformation =
        Transformation::makeTransformation(transformation_list);
    camera_->transform(transformation);
  }
}

} // namespace nanairo
