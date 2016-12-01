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
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

using zisc::treatAs;

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

} // namespace nanairo

#endif // NANAIRO_SMOOTHED_MESH_INL_HPP
