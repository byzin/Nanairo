/*!
  \file world.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "world.hpp"
// Standard C++ library
#include <chrono>
#include <cstdint>
#include <cstddef>
#include <future>
#include <list>
#include <utility>
#include <vector>
// Qt
#include <QJsonArray>
#include <QJsonObject>
#include <QLocale>
#include <QString>
#include <QtGlobal>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/error.hpp"
#include "zisc/stopwatch.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
#include "zisc/unit.hpp"
// Nanairo
#include "system.hpp"
#include "Data/light_source_reference.hpp"
#include "Data/object.hpp"
#include "DataStructure/bvh.hpp"
#include "Geometry/transformation.hpp"
#include "Material/material.hpp"
#include "Material/EmitterModel/emitter_model.hpp"
#include "Material/SurfaceModel/surface_model.hpp"
#include "Material/TextureModel/texture_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/light_source_sampler.hpp"
#include "Utility/unique_pointer.hpp"
#include "Utility/scene_value.hpp"


namespace nanairo {

/*!
  \details
  No detailed.
  */
World::World(System& system, const QJsonObject& settings) noexcept
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
void World::initialize(System& system, const QJsonObject& settings) noexcept
{
  using zisc::cast;
  using std::chrono::duration_cast;
  using Millis = std::chrono::milliseconds;

  zisc::Stopwatch timer;
  auto get_elapsed_time = [&timer]()
  {
    const auto time = timer.elapsedTime();
    return cast<long int>(duration_cast<Millis>(time).count());
  };

  // Initialize texture
  timer.start();
  {
    const auto texture_settings_list = SceneValue::toArray(settings,
                                                           keyword::textureModel);
    initializeTexture(system, texture_settings_list);
    qInfo("  TextureModel initialization time: %ld ms.", get_elapsed_time());
  }
  timer.stop();

  // Initialize surface scattering
  timer.start();
  {
    const auto surface_settings_list = SceneValue::toArray(settings,
                                                           keyword::surfaceModel);
    initializeSurface(system, surface_settings_list);
    qInfo("  SurfaceModel initialization time: %ld ms.", get_elapsed_time());
  }
  timer.stop();

  // Initialize emitter
  timer.start();
  {
    const auto emitter_settings_list = SceneValue::toArray(settings,
                                                           keyword::emitterModel);
    initializeEmitter(system, emitter_settings_list);
    qInfo("  EmitterModel initialization time: %ld ms.", get_elapsed_time());
  }
  timer.stop();

  // Initialize objects
  timer.start();
  const auto object_settings_list = SceneValue::toArray(settings, keyword::object);
  auto object_list = initializeObject(system, object_settings_list);
  qInfo("  Object initialization time: %ld ms.", get_elapsed_time());
  qInfo("  Num of objects: %ld.", cast<long int>(object_list.size()));
  timer.stop();

  // Initialize a BVH
  timer.start();
  {
    const auto bvh_settings = SceneValue::toObject(settings, keyword::bvh);
    bvh_ = Bvh::makeBvh(bvh_settings);
    bvh_->construct(system, std::move(object_list));
    qInfo("  BVH construction time: %ld ms", get_elapsed_time());
    qInfo("  Num of BVH nodes: %ld", cast<long int>(bvh_->bvhTree().size()));
  }
  timer.stop();

  initializeWorldLightSource();
  light_source_sampler_ = new LightSourceSampler{light_source_list_};
}

/*!
  \details
  No detailed.
  */
