/*!
  \file world.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_WORLD_HPP
#define NANAIRO_WORLD_HPP

// Standard C++ library
#include <cstddef>
#include <future>
#include <list>
#include <memory>
#include <tuple>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "Data/object.hpp"
#include "Geometry/transformation.hpp"
#include "Material/material.hpp"
#include "Material/EmitterModel/emitter_model.hpp"
#include "Material/SurfaceModel/surface_model.hpp"
#include "Material/TextureModel/texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Setting/setting_node_base.hpp"

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
class World : public zisc::NonCopyable<World>
{
 public:
  //! Initialize the world
  World(System& system, const SettingNodeBase* settings) noexcept;

  //! Finalize the world
  ~World() noexcept;


  //! Return the bvh
  const Bvh& bvh() const noexcept;

  //! Return the texture list
  const zisc::pmr::vector<const EmitterModel*>& emitterList() const noexcept;

  //! Return the light source list
  const zisc::pmr::vector<const Object*>& lightSourceList() const noexcept;

  //! Return the material list
  const zisc::pmr::vector<const Material*>& materialList() const noexcept;

  //! Return the object list
  const zisc::pmr::vector<Object>& objectList() const noexcept;

  //! Return the texture list
  const zisc::pmr::vector<const SurfaceModel*>& surfaceList() const noexcept;

  //! Return the texture list
  const zisc::pmr::vector<const TextureModel*>& textureList() const noexcept;

 private:
  using ObjectSet = std::tuple<zisc::pmr::vector<Object>,
                               zisc::UniqueMemoryPointer<Material>>;
  using ObjectSetResult = zisc::ThreadManager::UniqueResult<ObjectSet>;


  //! Initialize world
  void initialize(System& system, const SettingNodeBase* settings) noexcept;

  //! Initialize emitter list
  void initializeEmitter(System& system, const SettingNodeBase* settings) noexcept;

  //! Initialize Objects
  zisc::pmr::vector<Object> initializeObject(
      System& system,
      const SettingNodeBase* settings) noexcept;

  //! Initialize the world information of light sources
  void initializeWorldLightSource() noexcept;

  //! Initialize surface scattering list
  void initializeSurface(System& system, const SettingNodeBase* settings) noexcept;

  //! Initialize texture list
  void initializeTexture(System& system, const SettingNodeBase* settings) noexcept;

  //! Make objects
  zisc::pmr::vector<ObjectSet> makeObjects(
      System& system,
      const SettingNodeBase* settings) const noexcept;

  //! Make objects
  void makeObjects(
      System& system,
      const SettingNodeBase* settings,
      Matrix4x4 transformation,
      zisc::pmr::list<ObjectSetResult>& result) const noexcept;

  //! Make a single object
  void makeSingleObject(
      System& system,
      const SettingNodeBase* settings,
      const Matrix4x4& transformation,
      zisc::pmr::list<ObjectSetResult>& result) const noexcept;

  void makeGroupObject(
      System& system,
      const SettingNodeBase* settings,
      const Matrix4x4& transformation,
      zisc::pmr::list<ObjectSetResult>& result) const noexcept;


  zisc::pmr::vector<const EmitterModel*> emitter_list_;
  zisc::pmr::vector<const SurfaceModel*> surface_list_;
  zisc::pmr::vector<const TextureModel*> texture_list_;
  zisc::pmr::vector<const Material*> material_list_;
  zisc::pmr::vector<const Object*> light_source_list_;
  zisc::pmr::vector<zisc::UniqueMemoryPointer<EmitterModel>> emitter_body_list_;
  zisc::pmr::vector<zisc::UniqueMemoryPointer<SurfaceModel>> surface_body_list_;
  zisc::pmr::vector<zisc::UniqueMemoryPointer<TextureModel>> texture_body_list_;
  zisc::pmr::vector<zisc::UniqueMemoryPointer<Material>> material_body_list_;
  zisc::UniqueMemoryPointer<Bvh> bvh_;
};

//! \} Core

} // namespace nanairo

#include "world-inl.hpp"

#endif // NANAIRO_WORLD_HPP
