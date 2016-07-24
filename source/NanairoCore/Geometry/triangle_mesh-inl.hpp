/*!
  \file triangle_mesh-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TRIANGLE_MESH_INL_HPP
#define NANAIRO_TRIANGLE_MESH_INL_HPP

#include "triangle_mesh.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
void TriangleMesh::setTextureCoordinate(const Point2& texture_coordinate0,
                                        const Point2& texture_coordinate1,
                                        const Point2& texture_coordinate2) noexcept
{
  texture_coordinate_[0] = texture_coordinate0;
  texture_coordinate_[1] = texture_coordinate1;
  texture_coordinate_[2] = texture_coordinate2;
}

/*!
  \details
  No detailed.
  */
inline
Point2 TriangleMesh::textureCoordinate(const Float* barycentric) const noexcept
{
  Point2 texture_coordinate;
  for (uint index = 0; index < 2; ++index) {
    texture_coordinate[index] = barycentric[2] * texture_coordinate_[0][index] +
                                barycentric[0] * texture_coordinate_[1][index] +
                                barycentric[1] * texture_coordinate_[2][index];
  }
  return texture_coordinate;
}

} // namespace nanairo

#endif // NANAIRO_TRIANGLE_MESH_INL_HPP
