/*!
  \file plane.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PLANE_HPP
#define NANAIRO_PLANE_HPP

// Standard C++ library
#include <array>
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
 \brief 3D plane
 \details
 No detailed.
 */
class Plane : public Shape
{
 public:
  //! Initialize
  Plane() noexcept;


  //! Return the bounding box
  Aabb boundingBox() const noexcept override;

  //! Return the edges of the plane
  const std::array<Vector3, 2>& edge() const noexcept;

  //! Return the point and the normal by the st coordinate
  ShapePoint getPoint(const Point2& st) const noexcept override;

  //! Return the cost of a ray-plane intersection test
  Float getTraversalCost() const noexcept override;

  //! Return the normal of the plane
  const Vector3& normal() const noexcept;

  //! Test ray-plane intersection
  IntersectionTestResult testIntersection(
      const Ray& ray, 
      IntersectionInfo* intersection) const noexcept override;

  //! Test ray-plane intersection
  static bool testIntersection(const Point3& v,
                               const std::array<Vector3, 2>& e,
                               const Vector3& normal,
                               const Ray& ray, 
                               const Float max_distance,
                               Point2* st,
                               IntersectionInfo* intersection = nullptr) noexcept;

  //! Sample a point randomly on the surface of the plane 
  ShapePoint samplePoint(Sampler& sampler) const noexcept override;

  //! Return the vertex of the plane
  const Point3& vertex0() const noexcept;

 private:
  //! Calculate the surface area of the front side of the plane
  Float calcSurfaceArea() const noexcept override;

  //! Initialize
  void initialize() noexcept;

  //! Apply affine transformation
  void transformShape(const Matrix4x4& matrix) noexcept override;


  Point3 vertex0_; //!< The v0
  std::array<Vector3, 2> edge_; //!< Edges to v1 and v2
  Vector3 normal_;
};

//! \} Core

} // namespace nanairo

#include "plane-inl.hpp"

#endif // NANAIRO_PLANE_HPP
