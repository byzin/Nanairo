/*!
  \file world.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_WORLD_HPP
#define NANAIRO_WORLD_HPP

// Standard C++ library
#include <cstddef>
#include <functional>
#include <future>
#include <list>
#include <vector>
// Nanairo
#include "Data/light_source_reference.hpp"
#include "Data/object.hpp"
#include "Material/EmitterModel/emitter_model.hpp"
#include "Material/SurfaceModel/surface_model.hpp"
#include "Material/Texture/texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/light_source_sampler.hpp"
#include "Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class Bvh;
class System;

/*!
 \brief Rendering world
 \details
 No detailed.
 */
class World
{
 public:
  //! Initialize the world
  World(System& system, 
        const QJsonObject& settings,
        const std::function<void (const QString&)>& message_sender) noexcept;

  //! Finalize the world
  ~World() noexcept;


  //! Return the bvh
  const Bvh& bvh() const noexcept;

  //! Make emitter model list
  std::vector<const EmitterModel*> emitterList() const noexcept;

  //! Return the light source list
  const std::vector<LightSourceReference>& lightSourceList() const noexcept;

  //! Return the world light source sampler
  const LightSourceSampler& lightSourceSampler() const noexcept;

  //! Make surface list
//  std::vector<const SurfaceModel*> surfaceList() const;

  //! Make texture list
  std::vector<const Texture*> textureList() const noexcept;

  //! Return the world object list
//  const std::vector<Object>& objectList() const;

  //! Select a light source randomly
  const LightSourceReference& sampleLightSource(Sampler& sampler) const noexcept;

 private:
  //! Return the material size
  std::size_t getMaterialSize() const noexcept;

  //! Return the object size
  std::size_t getObjectSize(const std::vector<Object>& object_list) const noexcept;

  //! Initialize world
  void initialize(System& system, 
                  const QJsonObject& settings, 
                  const std::function<void (const QString&)>& message_sender) noexcept;

  //! Initialize emitter list
  void initializeEmitter(System& system, const QJsonArray& settings) noexcept;

  //! Initialize Objects
  std::vector<Object> initializeObject(System& system, 
                                       const QJsonArray& settings) noexcept;

  //! Initialize the world information of light sources
  void initializeWorldLightSource() noexcept;

  //! Initialize surface scattering list
  void initializeSurface(System& system, const QJsonArray& settings) noexcept;

  //! Initialize texture list
  void initializeTexture(System& system, const QJsonArray& settings) noexcept;

  //! Make objects
  std::list<std::future<std::vector<Object>>> makeObjects(
      System& system,
      const QJsonArray& settings) const noexcept;

  //! Make a single object
  std::vector<Object> makeSingleObject(const QJsonObject& settings,
                                       const Matrix4x4& transformation) const noexcept;

  std::list<std::future<std::vector<Object>>> makeGroupObject(
      System& system,
      const QJsonArray& settings,
      const QJsonObject& group_settings,
      const Matrix4x4& transformation,
      const uint count,
      uint& index) const noexcept;


  UniquePointer<Bvh> bvh_;
  UniquePointer<LightSourceSampler> light_source_sampler_;
  std::vector<UniquePointer<EmitterModel>> emitter_list_;
  std::vector<UniquePointer<SurfaceModel>> surface_list_;
  std::vector<UniquePointer<Texture>> texture_list_;
  std::vector<LightSourceReference> light_source_list_;
};

} // namespace nanairo

#include "world-inl.hpp"

#endif // NANAIRO_WORLD_HPP
