/*!
  \file flat_mesh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FLAT_MESH_HPP
#define NANAIRO_FLAT_MESH_HPP

// Zisc
#include "zisc/matrix.hpp"
// Nanairo
#include "shape.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

// Forward declaration
class Aabb;
class IntersectionInfo;
class Ray;
class Sampler;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class FlatMesh : public TriangleMesh
{
 public:
  //! Create flat mesh
  FlatMesh(const Point3& vertex1,
           const Point3& vertex2,
           const Point3& vertex3) noexcept;


  //! Return the bounding box
  Aabb boundingBox() const noexcept override;

  //! Return the point data by the st coordinate
  ShapePoint getPoint(const Point2& st) const noexcept override;

  //! Return the cost of a ray-triangle intersection test
  Float getTraversalCost() const noexcept override;

  //! Test ray-triangle intersection
  bool testIntersection(const Ray& ray,
                        IntersectionInfo* intersection) const noexcept override;

  //! Sample a point randomly on the surface of the triangle
  ShapePoint samplePoint(Sampler& sampler) const noexcept override;

  //! Apply affine transformation
  void transform(const Matrix4x4& matrix) noexcept override;

 private:
  void initialize(const Point3& vertex2, const Point3& vertex3) noexcept;

  //! Set normal vector
  void setNormal() noexcept;

  //! Set the world-to-local matrix
  void setToLocalMatrix() noexcept;


  zisc::Matrix<Float, 3, 4> to_local_;
  Point3 vertex_; // v1
  Vector3 edge_[2]; // v1 to v2 and v1 to v3 edge vectors
  Vector3 normal_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_FLAT_MESH_HPP
