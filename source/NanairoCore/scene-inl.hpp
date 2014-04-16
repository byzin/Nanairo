/*!
  \file scene-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SCENE_INL_HPP_
#define _NANAIRO_SCENE_INL_HPP_

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
CameraModel& Scene::camera()
{
  return *camera_;
}

/*!
  \details
  No detailed.
  */
inline
const CameraModel& Scene::camera() const
{
  return *camera_;
}

/*!
  \details
  No detailed.
  */
inline
Film& Scene::film()
{
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
const Film& Scene::film() const
{
  return *film_;
}

/*!
  \details
  No detailed.
  */
inline
const World& Scene::world() const
{
  return *world_;
}

} // namespace nanairo

#endif // _NANAIRO_SCENE_INL_HPP_
