/*!
  \file scene_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
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
SceneSettingNode::SceneSettingNode(const SettingNodeBase* parent) noexcept :
    SettingNodeBase(parent),
    scene_name_{&data_resource_},
    system_setting_node_{
        zisc::UniqueMemoryPointer<SystemSettingNode>::make(&data_resource_,
                                                           this)},
    rendering_method_setting_node_{
        zisc::UniqueMemoryPointer<RenderingMethodSettingNode>::make(&data_resource_,
                                                                    this)},
    texture_model_setting_node_{
        zisc::UniqueMemoryPointer<TextureModelSettingNode>::make(&data_resource_,
                                                                 this)},
    surface_model_setting_node_{
        zisc::UniqueMemoryPointer<SurfaceModelSettingNode>::make(&data_resource_,
                                                                 this)},
    emitter_model_setting_node_{
        zisc::UniqueMemoryPointer<EmitterModelSettingNode>::make(&data_resource_,
                                                                 this)},
    camera_setting_node_{
        zisc::UniqueMemoryPointer<ObjectModelSettingNode>::make(&data_resource_,
                                                                this)},
    object_setting_node_{
        zisc::UniqueMemoryPointer<ObjectModelSettingNode>::make(&data_resource_,
                                                                this)},
    bvh_setting_node_{
        zisc::UniqueMemoryPointer<BvhSettingNode>::make(&data_resource_,
                                                        this)}
{
}

/*!
  */
SceneSettingNode::~SceneSettingNode() noexcept
{
  // Destroy before before allocators are destroyed
  scene_name_ = zisc::pmr::string{&data_resource_};
  scene_name_.shrink_to_fit();
  system_setting_node_.reset();
  rendering_method_setting_node_.reset();
  texture_model_setting_node_.reset();
  surface_model_setting_node_.reset();
  emitter_model_setting_node_.reset();
  camera_setting_node_.reset();
  object_setting_node_.reset();
  bvh_setting_node_.reset();
}

/*!
  */
SettingNodeBase* SceneSettingNode::bvhSettingNode() noexcept
{
  return bvh_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::bvhSettingNode() const noexcept
{
  return bvh_setting_node_.get();
}

/*!
  */
SettingNodeBase* SceneSettingNode::cameraSettingNode() noexcept
{
  return camera_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::cameraSettingNode() const noexcept
{
  return camera_setting_node_.get();
}

/*!
  */
zisc::pmr::memory_resource* SceneSettingNode::dataResource() const noexcept
{
  const zisc::pmr::memory_resource* r = &data_resource_;
  return const_cast<zisc::pmr::memory_resource*>(r);
}

/*!
  */
SettingNodeBase* SceneSettingNode::emitterModelSettingNode() noexcept
{
  return emitter_model_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::emitterModelSettingNode() const noexcept
{
  return emitter_model_setting_node_.get();
}

/*!
  */
void SceneSettingNode::initialize() noexcept
{
  data_resource_.setMutex(&data_mutex_);
  work_resource_.setMutex(&work_mutex_);

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
SettingNodeType SceneSettingNode::nodeType() noexcept
{
  return SettingNodeType::kScene;
}

/*!
  */
SettingNodeBase* SceneSettingNode::objectSettingNode() noexcept
{
  return object_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::objectSettingNode() const noexcept
{
  return object_setting_node_.get();
}

/*!
  */
void SceneSettingNode::readData(std::istream* data_stream) noexcept
{
  SettingNodeType t = readType(data_stream);
  ZISC_ASSERT(type() == t, "The stream header is wrong.");

  // Read properties
  scene_name_ = readString(data_stream);

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
  return rendering_method_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::renderingMethodSettingNode() const noexcept
{
  return rendering_method_setting_node_.get();
}

/*!
  */
std::string_view SceneSettingNode::sceneName() const noexcept
{
  return std::string_view{scene_name_};
}

/*!
  */
void SceneSettingNode::setSceneName(const std::string_view& scene_name) noexcept
{
  scene_name_ = scene_name;
}

/*!
  */
SettingNodeBase* SceneSettingNode::surfaceModelSettingNode() noexcept
{
  return surface_model_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::surfaceModelSettingNode() const noexcept
{
  return surface_model_setting_node_.get();
}

/*!
  */
SettingNodeBase* SceneSettingNode::systemSettingNode() noexcept
{
  return system_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::systemSettingNode() const noexcept
{
  return system_setting_node_.get();
}

/*!
  */
SettingNodeBase* SceneSettingNode::textureModelSettingNode() noexcept
{
  return texture_model_setting_node_.get();
}

/*!
  */
const SettingNodeBase* SceneSettingNode::textureModelSettingNode() const noexcept
{
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
zisc::pmr::memory_resource* SceneSettingNode::workResource() const noexcept
{
  const zisc::pmr::memory_resource* r = &work_resource_;
  return const_cast<zisc::pmr::memory_resource*>(r);
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
