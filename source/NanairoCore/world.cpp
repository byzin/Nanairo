/*!
  \file world.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
#include <memory>
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
void World::initialize(System& system, const SettingNodeBase* settings) noexcept
{
  const auto scene_settings = castNode<SceneSettingNode>(settings);

  // Initialize texture
  {
    initializeTexture(system, scene_settings->textureModelSettingNode());
  }

  // Initialize surface scattering
  {
    initializeSurface(system, scene_settings->surfaceModelSettingNode());
  }

  // Initialize emitter
  {
    initializeEmitter(system, scene_settings->emitterModelSettingNode());
  }

  {
    // Initialize objects
    auto object_list = initializeObject(system, scene_settings->objectSettingNode());
    // Initialize a BVH
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
void World::initializeEmitter(System& system,
                              const SettingNodeBase* settings) noexcept
{
  const auto emitter_model_settings = castNode<EmitterModelSettingNode>(settings);

  const auto num_of_emitters = emitter_model_settings->numOfMaterials();
  emitter_list_.resize(num_of_emitters);

  const auto& emitter_setting_list = emitter_model_settings->materialList();
  auto make_emitter = [this, &emitter_setting_list](const uint index)
  {
    const auto emitter_settings = emitter_setting_list[index];
    const auto& texture_list = textureList();
    emitter_list_[index] = EmitterModel::makeEmitter(emitter_settings,  
                                                     texture_list);
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
std::vector<Object> World::initializeObject(System& system,
                                            const SettingNodeBase* settings) noexcept
{
  auto results = makeObjects(system, settings);

  // Initialize materials
  {
    const std::size_t num_of_materials = results.size();
    material_list_.reserve(num_of_materials);
    for (auto& result : results) {
      auto& material = std::get<1>(result);
      material_list_.emplace_back(std::move(material));
    }
  }

  // Initialize objects
  std::size_t num_of_objects = 0;
  for (const auto& result : results) {
    const auto& objects = std::get<0>(result);
    num_of_objects += objects.size();
  }
  ZISC_ASSERT(0 < num_of_objects, "The scene has no object.");

  // Merge all lists into a list
  std::vector<Object> object_list;
  object_list.reserve(num_of_objects);
  for (auto& result : results) {
    auto& objects = std::get<0>(result);
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
void World::initializeSurface(System& system,
                              const SettingNodeBase* settings) noexcept
{
  const auto surface_model_settings = castNode<SurfaceModelSettingNode>(settings);

  const auto num_of_surfaces = surface_model_settings->numOfMaterials();
  surface_list_.resize(num_of_surfaces);

  const auto& surface_setting_list = surface_model_settings->materialList();
  auto make_surface = [this, &surface_setting_list](const uint index)
  {
    const auto surface_settings = surface_setting_list[index];
    const auto& texture_list = textureList();
    surface_list_[index] = SurfaceModel::makeSurface(surface_settings,
                                                     texture_list);
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
void World::initializeTexture(System& system,
                              const SettingNodeBase* settings) noexcept
{
  const auto texture_model_settings = castNode<TextureModelSettingNode>(settings);

  const auto num_of_textures = texture_model_settings->numOfMaterials();
  texture_list_.resize(num_of_textures);

  const auto& texture_setting_list = texture_model_settings->materialList();
  auto make_texture = [this, &system, &texture_setting_list](const uint index)
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
  light_source_list_.clear();
  for (const auto& object : objectList()) {
    if (object.material().isLightSource())
      light_source_list_.emplace_back(&object);
  }
  light_source_list_.shrink_to_fit();
  std::sort(light_source_list_.begin(), light_source_list_.end());
}

/*!
  \details
  No detailed.
  */
auto World::makeObjects(System& system,
                        const SettingNodeBase* settings) const noexcept
    -> std::vector<ObjectSet>
{
  std::list<std::future<ObjectSet>> results;
  {
    const auto transformation = Transformation::makeIdentity();
    std::string object_name = "";
    makeObjects(system, settings, object_name, transformation, results);
  }
  std::vector<ObjectSet> object_list;
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
    const std::string& name,
    Matrix4x4 transformation,
    std::list<std::future<ObjectSet>>& results) const noexcept
{
  const auto object_model_settings = castNode<ObjectModelSettingNode>(settings);
  if (object_model_settings->visibility()) {
    // Transformation
    const auto transformation_list = object_model_settings->transformationList();
    if (0 < transformation_list.size()) {
      transformation = transformation *
                       Transformation::makeTransformation(transformation_list);
    }
    auto object_name = (name.empty())
        ? object_model_settings->name()
        : name + "/" + object_model_settings->name();
    // Object
    const auto object_settings = object_model_settings->objectSettingNode();
    if (object_settings->type() == SettingNodeType::kGroupObject)
      makeGroupObject(system, object_settings, object_name, transformation, results);
    else
      makeSingleObject(system, object_settings, std::move(object_name), transformation, results);
  }
}

/*!
  \details
  No detailed.
  */
void World::makeSingleObject(
    System& system,
    const SettingNodeBase* settings,
    std::string&& name,
    const Matrix4x4& transformation,
    std::list<std::future<ObjectSet>>& results) const noexcept
{
  auto make_object = [this, settings, name = std::move(name), transformation]()
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
    // Make material
    auto material = std::make_unique<Material>(surface_model, emitter_model);
    // Make objects
    std::vector<Object> object_list;
    object_list.reserve(shape_list.size());
    for (auto& shape : shape_list) {
      object_list.emplace_back(std::move(shape), material.get());
      object_list.back().setName(name);
    }
    return std::make_tuple(std::move(object_list), std::move(material));
  };

  auto& thread_pool = system.threadPool();
  auto result = thread_pool.enqueue<ObjectSet>(make_object);
  results.emplace_back(std::move(result));
}

/*!
  \details
  No detailed.
  */
void World::makeGroupObject(
    System& system,
    const SettingNodeBase* settings,
    const std::string& name,
    const Matrix4x4& transformation,
    std::list<std::future<ObjectSet>>& results) const noexcept
{
  const auto group_settings = castNode<GroupObjectSettingNode>(settings);

  const auto& object_list = group_settings->objectList();
  for (const auto object_settings : object_list)
    makeObjects(system, object_settings, name, transformation, results);
}

} // namespace nanairo
