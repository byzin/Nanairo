/*!
  \file flat_triangle.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FLAT_TRIANGLE_HPP
#define NANAIRO_FLAT_TRIANGLE_HPP

// Standard C++ library
#include <array>
// Zisc
#include "zisc/matrix.hpp"
// Nanairo
#include "shape.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_test_result.hpp"
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
class FlatTriangle : public Shape
{
 public:
  //! Create a flat triangle 
  FlatTriangle(const Point3& vertex1,
               const Point3& vertex2,
               const Point3& vertex3) noexcept;


  //! Return the bounding box
  Aabb boundingBox() const noexcept override;

  //! Return the edges of the triangle
  const std::array<Vector3, 2>& edge() const noexcept;

  //! Return the point data by the st coordinate
  ShapePoint getPoint(const Point2& st) const noexcept override;

  //! Return the cost of a ray-triangle intersection test
  Float getTraversalCost() const noexcept override;

  //! Return the normal of the triangle
  const Vector3& normal() const noexcept;

  //! Test ray-triangle intersection
  IntersectionTestResult testIntersection(
      const Ray& ray,
      IntersectionInfo* intersection) const noexcept override;

  //! Sample a point randomly on the surface of the triangle
  ShapePoint samplePoint(Sampler& sampler) const noexcept override;

  //! Set the UVs of the vertice
  void setUv(const Point2& uv1,
             const Point2& uv2,
             const Point2& uv3) noexcept;

  //! Return the UV of the vertex0
  const Point2& uv0() const noexcept;

  //! Return the UV edges
  const std::array<Vector2, 2>& uvEdge() const noexcept;

  //! Return the vertex of the triangle
  const Point3& vertex0() const noexcept;

 private:
  struct CanonicalMatrix
  {
    Vector3 row1_xyz_;
    Float row1_w_;
    Vector3 row2_xyz_;
    Float row2_w_;
    Vector3 row3_xyz_;
    Float row3_w_;
  };


  //! Calculate the normal vector
  Vector3 calcNormal() const noexcept;

  //! Calculate the surface area of the front side of the triangle
  Float calcSurfaceArea() const noexcept override;

  //! Calculate the UV of the point
  Point2 calcUv(const Point2& st) const noexcept;

  //! Initialize the matrix to transform canonical coordinate
  void initCanonicalMatrix() noexcept;

  // Initialize the flat triangle
  void initialize() noexcept;

  //! Return the matrix to transform canonical coordinate
  const CanonicalMatrix& toCanonicalMatrix() const noexcept;

  //! Apply affine transformation
  void transformShape(const Matrix4x4& matrix) noexcept override;


  CanonicalMatrix to_canonical_;
  Point3 vertex0_; //!< The v0
  std::array<Vector3, 2> edge_; //!< Edges to v1 and v2
  Vector3 normal_;
  Point2 uv0_;
  std::array<Vector2, 2> uv_edge_;
};

//! \} Core

} // namespace nanairo

#include "flat_triangle-inl.hpp"

#endif // NANAIRO_FLAT_TRIANGLE_HPP
