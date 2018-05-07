/*!
  \file scene.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SCENE_HPP
#define NANAIRO_SCENE_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
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
class Scene : public zisc::NonCopyable<Scene>
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


  zisc::UniqueMemoryPointer<CameraModel> camera_;
  zisc::UniqueMemoryPointer<Film> film_;
  zisc::UniqueMemoryPointer<World> world_;
};

//! \} Core

} // namespace nanairo

#include "scene-inl.hpp"

#endif // NANAIRO_SCENE_HPP
