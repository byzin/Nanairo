/*!
  \file flat_triangle-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FLAT_TRIANGLE_INL_HPP
#define NANAIRO_FLAT_TRIANGLE_INL_HPP

#include "flat_triangle.hpp"
// Standard C++ library
#include <array>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  */
inline
const std::array<Vector3, 2>& FlatTriangle::edge() const noexcept
{
  return edge_;
}

/*!
  */
inline
const Vector3& FlatTriangle::normal() const noexcept
{
  return normal_;
}

/*!
  */
inline
const Point2& FlatTriangle::uv0() const noexcept
{
  return uv0_;
}

/*!
  */
inline
const std::array<Vector2, 2>& FlatTriangle::uvEdge() const noexcept
{
  return uv_edge_;
}

/*!
  */
inline
const Point3& FlatTriangle::vertex0() const noexcept
{
  return vertex0_;
}

/*!
  */
inline
Point2 FlatTriangle::calcUv(const Point2& st) const noexcept
{
  const auto& uv_edge = uvEdge();
  Point2 uv = uv0() + st[0] * uv_edge[0] + st[1] * uv_edge[1];
  for (uint i = 0; i < uv.size(); ++i) {
    while (!zisc::isInClosedBounds(uv[i], 0.0, 1.0))
      uv[i] = (uv[i] < 0.0) ? (uv[i] + 1.0) : (uv[i] - 1.0);
  }
  return uv;
}

/*!
  */
inline
auto FlatTriangle::toCanonicalMatrix() const noexcept -> const CanonicalMatrix&
{
  return to_canonical_;
}

} // namespace nanairo

#endif // NANAIRO_FLAT_TRIANGLE_INL_HPP
