/*!
  \file triangle_mesh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TRIANGLE_MESH_HPP
#define NANAIRO_TRIANGLE_MESH_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "shape.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class Ray;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
enum class MeshType : int
{
  Flat = 0,
//  NormalSmoothed,
  Smoothed
};

/*!
  \details
  No detailed.
  */
class TriangleMesh : public Shape
{
 public:
  //! Make meshes
  static std::vector<UniquePointer<Shape>> makeMeshes(
      const SettingNodeBase* settings) noexcept;

  //! Make a smoothed mesh
  static UniquePointer<Shape> makeSmoothedMesh(const Point3& vertex1,
                                               const Point3& vertex2,
                                               const Point3& vertex3,
                                               const Vector3& normal1,
                                               const Vector3& normal2,
                                               const Vector3& normal3) noexcept;

  //! Set vertex texture coordinate
  void setTextureCoordinate(const Point2& texture_coordinate1,
                            const Point2& texture_coordinate2,
                            const Point2& texture_coordinate3) noexcept;

  //! Calculate the texture coordinate using Barycentric coordinate system
  Point2 textureCoordinate(const Float u, const Float v) const noexcept;

 private:
  Point2 texture_coordinate_;
  Vector2 texture_edge_[2];
};

//! \} Core

} // namespace nanairo

#include "triangle_mesh-inl.hpp"

#endif // NANAIRO_TRIANGLE_MESH_HPP