void World::initializeEmitter(System& system, const QJsonArray& settings) noexcept
{
  using zisc::cast;

  auto texture_list = textureList();

  const auto count = cast<uint>(settings.count());
  emitter_list_.resize(count);

  auto make_emitter = [this, &system, &settings, &texture_list](const uint index)
  {
    const auto emitter_settings = SceneValue::toObject(settings[index]);
    emitter_list_[index] = EmitterModel::makeEmitter(emitter_settings, texture_list);
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.enqueueLoop(make_emitter, start, count);
  result.get();

  ZISC_ASSERT(0 < emitter_list_.size(), "The scene has no emitter.");
}

/*!
  \details
  No detailed.
  */
std::vector<Object> World::initializeObject(System& system,
                                            const QJsonArray& settings) noexcept
{
  // Make objects
  std::vector<std::vector<Object>> object_list_array;
  {
    auto results = makeObjects(system, settings);
    object_list_array.reserve(results.size());
    for (auto& result : results)
      object_list_array.emplace_back(result.get());
  }
  // Calc the num of objects
  std::size_t num_of_objects = 0;
  for (const auto& objects : object_list_array)
    num_of_objects += objects.size();
  ZISC_ASSERT(0 < num_of_objects, "The scene has no object.");

  // Merge all lists
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
void World::initializeWorldLightSource() noexcept
{
  zisc::CompensatedSummation<Float> total_flux{0.0};
  std::list<const Object*> light_source_list;
  for (const auto& object : bvh().objectList()) {
    if (object.material().isLightSource()) {
      total_flux.add(object.shape().surfaceArea() *
                     object.material().emitter().radiantExitance());
      light_source_list.emplace_back(&object);
    }
  }
  ZISC_ASSERT(0 < light_source_list.size(), "The scene has no light source.");

  light_source_list_.reserve(light_source_list.size());
  for (const auto* light_source : light_source_list)
    light_source_list_.emplace_back(total_flux.get(), light_source);
}

/*!
  \details
  No detailed.
  */
void World::initializeSurface(System& system, const QJsonArray& settings) noexcept
{
  using zisc::cast;

  auto texture_list = textureList();

  const auto count = cast<uint>(settings.count());
  surface_list_.resize(count);

  auto make_surface = [this, &settings, &texture_list](const uint index)
  {
    const auto surface_settings = SceneValue::toObject(settings[index]);
    surface_list_[index] = SurfaceModel::makeSurface(surface_settings, texture_list);
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.enqueueLoop(make_surface, start, count);
  result.get();
  ZISC_ASSERT(0 < surface_list_.size(), "The scene has no surface.");
}

/*!
  \details
  No detailed.
  */
void World::initializeTexture(System& system, const QJsonArray& settings) noexcept
{
  using zisc::cast;

  const auto count = cast<uint>(settings.count());
  texture_list_.resize(count);

  auto make_texture = [this, &system, &settings](const uint index)
  {
    const auto texture_settings = SceneValue::toObject(settings[index]);
    texture_list_[index] = TextureModel::makeTexture(system, texture_settings);
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.enqueueLoop(make_texture, start, count);
  result.get();
  ZISC_ASSERT(0 < texture_list_.size(), "The scene has no texture");
}

/*!
  \details
  No detailed.
  */
std::list<std::future<std::vector<Object>>> World::makeObjects(
    System& system,
    const QJsonArray& settings) const noexcept
{
  std::list<std::future<std::vector<Object>>> results;
  {
    const auto transformation = Transformation::makeIdentity();
    int index = 1;
    makeGroupObject(system, settings, transformation, index, results);
  }
  return results;
}

/*!
  \details
  No detailed.
  */
std::vector<Object> World::makeSingleObject(
    const QJsonObject& settings,
    const Matrix4x4& transformation) const noexcept
{
  // Make geometries
  auto shape_list = Shape::makeShape(settings);

  // Transform geometries
  const auto t_settings = SceneValue::toArray(settings, keyword::transformation);
  const auto t = transformation * Transformation::makeTransformation(t_settings);
  for (auto& shape : shape_list)
    shape->transform(t);

  // Set materials of geometries
  // Set Surface
  const auto surface_index = SceneValue::toInt<uint>(settings,
                                                     keyword::surfaceIndex);
  const auto surface_model = surface_list_[surface_index].get();
  // Set Light
  const EmitterModel* emitter_model = nullptr;
  const bool is_emissive_object = SceneValue::toBool(settings,
                                                     keyword::isEmissiveObject);
  if (is_emissive_object) {
    const auto emitter_index = SceneValue::toInt<uint>(settings,
                                                       keyword::emitterIndex);
    emitter_model = emitter_list_[emitter_index].get();
  }
  const Material material{surface_model, emitter_model};

  // Make objects
  std::vector<Object> object_list;
  object_list.reserve(shape_list.size());
  for (auto& shape : shape_list)
    object_list.emplace_back(material, std::move(shape));
  return object_list;
}

/*!
  \details
  No detailed.
  */
void World::makeGroupObject(
    System& system,
    const QJsonArray& settings,
    const Matrix4x4& transformation,
    int& index,
    std::list<std::future<std::vector<Object>>>& results) const noexcept
{
  using zisc::toHash32;

  // Group settings
  const auto g_settings = SceneValue::toObject(settings[index]);
  const auto g_visibility = SceneValue::toBool(g_settings, keyword::enabled);
  const auto g_level = SceneValue::toInt<uint>(g_settings, keyword::groupLevel);

  // Transformation  settings
  const auto t_settings = SceneValue::toArray(g_settings, keyword::transformation);
  const auto t = transformation * Transformation::makeTransformation(t_settings);

  // Make objects
  for (++index; index < settings.size(); ++index) {
    const auto object_settings = SceneValue::toObject(settings[index]);
    // Object level
    const auto level = SceneValue::toInt<uint>(object_settings, keyword::groupLevel);
    if (level <= g_level) {
      --index;
      break;
    }
    const auto type = SceneValue::toString(object_settings, keyword::type);
    switch (keyword::toHash32(type)) {
     // Single object type
     case toHash32(keyword::singleObject): {
      const auto visibility = SceneValue::toBool(object_settings, keyword::enabled);
      if (g_visibility && visibility) {
        auto make_single_object = [this, object_settings, t]()
        {
          return makeSingleObject(object_settings, t);
        };
        auto& thread_pool = system.threadPool();
        auto result = thread_pool.enqueue<std::vector<Object>>(make_single_object);
        results.emplace_back(std::move(result));
      }
      break;
     }
     // Group type
     case toHash32(keyword::groupObject): {
      makeGroupObject(system, settings, t, index, results);
      break;
     }
     default: {
      zisc::raiseError("ObjectError: Unsupported type is specified.");
      break;
     }
    }
  }
}

} // namespace nanairo
