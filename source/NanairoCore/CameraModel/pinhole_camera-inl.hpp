/*!
  \file pinhole_camera-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PINHOLE_CAMERA_INL_HPP
#define NANAIRO_PINHOLE_CAMERA_INL_HPP

#include "pinhole_camera.hpp"
// Standard C++ library
#include <array>
#include <limits>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Shape/plane.hpp"

namespace nanairo {

// Forward declaration
class Ray;

/*!
  */
inline
Float PinholeCamera::angleOfView() const noexcept
{
  return angle_of_view_;
}

/*!
  */
inline
auto PinholeCamera::filmShape() const noexcept -> const FilmShape&
{
  return film_shape_;
}

/*!
  */
inline
const Vector3& PinholeCamera::FilmShape::normal() const noexcept
{
  return normal_;
}

/*!
  */
inline
Point3 PinholeCamera::FilmShape::center() const noexcept
{
  const auto& v = vertex0();
  const auto& e = edge();
  const auto c = v + 0.5 * (e[0] + e[1]);
  return c;
}

/*!
  */
inline
const std::array<Vector3, 2>& PinholeCamera::FilmShape::edge() const noexcept
{
  return edge_;
}

/*!
  */
inline
void PinholeCamera::FilmShape::setCenter(const Point3& c) noexcept
{
  const auto& e = edge();
  const auto v = c - 0.5 * (e[0] + e[1]);
  vertex0_ = v;
}

/*!
  */
inline
Float PinholeCamera::FilmShape::surfaceArea() const noexcept
{
  return surface_area_;
}

/*!
  */
inline
bool PinholeCamera::FilmShape::testIntersection(const Ray& ray, Point2* st)
    const noexcept
{
  ZISC_ASSERT(st != nullptr, "The st is null.");
  constexpr Float max_distance = std::numeric_limits<Float>::max();
  const bool result = Plane::testIntersection(vertex0(),
                                              edge(),
                                              normal(),
                                              ray,
                                              max_distance,
                                              st);
  return result;
}

/*!
  */
inline
const Point3& PinholeCamera::FilmShape::vertex0() const noexcept
{
  return vertex0_;
}

/*!
  */
inline
Vector3 PinholeCamera::FilmShape::calcNormal() const noexcept
{
  const auto& e = edge();
  const auto n = zisc::cross(e[0], e[1]).normalized();
  return n;
}

} // namespace nanairo

#endif // NANAIRO_PINHOLE_CAMERA_INL_HPP
