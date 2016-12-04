/*!
  \file smoothed_mesh-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTHED_MESH_INL_HPP
#define NANAIRO_SMOOTHED_MESH_INL_HPP

#include "smoothed_mesh.hpp"
// Standard C++ library
#include <array>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

using zisc::treatAs;

/*!
  */
inline
std::array<Float, 10> SmoothedMesh::calcCurveCoefficients(
    const Ray& ray) const noexcept
{
  const auto plane1 = calcRayPlane(ray, c_[4]);
  const auto& d1 = std::get<0>(plane1);
  const Float k1 = std::get<1>(plane1);
  const auto plane2 = calcRayPlane(ray, c_[5]);
  const auto& d2 = std::get<0>(plane2);
  const Float k2 = std::get<1>(plane2);

  // Plane1
  const Float a = zisc::dot(c_[0], d1);
  const Float b = zisc::dot(c_[1], d1);
  const Float c = zisc::dot(c_[2], d1) + k1;
  const Float d = zisc::dot(c_[3], d1);
//  const Float e = zisc::dot(c_[4], d1);
  const Float f = zisc::dot(c_[5], d1);
  // Plane2
  const Float l = zisc::dot(c_[0], d2);
  const Float m = zisc::dot(c_[1], d2);
  const Float n = zisc::dot(c_[2], d2) + k2;
  const Float o = zisc::dot(c_[3], d2);
  const Float p = zisc::dot(c_[4], d2);
//  const Float q = zisc::dot(c_[5], d2);

  return std::array<Float, 10>{{a, b, c, d, f, l, m, n, o, p}};
}

/*!
  \details
  No detailed.
  */
inline
Vector3 SmoothedMesh::normal(const Float u, const Float v) const noexcept
{
  const auto du = (2.0 * u) * c_[0] + v * c_[3] + c_[4];
  const auto dv = (2.0 * v) * c_[1] + u * c_[3] + c_[5];
  return zisc::cross(du, dv).normalized();
}

/*!
  */
inline
Vector3 SmoothedMesh::normal1() const noexcept
{
  const auto du = 2.0 * c_[0] + c_[4];
  const auto dv = c_[3] + c_[5];
  return zisc::cross(du, dv).normalized();
}

/*!
  */
inline
Vector3 SmoothedMesh::normal2() const noexcept
{
  const auto du = c_[3] + c_[4];
  const auto dv = 2.0 * c_[1] + c_[5];
  return zisc::cross(du, dv).normalized();
}

/*!
  */
inline
Vector3 SmoothedMesh::normal3() const noexcept
{
  const auto& du = c_[4];
  const auto& dv = c_[5];
  return zisc::cross(du, dv).normalized();
}

/*!
  \details
  No detailed.
  */
inline
Point3 SmoothedMesh::point(const Float u, const Float v) const noexcept
{
  const auto p = (c_[0] * u + c_[3] * v + c_[4]) * u + (c_[1] * v + c_[5]) * v + c_[2];
  return Point3{p.data()};
}

/*!
  */
inline
Vector3 SmoothedMesh::vertex1() const noexcept
{
  return c_[0] + c_[2] + c_[4];
}

/*!
  */
inline
Vector3 SmoothedMesh::vertex2() const noexcept
{
  return c_[1] + c_[2] + c_[5];
}

/*!
  */
inline
const Vector3& SmoothedMesh::vertex3() const noexcept
{
  return c_[2];
}

/*!
  */
inline
constexpr auto SmoothedMesh::smoothingMethod() noexcept -> SmoothingMethod
{
  return SmoothingMethod::kResultant;
//  return SmoothingMethod::kPencil;
//  return SmoothingMethod::kNoSmoothed;
}

} // namespace nanairo

#endif // NANAIRO_SMOOTHED_MESH_INL_HPP
