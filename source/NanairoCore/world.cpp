/*!
  \file world.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
#include <QString>
#include <QLocale>
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
#include "Material/Texture/texture.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/light_source_sampler.hpp"
#include "Utility/unique_pointer.hpp"
#include "Utility/scene_settings.hpp"


namespace nanairo {

/*!
  \details
  No detailed.
  */
World::World(System& system,
             const SceneSettings& settings, 
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
std::vector<const Texture*> World::textureList() const noexcept
{
  std::vector<const Texture*> texture_list;
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
                       const SceneSettings& settings, 
                       const std::function<void (const QString&)>& message_sender) noexcept
{
  using std::chrono::duration_cast;
  using Millis = std::chrono::milliseconds;

  zisc::Stopwatch timer;
 
  auto time = timer.elapsedTime();
  auto message = QString{""};

  // Initialize texture
  timer.start();
  initializeTexture(system, settings);
  time = timer.elapsedTime();
  message = QStringLiteral("    Texture initialization time: %1 ms.");
  message = message.arg(duration_cast<Millis>(time).count());
  message_sender(message);
  timer.stop();

  // Initialize surface scattering
  timer.start();
  initializeSurface(system, settings);
  time = timer.elapsedTime();
  message = QStringLiteral("    SurfaceModel  initialization time: %1 ms.");
  message = message.arg(duration_cast<Millis>(time).count());
  message_sender(message);
  timer.stop();

  // Initialize emitter
  timer.start();
  initializeEmitter(system, settings);
  time = timer.elapsedTime();
  message = QStringLiteral("    EmitterModel initialization time: %1 ms.");
  message = message.arg(duration_cast<Millis>(time).count());
  message_sender(message);
  timer.stop();

  // Print material size
  const auto material_byte = zisc::toMegaByte(getMaterialSize());
  const auto material_size_string = 
      QLocale{QLocale::English}.toString(material_byte, 'f', 3);
  message = QStringLiteral("    Material size: %1 MB.").arg(material_size_string);
  message_sender(message);

  // Initialize objects
  timer.start();
  auto object_list = initializeObject(system, settings);
  time = timer.elapsedTime();
  message = QStringLiteral("    Object initialization time: %1 ms.");
  message = message.arg(duration_cast<Millis>(time).count());
  message_sender(message);
  timer.stop();

  // Print object information
  message = QStringLiteral("    Num of objects: %1.").arg(object_list.size());
  message_sender(message);
  auto object_byte = zisc::toMegaByte(getObjectSize(object_list));
  const auto object_size_string = 
      QLocale{QLocale::English}.toString(object_byte, 'f', 3);
  message = QStringLiteral("    Object size: %1 MB.").arg(object_size_string);
  message_sender(message);

//  // Initialize a BVH
  timer.start();
  bvh_ = makeBvh(settings);
  bvh_->construct(system, std::move(object_list));
  time = timer.elapsedTime();
  message = QStringLiteral("    BVH construction time: %1 ms");
  message = message.arg(duration_cast<Millis>(time).count());
  message_sender(message);

  // Print BVH information
  auto bvh_byte = zisc::toMegaByte(bvh_->getBvhSize());
  const auto bvh_size_string = QLocale{QLocale::English}.toString(bvh_byte, 'f', 3);
  message = QStringLiteral("    BVH size: %1 MB.").arg(bvh_size_string);
  message_sender(message);


  initializeWorldLightSource();
  light_source_sampler_ = new LightSourceSampler{light_source_list_};
}

/*!
  \details
  No detailed.
  */
void World::initializeEmitter(System& system, const SceneSettings& settings) noexcept
{
  using zisc::cast;

  const QString prefix{keyword::emitterModel};
  auto key = prefix + "/" + keyword::count;
  const auto count = cast<uint>(settings.intValue(key));
  emitter_list_.resize(count);

  std::function<void (const uint)> make_emitter{
  [this, &system, &settings, &prefix](const uint index)
  {
    const auto item_id = (prefix + "/%1").arg(index);
    emitter_list_[index] = makeEmitter(system, settings, item_id);
  }};

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.loop(std::move(make_emitter), start, count);
  result.get();

  ZISC_ASSERT(0 < emitter_list_.size(), "The scene has no emitter.");
}

/*!
  \details
  No detailed.
  */
std::vector<Object> World::initializeObject(System& system, 
                                            const SceneSettings& settings) noexcept
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
void World::initializeSurface(System& system, const SceneSettings& settings) noexcept
{
  using zisc::cast;

  auto texture_list = textureList();

  const QString prefix{keyword::surfaceModel};
  auto key = prefix + "/" + keyword::count;
  const auto count = cast<uint>(settings.intValue(key));
  surface_list_.resize(count);

  std::function<void (const uint)> make_surface{
  [this, &settings, &prefix, &texture_list](const uint index)
  {
    const auto item_id = (prefix + "/%1").arg(index);
    surface_list_[index] = makeSurface(settings, item_id, texture_list);
  }};

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.loop(std::move(make_surface), start, count);
  result.get();
  ZISC_ASSERT(0 < surface_list_.size(), "The scene has no surface.");
}

/*!
  \details
  No detailed.
  */
void World::initializeTexture(System& system, const SceneSettings& settings) noexcept
{
  using zisc::cast;

  const QString prefix{keyword::texture};
  auto key = prefix + "/" + keyword::count;
  const auto count = cast<uint>(settings.intValue(key));
  texture_list_.resize(count);

  std::function<void (const uint)> make_texture{
  [this, &system, &settings, &prefix](const uint index)
  {
    const auto item_id = (prefix + "/%1").arg(index);
    texture_list_[index] = makeTexture(system, settings, item_id);
  }};

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.loop(std::move(make_texture), start, count);
  result.get();
  ZISC_ASSERT(0 < texture_list_.size(), "The scene has no texture");
}

/*!
  \details
  No detailed.
  */
std::list<std::future<std::vector<Object>>> World::makeObjects(
    System& system,
    const SceneSettings& settings) const noexcept
{
  using zisc::cast;
  using zisc::toHash32;

  std::list<std::future<std::vector<Object>>> results;

  const QString prefix{keyword::object};
  auto key = prefix + "/" + keyword::count;
  const auto count = cast<uint>(settings.intValue(key));

  auto t = makeIdentityMatrix();

  for (uint index = 1; index < count; ++index) {
    const auto p = (prefix + "/%1").arg(index);
    key = p + "/" + keyword::type;
    const auto type = keyword::toHash32(settings.stringValue(key));
    if (type == toHash32(keyword::singleObject)) {
      const auto visibility = settings.booleanValue(p + "/" + keyword::visibility);
      if (visibility) {
        std::function<std::vector<Object> ()> make_single_object{
        [this, &settings, p, t]()
        {
          return makeSingleObject(settings, p, t);
        }};
        auto& thread_pool = system.threadPool();
        auto result = thread_pool.enqueue(std::move(make_single_object));
        results.emplace_back(std::move(result));
      }
    }
    else {
      auto object_results = makeGroupObject(system, settings, p, t, count, index);
      for (auto& result : object_results)
        results.emplace_back(std::move(result));
    }
  }
  return results;
}

/*!
  \details
  No detailed.
  */
std::vector<Object> World::makeSingleObject(const SceneSettings& settings,
                                            const QString& prefix,
                                            const Matrix4x4& transformation) const noexcept
{
  auto geometry_list = makeGeometry(settings, prefix);
  const auto t = transformation * makeTransformationMatrix(settings, prefix);
  for (auto& geometry : geometry_list)
    geometry->transform(t);

  // Make a material
  // Set BSDF
  auto key = prefix + "/" + keyword::surfaceIndex;
  const auto surface_index = settings.intValue(key);
  const auto surface_model = surface_list_[surface_index].get();
  // Set Light
  const EmitterModel* emitter_model = nullptr;
  key = prefix + "/" + keyword::isEmissiveObject;
  const bool is_emissive_object = settings.booleanValue(key);
  if (is_emissive_object) {
    key = prefix + "/" + keyword::emitterIndex;
    const auto emitter_index = settings.intValue(key);
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
    const SceneSettings& settings,
    const QString& prefix,
    const Matrix4x4& transformation,
    const uint count,
    uint& index) const noexcept
{
  using zisc::toHash32;

  std::list<std::future<std::vector<Object>>> results;

  auto key = prefix + "/" + keyword::visibility;
  const auto group_visibility = settings.booleanValue(key);
  key = prefix + "/" + keyword::treeLevel;
  const auto group_level = settings.intValue(key);

  const auto t = transformation * makeTransformationMatrix(settings, prefix);

  for (++index; index < count; ++index) {
    const auto p = (QString{keyword::object} + "/%1").arg(index);
    key = p + "/" + keyword::treeLevel;
    const auto tree_level = settings.intValue(key);
    if (tree_level <= group_level) {
      --index;
      break;
    }
    key = p + "/" + keyword::type;
    const auto type = keyword::toHash32(settings.stringValue(key));
    if (type == toHash32(keyword::singleObject)) {
      const auto visibility = settings.booleanValue(p + "/" + keyword::visibility);
      if (group_visibility && visibility) {
        std::function<std::vector<Object> ()> make_single_object{
        [this, &settings, p, t]()
        {
          return makeSingleObject(settings, p, t);
        }};
        auto& thread_pool = system.threadPool();
        auto result = thread_pool.enqueue(std::move(make_single_object));
        results.emplace_back(std::move(result));
      }
    }
    else {
      auto object_results = makeGroupObject(system, settings, p, t, count, index);
      for (auto& result : object_results)
        results.emplace_back(std::move(result));
    }
  }
  return results;
}

} // namespace nanairo
