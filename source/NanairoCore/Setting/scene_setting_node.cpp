/*!
  \file scene_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <ostream>
#include <string>
#include <utility>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "bvh_setting_node.hpp"
#include "camera_setting_node.hpp"
#include "group_object_setting_node.hpp"
#include "material_setting_node.hpp"
#include "object_model_setting_node.hpp"
#include "rendering_method_setting_node.hpp"
#include "setting_node_base.hpp"
#include "system_setting_node.hpp"

namespace nanairo {

/*!
  */
SceneSettingNode::SceneSettingNode() noexcept :
    system_setting_node_{new SystemSettingNode{}},
    rendering_method_setting_node_{new RenderingMethodSettingNode{}},
    texture_model_setting_node_{new TextureModelSettingNode{}},
    surface_model_setting_node_{new SurfaceModelSettingNode{}},
    emitter_model_setting_node_{new EmitterModelSettingNode{}},
    camera_setting_node_{new ObjectModelSettingNode{}},
    object_setting_node_{new ObjectModelSettingNode{}},
    bvh_setting_node_{new BvhSettingNode{}}
{
}

/*!
  */
SettingNodeBase* SceneSettingNode::bvhSettingNode() noexcept
{
  ZISC_ASSERT(bvh_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(bvh_setting_node_->type() == SettingNodeType::kBvh,
              "The node isn't BVH setting node.");
  return bvh_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::bvhSettingNode() const noexcept
{
  ZISC_ASSERT(bvh_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(bvh_setting_node_->type() == SettingNodeType::kBvh,
              "The node isn't BVH setting node.");
  return bvh_setting_node_.get();
}

/*!
  */
SettingNodeBase* SceneSettingNode::cameraSettingNode() noexcept
{
  ZISC_ASSERT(camera_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(camera_setting_node_->type() == SettingNodeType::kObjectModel,
              "The node isn't Camera setting node.");
  return camera_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::cameraSettingNode() const noexcept
{
  ZISC_ASSERT(camera_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(camera_setting_node_->type() == SettingNodeType::kObjectModel,
              "The node isn't Camera setting node.");
  return camera_setting_node_.get();
}

/*!
  */
SettingNodeBase* SceneSettingNode::emitterModelSettingNode() noexcept
{
  ZISC_ASSERT(emitter_model_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(emitter_model_setting_node_->type() == SettingNodeType::kEmitterModel,
              "The node isn't emitter model setting node.");
  return emitter_model_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::emitterModelSettingNode() const noexcept
{
  ZISC_ASSERT(emitter_model_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(emitter_model_setting_node_->type() == SettingNodeType::kEmitterModel,
              "The node isn't emitter model setting node.");
  return emitter_model_setting_node_.get();
}

/*!
  */
void SceneSettingNode::initialize() noexcept
{
  setSceneName("DefaultScene");
  systemSettingNode()->initialize();
  renderingMethodSettingNode()->initialize();
  textureModelSettingNode()->initialize();
  surfaceModelSettingNode()->initialize();
  emitterModelSettingNode()->initialize();
  cameraSettingNode()->initialize();
  objectSettingNode()->initialize();
  bvhSettingNode()->initialize();
}

/*!
  */
void SceneSettingNode::log() const noexcept
{
  zisc::logMessage("\n");

  zisc::logMessage("Scene name: \"", sceneName(), "\"");
  systemSettingNode()->log();
  renderingMethodSettingNode()->log();
  textureModelSettingNode()->log();
  surfaceModelSettingNode()->log();
  emitterModelSettingNode()->log();
  cameraSettingNode()->log();
  objectSettingNode()->log();
  bvhSettingNode()->log();

  zisc::logMessage("\n");
}

/*!
  */
SettingNodeBase* SceneSettingNode::objectSettingNode() noexcept
{
  ZISC_ASSERT(object_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(object_setting_node_->type() == SettingNodeType::kObjectModel,
              "The node isn't object setting node.");
  return object_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::objectSettingNode() const noexcept
{
  ZISC_ASSERT(object_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(object_setting_node_->type() == SettingNodeType::kObjectModel,
              "The node isn't object setting node.");
  return object_setting_node_.get();
}

/*!
  */
void SceneSettingNode::readData(std::istream* data_stream) noexcept
{
  SettingNodeType t = readType(data_stream);
  ZISC_ASSERT(type() == t, "The stream header is wrong.");

  // Read properties
  auto scene_name = readString(data_stream);
  setSceneName(std::move(scene_name));

  // Child nodes
  // System
  zisc::read(&t, data_stream);
  ZISC_ASSERT(SettingNodeType::kSystem == t, "The stream header is wrong.");
  systemSettingNode()->readData(data_stream);
  // Rendering method
  zisc::read(&t, data_stream);
  ZISC_ASSERT(SettingNodeType::kRenderingMethod == t, "The stream header is wrong.");
  renderingMethodSettingNode()->readData(data_stream);
  // Texture model
  zisc::read(&t, data_stream);
  ZISC_ASSERT(SettingNodeType::kTextureModel == t, "The stream header is wrong.");
  textureModelSettingNode()->readData(data_stream);
  // Surface model
  zisc::read(&t, data_stream);
  ZISC_ASSERT(SettingNodeType::kSurfaceModel == t, "The stream header is wrong.");
  surfaceModelSettingNode()->readData(data_stream);
  // Emitter model
  zisc::read(&t, data_stream);
  ZISC_ASSERT(SettingNodeType::kEmitterModel == t, "The stream header is wrong.");
  emitterModelSettingNode()->readData(data_stream);
  // Object
  // Camera
  zisc::read(&t, data_stream);
  ZISC_ASSERT(SettingNodeType::kObjectModel == t, "The stream header is wrong.");
  cameraSettingNode()->readData(data_stream);
  // Objects
  zisc::read(&t, data_stream);
  ZISC_ASSERT(SettingNodeType::kObjectModel == t, "The stream header is wrong.");
  objectSettingNode()->readData(data_stream);
  // BVH
  zisc::read(&t, data_stream);
  ZISC_ASSERT(SettingNodeType::kBvh == t, "The stream header is wrong.");
  bvhSettingNode()->readData(data_stream);
}

/*!
  */
SettingNodeBase* SceneSettingNode::renderingMethodSettingNode() noexcept
{
  ZISC_ASSERT(rendering_method_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(rendering_method_setting_node_->type() == SettingNodeType::kRenderingMethod,
              "The node isn't rendering method setting node.");
  return rendering_method_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::renderingMethodSettingNode() const noexcept
{
  ZISC_ASSERT(rendering_method_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(rendering_method_setting_node_->type() == SettingNodeType::kRenderingMethod,
              "The node isn't rendering method setting node.");
  return rendering_method_setting_node_.get();
}

/*!
  */
const std::string& SceneSettingNode::sceneName() const noexcept
{
  return scene_name_;
}

/*!
  */
void SceneSettingNode::setSceneName(const std::string& scene_name) noexcept
{
  scene_name_ = scene_name;
}

/*!
  */
void SceneSettingNode::setSceneName(std::string&& scene_name) noexcept
{
  scene_name_ = std::move(scene_name);
}

/*!
  */
SettingNodeBase* SceneSettingNode::surfaceModelSettingNode() noexcept
{
  ZISC_ASSERT(surface_model_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(surface_model_setting_node_->type() == SettingNodeType::kSurfaceModel,
              "The node isn't surface model setting node.");
  return surface_model_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::surfaceModelSettingNode() const noexcept
{
  ZISC_ASSERT(surface_model_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(surface_model_setting_node_->type() == SettingNodeType::kSurfaceModel,
              "The node isn't surface model setting node.");
  return surface_model_setting_node_.get();
}

/*!
  */
SettingNodeBase* SceneSettingNode::systemSettingNode() noexcept
{
  ZISC_ASSERT(system_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(system_setting_node_->type() == SettingNodeType::kSystem,
              "The node isn't system setting node.");
  return system_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::systemSettingNode() const noexcept
{
  ZISC_ASSERT(system_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(system_setting_node_->type() == SettingNodeType::kSystem,
              "The node isn't system setting node.");
  return system_setting_node_.get();
}

/*!
  */
SettingNodeBase* SceneSettingNode::textureModelSettingNode() noexcept
{
  ZISC_ASSERT(texture_model_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(texture_model_setting_node_->type() == SettingNodeType::kTextureModel,
              "The node isn't texture model setting node.");
  return texture_model_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::textureModelSettingNode() const noexcept
{
  ZISC_ASSERT(texture_model_setting_node_ != nullptr, "The setting node is null.");
  ZISC_ASSERT(texture_model_setting_node_->type() == SettingNodeType::kTextureModel,
              "The node isn't texture model setting node.");
  return texture_model_setting_node_.get();
}

/*!
  */
SettingNodeType SceneSettingNode::type() const noexcept
{
  return SettingNodeType::kScene;
}

/*!
  */
void SceneSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);

  // Write properties
  writeString(sceneName(), data_stream);

  // Child nodes
  systemSettingNode()->writeData(data_stream);
  renderingMethodSettingNode()->writeData(data_stream);
  textureModelSettingNode()->writeData(data_stream);
  surfaceModelSettingNode()->writeData(data_stream);
  emitterModelSettingNode()->writeData(data_stream);
  cameraSettingNode()->writeData(data_stream);
  objectSettingNode()->writeData(data_stream);
  bvhSettingNode()->writeData(data_stream);
}

} // namespace nanairo
