/*!
  \file world.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include <mutex>
#include <utility>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/error.hpp"
#include "zisc/thread_manager.hpp"
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
World::World(System& system, const SettingNodeBase* settings) noexcept :
    emitter_list_{&system.dataMemoryManager()},
    surface_list_{&system.dataMemoryManager()},
    texture_list_{&system.dataMemoryManager()},
    material_list_{&system.dataMemoryManager()},
    light_source_list_{&system.dataMemoryManager()},
    emitter_body_list_{&system.dataMemoryManager()},
    surface_body_list_{&system.dataMemoryManager()},
    texture_body_list_{&system.dataMemoryManager()},
    material_body_list_{&system.dataMemoryManager()}
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

  auto work_resource = static_cast<System::MemoryManager*>(settings->workResource());
  std::mutex work_mutex;
  work_resource->setMutex(&work_mutex);
  work_resource->reset();

  // Initialize texture
  {
    initializeTexture(system, scene_settings->textureModelSettingNode());
    work_resource->reset();
  }

  // Initialize surface scattering
  {
    initializeSurface(system, scene_settings->surfaceModelSettingNode());
    work_resource->reset();
  }

  // Initialize emitter
  {
    initializeEmitter(system, scene_settings->emitterModelSettingNode());
    work_resource->reset();
  }

  {
    // Initialize objects
    auto object_list = initializeObject(system, scene_settings->objectSettingNode());
    work_resource->reset();

    // Initialize a BVH
    auto bvh_settings = scene_settings->bvhSettingNode();
    bvh_ = Bvh::makeBvh(system, bvh_settings);
    bvh_->construct(system, bvh_settings, std::move(object_list));
    work_resource->reset();
  }

  {
    initializeWorldLightSource();
  }

  work_resource->setMutex(nullptr);
}

/*!
  \details
  No detailed.
  */
void World::initializeEmitter(System& system,
                              const SettingNodeBase* settings) noexcept
{
  auto work_resource = static_cast<System::MemoryManager*>(settings->workResource());
  const auto emitter_model_settings = castNode<EmitterModelSettingNode>(settings);

  const auto num_of_emitters = emitter_model_settings->numOfMaterials();
  emitter_list_.resize(num_of_emitters);
  emitter_body_list_.resize(num_of_emitters);

  const auto& emitter_setting_list = emitter_model_settings->materialList();
  auto make_emitter = [this, &system, &emitter_setting_list](const uint index)
  {
    const auto emitter_settings = emitter_setting_list[index];
    const auto& texture_list = textureList();
    emitter_body_list_[index] = EmitterModel::makeEmitter(system,
                                                          emitter_settings,  
                                                          texture_list);
    emitter_list_[index] = emitter_body_list_[index].get();
  };

  {
    auto& threads = system.threadManager();
    constexpr uint start = 0;
    auto result = threads.enqueueLoop(make_emitter,
                                      start,
                                      num_of_emitters,
                                      work_resource);
    result->wait();
  }
  ZISC_ASSERT(0 < emitter_list_.size(), "The scene has no emitter.");
}

/*!
  \details
  No detailed.
  */
