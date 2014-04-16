/*!
  \file scene.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SCENE_HPP_
#define _NANAIRO_SCENE_HPP_

// Standard C++ library
#include <cstddef>
#include <functional>
// Nanairo
#include "Utility/unique_pointer.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class CameraModel;
class Film;
class SceneSettings;
class System;
class World;

/*!
  \brief Rendering scene
  \details
  No detailed.
  */
class Scene
{
 public:
  //! Initialize the scene
  Scene(System& system, 
        const SceneSettings& settings, 
        const std::function<void (const QString&)>& message_sender);


  //! Return the camera
  CameraModel& camera();

  //! Return the camera
  const CameraModel& camera() const;

  //! Return the film
  Film& film();

  //! Returh the film
  const Film& film() const;

  //! Return the world data
  const World& world() const;

 private:
  //! Initialize the scene
  void initialize(System& system, 
                  const SceneSettings& settings,
                  const std::function<void (const QString&)>& message_sender);


  UniquePointer<CameraModel> camera_;
  UniquePointer<Film> film_;
  UniquePointer<World> world_;
};

} // namespace nanairo

#include "scene-inl.hpp"

#endif // _NANAIRO_SCENE_HPP_
