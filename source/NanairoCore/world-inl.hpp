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
// Nanairo
#include "NanairoCore/DataStructure/bvh.hpp"

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
  \details
  No detailed.
  */
inline
const std::vector<const Object*>& World::lightSourceList() const noexcept
{
  return light_source_list_;
}

} // namespace nanairo

#endif // NANAIRO_WORLD_INL_HPP