zisc::pmr::vector<Object> World::initializeObject(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  auto results = makeObjects(system, settings);

  // Initialize materials
  {
    const std::size_t num_of_materials = results.size();
    material_list_.reserve(num_of_materials);
    material_body_list_.reserve(num_of_materials);
    for (auto& result : results) {
      auto& material = std::get<1>(result);
      material_body_list_.emplace_back(std::move(material));
      material_list_.emplace_back(material_body_list_.back().get());
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
  auto data_resource = settings->dataResource();
  zisc::pmr::vector<Object> object_list{data_resource};
  object_list.reserve(num_of_objects);
  for (auto& result : results) {
    auto& objects = std::get<0>(result);
    for (auto& object : objects)
      object_list.emplace_back(std::move(object));
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
  auto work_resource = static_cast<System::MemoryManager*>(settings->workResource());
  const auto surface_model_settings = castNode<SurfaceModelSettingNode>(settings);

  const auto num_of_surfaces = surface_model_settings->numOfMaterials();
  surface_list_.resize(num_of_surfaces);
  surface_body_list_.resize(num_of_surfaces);

  const auto& surface_setting_list = surface_model_settings->materialList();
  auto make_surface = [this, &system, &surface_setting_list](const uint index)
  {
    const auto surface_settings = surface_setting_list[index];
    const auto& texture_list = textureList();
    surface_body_list_[index] = SurfaceModel::makeSurface(system,
                                                          surface_settings,
                                                          texture_list);
    surface_list_[index] = surface_body_list_[index].get();
  };

  {
    auto& threads = system.threadManager();
    constexpr uint start = 0;
    auto result = threads.enqueueLoop(make_surface,
                                      start,
                                      num_of_surfaces,
                                      work_resource);
    result->wait();
  }
  ZISC_ASSERT(0 < surface_list_.size(), "The scene has no surface.");
}

/*!
  \details
  No detailed.
  */
void World::initializeTexture(System& system,
                              const SettingNodeBase* settings) noexcept
{
  auto work_resource = static_cast<System::MemoryManager*>(settings->workResource());
  const auto texture_model_settings = castNode<TextureModelSettingNode>(settings);

  const auto num_of_textures = texture_model_settings->numOfMaterials();
  texture_list_.resize(num_of_textures);
  texture_body_list_.resize(num_of_textures);

  const auto& texture_setting_list = texture_model_settings->materialList();
  auto make_texture =
  [this, &system, &texture_setting_list](const uint index)
  {
    const auto texture_settings = texture_setting_list[index];
    texture_body_list_[index] = TextureModel::makeTexture(system, texture_settings);
    texture_list_[index] = texture_body_list_[index].get();
  };

  {
    auto& threads = system.threadManager();
    constexpr uint start = 0;
    auto result = threads.enqueueLoop(make_texture,
                                      start,
                                      num_of_textures,
                                      work_resource);
    result->wait();
  }
  ZISC_ASSERT(0 < texture_list_.size(), "The scene has no texture");
}

/*!
  \details
  No detailed.
  */
void World::initializeWorldLightSource() noexcept
{
  light_source_list_.clear();
  std::size_t num_of_lights = 0;
  for (const auto& object : objectList()) {
    if (object.material().isLightSource())
      ++num_of_lights;
  }
  light_source_list_.reserve(num_of_lights);
  for (const auto& object : objectList()) {
    if (object.material().isLightSource())
      light_source_list_.emplace_back(&object);
  }
  std::sort(light_source_list_.begin(), light_source_list_.end());
}

/*!
  \details
  No detailed.
  */
auto World::makeObjects(System& system,
                        const SettingNodeBase* settings) const noexcept
    -> zisc::pmr::vector<ObjectSet>
{
  auto work_resource = settings->workResource();
  zisc::pmr::list<ObjectSetResult> results{work_resource};
  {
    const auto transformation = Transformation::makeIdentity();
    makeObjects(system, settings, transformation, results);
  }
  zisc::pmr::vector<ObjectSet> object_list{work_resource};
  {
    object_list.reserve(results.size());
    for (auto& result : results)
      object_list.emplace_back(result->get());
  }
  return object_list;
}

/*!
  */
void World::makeObjects(
    System& system,
    const SettingNodeBase* settings,
    Matrix4x4 transformation,
    zisc::pmr::list<ObjectSetResult>& results) const noexcept
{
  const auto object_model_settings = castNode<ObjectModelSettingNode>(settings);
  if (object_model_settings->visibility()) {
    // Transformation
    const auto& transformation_list = object_model_settings->transformationList();
    if (0 < transformation_list.size()) {
      transformation = transformation *
                       Transformation::makeTransformation(transformation_list);
    }
    // Object
    const auto object_settings = object_model_settings->objectSettingNode();
    if (object_settings->type() == SettingNodeType::kGroupObject)
      makeGroupObject(system, settings, transformation, results);
    else
      makeSingleObject(system, settings, transformation, results);
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
    zisc::pmr::list<ObjectSetResult>& results) const noexcept
{
  auto work_resource = settings->workResource();
  auto make_object =
  [this, &system, settings, transformation, work_resource]()
  {
    const auto model_settings = castNode<ObjectModelSettingNode>(settings);
    const auto object_settings =
        castNode<SingleObjectSettingNode>(model_settings->objectSettingNode());
    // Make geometries
    auto shape_list = Shape::makeShape(system, object_settings);
    // Transform geometries
    for (auto& shape : shape_list)
      shape->transform(transformation);
    // Set materials of geometries
    // Set Surface
    const auto surface_index = object_settings->surfaceIndex();
    const SurfaceModel* surface_model = surface_list_[surface_index];
    // Set Emitter
    const EmitterModel* emitter_model = nullptr;
    if (object_settings->isEmissiveObject()) {
      const auto emitter_index = object_settings->emitterIndex();
      emitter_model = emitter_list_[emitter_index];
    }
    // Make material
    auto material = zisc::UniqueMemoryPointer<Material>::make(
        &system.dataMemoryManager(),
        surface_model,
        emitter_model);
    // Make objects
    zisc::pmr::vector<Object> object_list{work_resource};
    object_list.reserve(shape_list.size());
    for (auto& shape : shape_list) {
      object_list.emplace_back(std::move(shape), material.get());
      object_list.back().setName(model_settings->name());
    }
    return std::make_tuple(std::move(object_list), std::move(material));
  };

  {
    auto& threads = system.threadManager();
    auto result = threads.enqueue<ObjectSet>(make_object, work_resource);
    results.emplace_back(std::move(result));
  }
}

/*!
  \details
  No detailed.
  */
void World::makeGroupObject(
    System& system,
    const SettingNodeBase* settings,
    const Matrix4x4& transformation,
    zisc::pmr::list<ObjectSetResult>& results) const noexcept
{
  const auto model_settings = castNode<ObjectModelSettingNode>(settings);
  const auto group_settings =
      castNode<GroupObjectSettingNode>(model_settings->objectSettingNode());

  const auto& object_list = group_settings->objectList();
  for (const auto object_settings : object_list)
    makeObjects(system, object_settings, transformation, results);
}

} // namespace nanairo
