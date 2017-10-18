/*!
  \file plane-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PLANE_INL_HPP
#define NANAIRO_PLANE_INL_HPP

#include "plane.hpp"
// Standard C++ library
#include <array>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  */
inline
const std::array<Vector3, 2>& Plane::edge() const noexcept
{
  return edge_;
}

/*!
  */
inline
const Vector3& Plane::normal() const noexcept
{
  return normal_;
}

/*!
  */
inline
const Point3& Plane::vertex0() const noexcept
{
  return vertex0_;
}

} // namespace nanairo

#endif // NANAIRO_PLANE_INL_HPP
