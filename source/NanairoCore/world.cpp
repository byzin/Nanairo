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
#include <functional>
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
#include "LinearAlgebra/transformation.hpp"
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
World::World(System& system,
             const QJsonObject& settings, 
             const std::function<void (const QString&)>& message_sender) noexcept
{
  initialize(system, settings, message_sender);
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
//std::vector<const EmitterModel*> World::emitterList() const
//{
//  std::vector<const EmitterModel*> emitter_list;
//  emitter_list.reserve(emitter_list_.size());
//  for (const auto& emitter_model : emitter_list_)
//    emitter_list.emplace_back(emitter_model.get());
//  return emitter_list;
//}

/*!
  \details
  No detailed.
  */
//std::vector<const SurfaceModel*> World::surfaceList() const
//{
//  std::vector<const SurfaceModel*> surface_list;
//  surface_list.reserve(surface_list_.size());
//  for (const auto& surface_model : surface_list_)
//    surface_list.emplace_back(surface_model.get());
//  return surface_list;
//}

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
std::size_t World::getMaterialSize() const noexcept
{
  std::size_t size = 0;
  for (const auto& texture : texture_list_)
    size += texture->textureSize();
  for (const auto& surface : surface_list_)
    size += surface->surfaceSize();
  for (const auto& emitter : emitter_list_)
    size += emitter->emitterSize();
  return size;
}

/*!
  \details
  No detailed.
  */
std::size_t World::getObjectSize(const std::vector<Object>& object_list) const noexcept
{
  std::size_t object_size = 0;
  for (const auto& object : object_list)
    object_size += object.geometry().geometrySize();
  return object_size;
}

/*!
  \details
  No detailed.
  */
void World::initialize(System& system,
                       const QJsonObject& settings, 
                       const std::function<void (const QString&)>& /* message_sender */) noexcept
{
  using zisc::cast;
  using std::chrono::duration_cast;
  using Millis = std::chrono::milliseconds;

  zisc::Stopwatch timer;
 
  // Initialize texture
  timer.start();
  {
    const auto texture_settings_list = SceneValue::toArray(settings,
                                                           keyword::textureModel);
    initializeTexture(system, texture_settings_list);
    const auto time = timer.elapsedTime();
    const auto time_count = cast<long int>(duration_cast<Millis>(time).count());
    qInfo("    Texture initialization time: %ld ms.", time_count);
  }
  timer.stop();

  // Initialize surface scattering
  timer.start();
  {
    const auto surface_settings_list = SceneValue::toArray(settings,
                                                           keyword::surfaceModel);
    initializeSurface(system, surface_settings_list);
    const auto time = timer.elapsedTime();
    const auto time_count = cast<long int>(duration_cast<Millis>(time).count());
    qInfo("    SurfaceModel  initialization time: %ld ms.", time_count);
  }
  timer.stop();

  // Initialize emitter
  timer.start();
  {
    const auto emitter_settings_list = SceneValue::toArray(settings,
                                                           keyword::emitterModel);
    initializeEmitter(system, emitter_settings_list);
    const auto time = timer.elapsedTime();
    const auto time_count = cast<long int>(duration_cast<Millis>(time).count());
    qInfo("    EmitterModel initialization time: %ld ms.", time_count);
  }
  timer.stop();

  // Print material size
  {
    const auto material_byte = zisc::toMegaByte(getMaterialSize());
    const auto material_size_string = 
        QLocale{QLocale::English}.toString(material_byte, 'f', 3);
    qInfo("    Material size: %s MB.", material_size_string.toStdString().c_str());
  }

  // Initialize objects
  timer.start();
  const auto object_settings_list = SceneValue::toArray(settings, keyword::object);
  auto object_list = initializeObject(system, object_settings_list);
  {
    const auto time = timer.elapsedTime();
    const auto time_count = cast<long int>(duration_cast<Millis>(time).count());
    qInfo("    Object initialization time: %ld ms.", time_count);
  }
  timer.stop();

  // Print object information
  {
    qInfo("    Num of objects: %ld.", cast<long int>(object_list.size()));
    auto object_byte = zisc::toMegaByte(getObjectSize(object_list));
    const auto object_size_string = 
        QLocale{QLocale::English}.toString(object_byte, 'f', 3);
    qInfo("    Object size: %s MB.", object_size_string.toStdString().c_str());
  }

  // Initialize a BVH
  timer.start();
  {
    const auto bvh_settings = SceneValue::toObject(settings, keyword::bvh);
    bvh_ = makeBvh(bvh_settings);
    bvh_->construct(system, std::move(object_list));
    const auto time = timer.elapsedTime();
    const auto time_count = cast<long int>(duration_cast<Millis>(time).count());
    qInfo("    BVH construction time: %ld ms", time_count);
  }
  timer.stop();

  // Print BVH information
  {
    auto bvh_byte = zisc::toMegaByte(bvh_->getBvhSize());
    const auto bvh_size_string = QLocale{QLocale::English}.toString(bvh_byte, 'f', 3);
    qInfo("    BVH size: %s MB.", bvh_size_string.toStdString().c_str());
  }

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
    emitter_list_[index] = makeEmitter(emitter_settings, texture_list);
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
  auto results = makeObjects(system, settings);

  std::list<std::vector<Object>> object_array_list;
  for (auto& result : results)
    object_array_list.emplace_back(result.get());

  std::size_t num_of_objects = 0;
  for (const auto& objects : object_array_list)
    num_of_objects += objects.size();
  ZISC_ASSERT(0 < num_of_objects, "The scene has no object.");

  std::vector<Object> object_list;
  object_list.reserve(num_of_objects);
  for (auto& objects : object_array_list) {
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
      total_flux.add(object.geometry().surfaceArea() *
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
    surface_list_[index] = makeSurface(surface_settings, texture_list);
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
  using zisc::cast;
  using zisc::toHash32;

  std::list<std::future<std::vector<Object>>> results;

  const auto count = cast<uint>(settings.count());
  auto t = makeIdentityMatrix();
  for (uint index = 1; index < count; ++index) {
    const auto object_settings = SceneValue::toObject(settings[index]);
    const auto type = SceneValue::toString(object_settings, keyword::type);
    switch (keyword::toHash32(type)) {
      case toHash32(keyword::singleObject): {
        const auto visibility = SceneValue::toBool(object_settings,
                                                   keyword::enabled);
        if (visibility) {
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
      case toHash32(keyword::groupObject): {
        auto result_list =
            makeGroupObject(system, settings, object_settings, t, count, index);
        for (auto& result : result_list)
          results.emplace_back(std::move(result));
        break;
      }
      default:
        ZISC_ASSERT(false, "ObjectError: Unsupported type is specified.");
    }
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
  auto geometry_list = makeGeometry(settings);

  // Transformation
  const auto transformation_settings = SceneValue::toArray(settings,
                                                           keyword::transformation);
  const auto t = transformation * makeTransformationMatrix(transformation_settings);
  for (auto& geometry : geometry_list)
    geometry->transform(t);

  // Make a material
  // Set BSDF
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
  object_list.reserve(geometry_list.size());
  for (auto& geometry : geometry_list)
    object_list.emplace_back(material, std::move(geometry));
  return object_list;
}

/*!
  \details
  No detailed.
  */
std::list<std::future<std::vector<Object>>> World::makeGroupObject(
    System& system,
    const QJsonArray& settings,
    const QJsonObject& group_settings,
    const Matrix4x4& transformation,
    const uint count,
    uint& index) const noexcept
{
  using zisc::toHash32;

  std::list<std::future<std::vector<Object>>> results;

  const auto group_visibility = SceneValue::toBool(group_settings,
                                                   keyword::enabled);
  const auto group_level = SceneValue::toInt<uint>(group_settings,
                                                   keyword::groupLevel);

  const auto transformation_settings = SceneValue::toArray(group_settings,
                                                  keyword::transformation);
  const auto t = transformation * makeTransformationMatrix(transformation_settings);

  for (++index; index < count; ++index) {
    const auto object_settings = SceneValue::toObject(settings[index]);
    const auto level = SceneValue::toInt<uint>(object_settings,
                                               keyword::groupLevel);
    if (level <= group_level) {
      --index;
      break;
    }
    const auto type = SceneValue::toString(object_settings, keyword::type);
    switch (keyword::toHash32(type)) {
      case toHash32(keyword::singleObject): {
        const auto visibility = SceneValue::toBool(object_settings,
                                                   keyword::enabled);
        if (group_visibility && visibility) {
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
      case toHash32(keyword::groupObject): {
        auto object_results =
            makeGroupObject(system, settings, object_settings, t, count, index);
        for (auto& result : object_results)
          results.emplace_back(std::move(result));
        break;
      }
      default:
        ZISC_ASSERT(false, "ObjectError: Unsupported type is specified.");
    }
  }
  return results;
}

} // namespace nanairo
