/*!
  \file scene.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SCENE_HPP
#define NANAIRO_SCENE_HPP

// Standard C++ library
#include <cstddef>
// Nanairo
#include "Utility/unique_pointer.hpp"
#include "Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class CameraModel;
class Film;
class System;
class World;

//! \addtogroup Core
//! \{

/*!
  \brief Rendering scene
  \details
  No detailed.
  */
class Scene
{
 public:
  //! Initialize the scene
  Scene(System& system, const SettingNodeBase* settings) noexcept;


  //! Return the camera
  CameraModel& camera() noexcept;

  //! Return the camera
  const CameraModel& camera() const noexcept;

  //! Return the film
  Film& film() noexcept;

  //! Returh the film
  const Film& film() const noexcept;

  //! Return the world data
  const World& world() const noexcept;

 private:
  //! Initialize the scene
  void initialize(System& system, const SettingNodeBase* settings) noexcept;

  //! Initialize the camera
  void initializeCamera(System& system, const SettingNodeBase* settings) noexcept;


  UniquePointer<CameraModel> camera_;
  UniquePointer<Film> film_;
  UniquePointer<World> world_;
};

//! \} Core

} // namespace nanairo

#include "scene-inl.hpp"

#endif // NANAIRO_SCENE_HPP
