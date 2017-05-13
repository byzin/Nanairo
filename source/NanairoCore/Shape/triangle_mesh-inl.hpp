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
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
void TriangleMesh::setTextureCoordinate(const Point2& texture_coordinate1,
                                        const Point2& texture_coordinate2,
                                        const Point2& texture_coordinate3) noexcept
{
  texture_coordinate_ = texture_coordinate1;
  texture_edge_[0] = texture_coordinate2 - texture_coordinate1;
  texture_edge_[1] = texture_coordinate3 - texture_coordinate1;
}

/*!
  \details
  No detailed.
  */
inline
Point2 TriangleMesh::textureCoordinate(const Float u, const Float v) const noexcept
{
  Point2 texture_coordinate = texture_coordinate_ + u * texture_edge_[0] +
                                                    v * texture_edge_[1];
  ZISC_ASSERT(zisc::isInClosedBounds(texture_coordinate[0], 0.0, 1.0),
              "The texture_coordinate[0] is out of the range [0.0, 1.0]: ",
              texture_coordinate[0]);
  ZISC_ASSERT(zisc::isInClosedBounds(texture_coordinate[1], 0.0, 1.0),
              "The texture_coordinate[1] is out of the range [0.0, 1.0]: ",
              texture_coordinate[1]);
  return texture_coordinate;
}

} // namespace nanairo

#endif // NANAIRO_TRIANGLE_MESH_INL_HPP
