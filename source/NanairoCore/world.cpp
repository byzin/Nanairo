/*!
  \file world.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "world.hpp"
// Standard C++ library
#include <algorithm>
#include <cstdint>
#include <cstddef>
#include <future>
#include <list>
#include <utility>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/error.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
#include "zisc/unit.hpp"
// Nanairo
#include "system.hpp"
#include "Data/object.hpp"
#include "DataStructure/bvh.hpp"
#include "Geometry/transformation.hpp"
#include "Material/material.hpp"
#include "Material/EmitterModel/emitter_model.hpp"
#include "Material/SurfaceModel/surface_model.hpp"
#include "Material/TextureModel/texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Setting/group_object_setting_node.hpp"
#include "Setting/material_setting_node.hpp"
#include "Setting/object_model_setting_node.hpp"
#include "Setting/scene_setting_node.hpp"
#include "Setting/setting_node_base.hpp"
#include "Setting/single_object_setting_node.hpp"
#include "Utility/unique_pointer.hpp"


namespace nanairo {

/*!
  \details
  No detailed.
  */
World::World(System& system, const SettingNodeBase* settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
World::~World() noexcept
{
}

/*!
  \details
  No detailed.
  */
std::vector<const TextureModel*> World::textureList() const noexcept
{
  std::vector<const TextureModel*> texture_list;
  texture_list.reserve(texture_list_.size());
  for (const auto& texture : texture_list_)
    texture_list.emplace_back(texture.get());
  return texture_list;
}

/*!
  \details
  No detailed.
  */
void World::initialize(System& system, const SettingNodeBase* settings) noexcept
{
  const auto scene_settings = castNode<SceneSettingNode>(settings);

  // Initialize texture
  {
    initializeTexture(system, scene_settings->textureModelSettingNode());
  }

  // Initialize surface scattering
  {
    const auto texture_list = textureList();
    initializeSurface(system,
                      scene_settings->surfaceModelSettingNode(),
                      texture_list);
  }

  // Initialize emitter
  {
    const auto texture_list = textureList();
    initializeEmitter(system,
                      scene_settings->emitterModelSettingNode(),
                      texture_list);
  }

  // Initialize objects
  auto object_list = initializeObject(system, scene_settings->objectSettingNode());

  // Initialize a BVH
  {
    bvh_ = Bvh::makeBvh(scene_settings->bvhSettingNode());
    bvh_->construct(system, std::move(object_list));
  }

  {
    initializeWorldLightSource();
  }
}

/*!
  \details
  No detailed.
  */
void World::initializeEmitter(
    System& system,
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto emitter_model_settings = castNode<EmitterModelSettingNode>(settings);

  const auto num_of_emitters = emitter_model_settings->numOfMaterials();
  emitter_list_.resize(num_of_emitters);

  const auto emitter_setting_list = emitter_model_settings->materialList();
  auto make_emitter =
  [this, &emitter_setting_list, &texture_list](const uint index)
  {
    const auto emitter_settings = emitter_setting_list[index];
    emitter_list_[index] = EmitterModel::makeEmitter(emitter_settings, texture_list);
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.enqueueLoop(make_emitter, start, num_of_emitters);
  result.get();

  ZISC_ASSERT(0 < emitter_list_.size(), "The scene has no emitter.");
}

/*!
  \details
  No detailed.
  */
std::vector<Object> World::initializeObject(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  // Make objects
  auto object_list_array = makeObjects(system, settings);
  // Calc the num of objects
  std::size_t num_of_objects = 0;
  for (const auto& objects : object_list_array)
    num_of_objects += objects.size();
  ZISC_ASSERT(0 < num_of_objects, "The scene has no object.");

  // Merge all lists into a list
  std::vector<Object> object_list;
  object_list.reserve(num_of_objects);
  for (auto& objects : object_list_array) {
    for (auto& object : objects) {
      object_list.emplace_back(std::move(object));
    }
  }
  return object_list;
}

/*!
  \details
  No detailed.
  */
void World::initializeSurface(
    System& system,
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_model_settings = castNode<SurfaceModelSettingNode>(settings);

  const auto num_of_surfaces = surface_model_settings->numOfMaterials();
  surface_list_.resize(num_of_surfaces);

  const auto surface_setting_list = surface_model_settings->materialList();
  auto make_surface =
  [this, &surface_setting_list, &texture_list](const uint index)
  {
    const auto surface_settings = surface_setting_list[index];
    surface_list_[index] = SurfaceModel::makeSurface(surface_settings, texture_list);
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.enqueueLoop(make_surface, start, num_of_surfaces);
  result.get();
  ZISC_ASSERT(0 < surface_list_.size(), "The scene has no surface.");
}

/*!
  \details
  No detailed.
  */
void World::initializeTexture(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto texture_model_settings = castNode<TextureModelSettingNode>(settings);

  const auto num_of_textures = texture_model_settings->numOfMaterials();
  texture_list_.resize(num_of_textures);

  const auto texture_setting_list = texture_model_settings->materialList();
  auto make_texture =
  [this, &system, &texture_setting_list](const uint index)
  {
    const auto texture_settings = texture_setting_list[index];
    texture_list_[index] = TextureModel::makeTexture(system, texture_settings);
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.enqueueLoop(make_texture, start, num_of_textures);
  result.get();
  ZISC_ASSERT(0 < texture_list_.size(), "The scene has no texture");
}

/*!
  \details
  No detailed.
  */
void World::initializeWorldLightSource() noexcept
{
  std::list<const Object*> light_source_list;
  for (const auto& object : bvh().objectList()) {
    if (object.material().isLightSource())
      light_source_list.emplace_back(&object);
  }
  light_source_list_.resize(light_source_list.size());
  std::copy(light_source_list.begin(), light_source_list.end(),
            light_source_list_.begin());
}

/*!
  \details
  No detailed.
  */
std::vector<std::vector<Object>> World::makeObjects(
    System& system,
    const SettingNodeBase* settings) const noexcept
{
  std::list<std::future<std::vector<Object>>> results;
  {
    const auto transformation = Transformation::makeIdentity();
    makeObjects(system, settings, transformation, results);
  }
  std::vector<std::vector<Object>> object_list;
  {
    object_list.reserve(results.size());
    for (auto& result : results)
      object_list.emplace_back(result.get());
  }
  return object_list;
}

/*!
  */
void World::makeObjects(
    System& system,
    const SettingNodeBase* settings,
    Matrix4x4 transformation,
    std::list<std::future<std::vector<Object>>>& results) const noexcept
{
  const auto object_model_settings = castNode<ObjectModelSettingNode>(settings);
  if (object_model_settings->visibility()) {
    // Transformation
    const auto transformation_list = object_model_settings->transformationList();
    if (0 < transformation_list.size()) {
      transformation = transformation *
                       Transformation::makeTransformation(transformation_list);
    }
    // Object
    const auto object_settings = object_model_settings->objectSettingNode();
    if (object_settings->type() == SettingNodeType::kGroupObject)
      makeGroupObject(system, object_settings, transformation, results);
    else
      makeSingleObject(system, object_settings, transformation, results);
  }
}

/*!
  \details
  No detailed.
  */
void World::makeSingleObject(
    System& system,
    const SettingNodeBase* settings,
    const Matrix4x4& transformation,
    std::list<std::future<std::vector<Object>>>& results) const noexcept
{
  auto make_object = [this, settings, transformation]()
  {
    const auto object_settings = castNode<SingleObjectSettingNode>(settings);
    // Make geometries
    auto shape_list = Shape::makeShape(object_settings);
    // Transform geometries
    for (auto& shape : shape_list)
      shape->transform(transformation);
    // Set materials of geometries
    // Set Surface
    const auto surface_index = object_settings->surfaceIndex();
    const SurfaceModel* surface_model = surface_list_[surface_index].get();
    // Set Emitter
    const EmitterModel* emitter_model = nullptr;
    if (object_settings->isEmissiveObject()) {
      const auto emitter_index = object_settings->emitterIndex();
      emitter_model = emitter_list_[emitter_index].get();
    }
    const Material material{surface_model, emitter_model};
    // Make objects
    std::vector<Object> object_list;
    object_list.reserve(shape_list.size());
    for (auto& shape : shape_list)
      object_list.emplace_back(material, std::move(shape));
    return object_list;
  };

  auto& thread_pool = system.threadPool();
  auto result = thread_pool.enqueue<std::vector<Object>>(make_object);
  results.emplace_back(std::move(result));
}

/*!
  \details
  No detailed.
  */
void World::makeGroupObject(
    System& system,
    const SettingNodeBase* settings,
    const Matrix4x4& transformation,
    std::list<std::future<std::vector<Object>>>& results) const noexcept
{
  const auto group_settings = castNode<GroupObjectSettingNode>(settings);

  const auto& object_list = group_settings->objectList();
  for (const auto object_settings : object_list)
    makeObjects(system, object_settings, transformation, results);
}

} // namespace nanairo
