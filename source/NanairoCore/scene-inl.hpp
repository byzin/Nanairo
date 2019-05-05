/*!
  \file scene-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SCENE_INL_HPP
#define NANAIRO_SCENE_INL_HPP

#include "scene.hpp"
// Standard C++ library
#include <vector>
// Nanairo
#include "world.hpp"
#include "CameraModel/camera_model.hpp"
#include "CameraModel/film.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
CameraModel& Scene::camera() noexcept
{
  return *camera_;
}

/*!
  \details
  No detailed.
  */
inline
const CameraModel& Scene::camera() const noexcept
{
  return *camera_;
}

/*!
  \details
  No detailed.
  */
inline
Film& Scene::film() noexcept
{
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
const Film& Scene::film() const noexcept
{
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
const World& Scene::world() const noexcept
{
  return *world_;
}

} // namespace nanairo

#endif // NANAIRO_SCENE_INL_HPP
