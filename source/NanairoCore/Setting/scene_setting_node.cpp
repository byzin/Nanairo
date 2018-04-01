/*!
  \file scene_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <memory>
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
    system_setting_node_{std::make_unique<SystemSettingNode>()},
    rendering_method_setting_node_{std::make_unique<RenderingMethodSettingNode>()},
    texture_model_setting_node_{std::make_unique<TextureModelSettingNode>()},
    surface_model_setting_node_{std::make_unique<SurfaceModelSettingNode>()},
    emitter_model_setting_node_{std::make_unique<EmitterModelSettingNode>()},
    camera_setting_node_{std::make_unique<ObjectModelSettingNode>()},
    object_setting_node_{std::make_unique<ObjectModelSettingNode>()},
    bvh_setting_node_{std::make_unique<BvhSettingNode>()}
{
}

/*!
  */
SettingNodeBase* SceneSettingNode::bvhSettingNode() noexcept
{
  return castNode<BvhSettingNode>(bvh_setting_node_.get());
}

/*!
  */
const SettingNodeBase* SceneSettingNode::bvhSettingNode() const noexcept
{
  return castNode<BvhSettingNode>(bvh_setting_node_.get());
}

/*!
  */
SettingNodeBase* SceneSettingNode::cameraSettingNode() noexcept
{
  return castNode<ObjectModelSettingNode>(camera_setting_node_.get());
}

/*!
  */
const SettingNodeBase* SceneSettingNode::cameraSettingNode() const noexcept
{
  return castNode<ObjectModelSettingNode>(camera_setting_node_.get());
}

/*!
  */
SettingNodeBase* SceneSettingNode::emitterModelSettingNode() noexcept
{
  return castNode<EmitterModelSettingNode>(emitter_model_setting_node_.get());
}

/*!
  */
const SettingNodeBase* SceneSettingNode::emitterModelSettingNode() const noexcept
{
  return castNode<EmitterModelSettingNode>(emitter_model_setting_node_.get());
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
SettingNodeBase* SceneSettingNode::objectSettingNode() noexcept
{
  return castNode<ObjectModelSettingNode>(object_setting_node_.get());
}

/*!
  */
const SettingNodeBase* SceneSettingNode::objectSettingNode() const noexcept
{
  return castNode<ObjectModelSettingNode>(object_setting_node_.get());
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
  return castNode<RenderingMethodSettingNode>(rendering_method_setting_node_.get());
}

/*!
  */
const SettingNodeBase* SceneSettingNode::renderingMethodSettingNode() const noexcept
{
  return castNode<RenderingMethodSettingNode>(rendering_method_setting_node_.get());
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
  return castNode<SurfaceModelSettingNode>(surface_model_setting_node_.get());
}

/*!
  */
const SettingNodeBase* SceneSettingNode::surfaceModelSettingNode() const noexcept
{
  return castNode<SurfaceModelSettingNode>(surface_model_setting_node_.get());
}

/*!
  */
SettingNodeBase* SceneSettingNode::systemSettingNode() noexcept
{
  return castNode<SystemSettingNode>(system_setting_node_.get());
}

/*!
  */
const SettingNodeBase* SceneSettingNode::systemSettingNode() const noexcept
{
  return castNode<SystemSettingNode>(system_setting_node_.get());
}

/*!
  */
SettingNodeBase* SceneSettingNode::textureModelSettingNode() noexcept
{
  return castNode<TextureModelSettingNode>(texture_model_setting_node_.get());
}

/*!
  */
const SettingNodeBase* SceneSettingNode::textureModelSettingNode() const noexcept
{
  return castNode<TextureModelSettingNode>(texture_model_setting_node_.get());
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
