/*!
  \file world.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
#include "Data/object.hpp"
#include "Material/EmitterModel/emitter_model.hpp"
#include "Material/SurfaceModel/surface_model.hpp"
#include "Material/TextureModel/texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Setting/setting_node_base.hpp"
#include "Utility/unique_pointer.hpp"

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
  World(System& system, const SettingNodeBase* settings) noexcept;

  //! Finalize the world
  ~World() noexcept;


  //! Return the bvh
  const Bvh& bvh() const noexcept;

  //! Return the light source list
  const std::vector<const Object*>& lightSourceList() const noexcept;

  //! Make texture list
  std::vector<const TextureModel*> textureList() const noexcept;

 private:
  //! Initialize world
  void initialize(
      System& system,
      const SettingNodeBase* settings) noexcept;

  //! Initialize emitter list
  void initializeEmitter(
      System& system,
      const SettingNodeBase* settings,
      const std::vector<const TextureModel*>& texture_list) noexcept;

  //! Initialize Objects
  std::vector<Object> initializeObject(
      System& system, 
      const SettingNodeBase* settings) noexcept;

  //! Initialize the world information of light sources
  void initializeWorldLightSource() noexcept;

  //! Initialize surface scattering list
  void initializeSurface(
      System& system,
      const SettingNodeBase* settings,
      const std::vector<const TextureModel*>& texture_list) noexcept;

  //! Initialize texture list
  void initializeTexture(
      System& system,
      const SettingNodeBase* settings) noexcept;

  //! Make objects
  std::vector<std::vector<Object>> makeObjects(
      System& system,
      const SettingNodeBase* settings) const noexcept;

  //! Make objects
  void makeObjects(
      System& system,
      const SettingNodeBase* settings,
      Matrix4x4 transformation,
      std::list<std::future<std::vector<Object>>>& results) const noexcept;

  //! Make a single object
  void makeSingleObject(
      System& system,
      const SettingNodeBase* settings,
      const Matrix4x4& transformation,
      std::list<std::future<std::vector<Object>>>& results) const noexcept;

  void makeGroupObject(
      System& system,
      const SettingNodeBase* settings,
      const Matrix4x4& transformation,
      std::list<std::future<std::vector<Object>>>& results) const noexcept;


  std::vector<UniquePointer<EmitterModel>> emitter_list_;
  std::vector<UniquePointer<SurfaceModel>> surface_list_;
  std::vector<UniquePointer<TextureModel>> texture_list_;
  std::vector<const Object*> light_source_list_;
  UniquePointer<Bvh> bvh_;
};

//! \} Core

} // namespace nanairo

#include "world-inl.hpp"

#endif // NANAIRO_WORLD_HPP
