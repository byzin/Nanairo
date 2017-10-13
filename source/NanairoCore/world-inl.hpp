/*!
  \file world-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_WORLD_INL_HPP
#define NANAIRO_WORLD_INL_HPP

#include "world.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "Data/object.hpp"
#include "DataStructure/bvh.hpp"
#include "Material/EmitterModel/emitter_model.hpp"
#include "Material/SurfaceModel/surface_model.hpp"
#include "Material/TextureModel/texture_model.hpp"

namespace nanairo {

/*!
  \details
  No detailed.

  \return BVH
  */
inline
const Bvh& World::bvh() const noexcept
{
  return *bvh_;
}

/*!
  */
inline
const std::vector<EmitterModel*>& World::emitterList() const noexcept
{
  const auto emitter_list =
      zisc::treatAs<const std::vector<EmitterModel*>*>(&emitter_list_);
  return *emitter_list;
}

/*!
  \details
  No detailed.
  */
inline
const std::vector<const Object*>& World::lightSourceList() const noexcept
{
  return light_source_list_;
}

/*!
  */
inline
const std::vector<Object>& World::objectList() const noexcept
{
  return bvh().objectList();
}

/*!
  */
inline
const std::vector<SurfaceModel*>& World::surfaceList() const noexcept
{
  const auto surface_list =
      zisc::treatAs<const std::vector<SurfaceModel*>*>(&surface_list_);
  return *surface_list;
}

/*!
  */
inline
const std::vector<TextureModel*>& World::textureList() const noexcept
{
  const auto texture_list =
      zisc::treatAs<const std::vector<TextureModel*>*>(&texture_list_);
  return *texture_list;
}

} // namespace nanairo

#endif // NANAIRO_WORLD_INL_HPP
