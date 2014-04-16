/*!
  \file world.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_WORLD_HPP_
#define _NANAIRO_WORLD_HPP_

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
class QString;

namespace nanairo {

// Forward declaration
class Bvh;
class SceneSettings;
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
        const SceneSettings& settings,
        const std::function<void (const QString&)>& message_sender);

  //! Finalize the world
  ~World();


  //! Return the bvh
  const Bvh& bvh() const;

  //! Make emitter model list
  std::vector<const EmitterModel*> emitterList() const;

  //! Return the light source list
  const std::vector<LightSourceReference>& lightSourceList() const;

  //! Return the world light source sampler
  const LightSourceSampler& lightSourceSampler() const;

  //! Make surface list
//  std::vector<const SurfaceModel*> surfaceList() const;

  //! Make texture list
  std::vector<const Texture*> textureList() const;

  //! Return the world object list
//  const std::vector<Object>& objectList() const;

  //! Select a light source randomly
  const LightSourceReference& sampleLightSource(Sampler& sampler) const;

 private:
  //! Return the material size
  std::size_t getMaterialSize() const;

  //! Return the object size
  std::size_t getObjectSize(const std::vector<Object>& object_list) const;

  //! Initialize world
  void initialize(System& system, 
                  const SceneSettings& settings, 
                  const std::function<void (const QString&)>& message_sender);

  //! Initialize emitter list
  void initializeEmitter(System& system, const SceneSettings& settings);

  //! Initialize Objects
  std::vector<Object> initializeObject(System& system, 
                                       const SceneSettings& settings);

  //! Initialize the world information of light sources
  void initializeWorldLightSource();

  //! Initialize surface scattering list
  void initializeSurface(System& system, const SceneSettings& settings);

  //! Initialize texture list
  void initializeTexture(System& system, const SceneSettings& settings);

  //! Make objects
  std::list<std::future<std::vector<Object>>> makeObjects(
      System& system,
      const SceneSettings& settings) const;

  //! Make a single object
  std::vector<Object> makeSingleObject(const SceneSettings& settings,
                                       const QString& prefix,
                                       const Matrix4x4& transformation) const;

  std::list<std::future<std::vector<Object>>> makeGroupObject(
      System& system,
      const SceneSettings& settings,
      const QString& prefix,
      const Matrix4x4& transformation,
      const uint count,
      uint& index) const;


  UniquePointer<Bvh> bvh_;
  UniquePointer<LightSourceSampler> light_source_sampler_;
  std::vector<UniquePointer<EmitterModel>> emitter_list_;
  std::vector<UniquePointer<SurfaceModel>> surface_list_;
  std::vector<UniquePointer<Texture>> texture_list_;
  std::vector<LightSourceReference> light_source_list_;
};

} // namespace nanairo

#include "world-inl.hpp"

#endif // _NANAIRO_WORLD_HPP_
