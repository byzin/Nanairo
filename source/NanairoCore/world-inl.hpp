/*!
  \file world-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_WORLD_INL_HPP
#define NANAIRO_WORLD_INL_HPP

#include "world.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "Data/object.hpp"
#include "DataStructure/bvh.hpp"
#include "Material/material.hpp"
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
const zisc::pmr::vector<const EmitterModel*>& World::emitterList() const noexcept
{
  return emitter_list_;
}

/*!
  \details
  No detailed.
  */
inline
const zisc::pmr::vector<const Object*>& World::lightSourceList() const noexcept
{
  return light_source_list_;
}

/*!
  */
inline
const zisc::pmr::vector<const Material*>& World::materialList() const noexcept
{
  return material_list_;
}

/*!
  */
inline
const zisc::pmr::vector<Object>& World::objectList() const noexcept
{
  return bvh().objectList();
}

/*!
  */
inline
const zisc::pmr::vector<const SurfaceModel*>& World::surfaceList() const noexcept
{
  return surface_list_;
}

/*!
  */
inline
const zisc::pmr::vector<const TextureModel*>& World::textureList() const noexcept
{
  return texture_list_;
}

} // namespace nanairo

#endif // NANAIRO_WORLD_INL_HPP
