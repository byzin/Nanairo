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
#include <future>
#include <list>
#include <vector>
// Nanairo
#include "Data/light_source_reference.hpp"
#include "Data/object.hpp"
#include "Material/EmitterModel/emitter_model.hpp"
#include "Material/SurfaceModel/surface_model.hpp"
#include "Material/TextureModel/texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/light_source_sampler.hpp"
#include "Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class Bvh;
class System;

//! \addtogroup Core
//! \{

/*!
 \brief Rendering world
 \details
 No detailed.
 */
class World
{
 public:
  //! Initialize the world
  World(System& system, const QJsonObject& settings) noexcept;

  //! Finalize the world
  ~World() noexcept;


  //! Return the bvh
  const Bvh& bvh() const noexcept;

  //! Return the light source list
  const std::vector<LightSourceReference>& lightSourceList() const noexcept;

  //! Return the world light source sampler
  const LightSourceSampler& lightSourceSampler() const noexcept;

  //! Make texture list
  std::vector<const TextureModel*> textureList() const noexcept;

  //! Select a light source randomly
  const LightSourceReference& sampleLightSource(Sampler& sampler) const noexcept;

 private:
  //! Initialize world
  void initialize(System& system, const QJsonObject& settings) noexcept;

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
  std::vector<Object> makeSingleObject(
      const QJsonObject& settings,
      const Matrix4x4& transformation) const noexcept;

  void makeGroupObject(
      System& system,
      const QJsonArray& settings,
      const Matrix4x4& transformation,
      int& index,
      std::list<std::future<std::vector<Object>>>& results) const noexcept;


  std::vector<UniquePointer<EmitterModel>> emitter_list_;
  std::vector<UniquePointer<SurfaceModel>> surface_list_;
  std::vector<UniquePointer<TextureModel>> texture_list_;
  std::vector<LightSourceReference> light_source_list_;
  UniquePointer<Bvh> bvh_;
  UniquePointer<LightSourceSampler> light_source_sampler_;
};

//! \} Core

} // namespace nanairo

#include "world-inl.hpp"

#endif // NANAIRO_WORLD_HPP
