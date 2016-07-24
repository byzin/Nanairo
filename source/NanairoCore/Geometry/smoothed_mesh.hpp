/*!
  \file smoothed_mesh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTHED_MESH_HPP
#define NANAIRO_SMOOTHED_MESH_HPP

// Standard C++ library
#include <tuple>
#include <utility>
// Nanairo
#include "geometry.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"

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
  Please see the detail of this algorithm the pdf
  Entitled "Ray tracing simulation method using piecewise quadratic interpolant
  for aspheric optical systems"
  */
class SmoothedMesh : public TriangleMesh 
{
 public:
  //! Initialize smoothed mesh
  SmoothedMesh(const Point3& vertex0, 
               const Point3& vertex1, 
               const Point3& vertex2,
               const Vector3& normal0, 
               const Vector3& normal1,
               const Vector3& normal2) noexcept;


  //! Return the bounding box
  Aabb boundingBox() const noexcept override;

  //! Return the smoothd mesh size
  std::size_t geometrySize() const noexcept override;

  //! Return the cost of a ray-patch intersection test
  Float getTraversalCost() const noexcept override;

  //! Test ray-mesh intersection
  bool testIntersection(const Ray& ray, 
                        IntersectionInfo* intersection) const noexcept override;

  //! Sample a point randomly on the surface of the mesh
  std::tuple<SampledPoint, Vector3> samplePoint(Sampler& sampler) const noexcept override;

  //! Apply affine transformation
  void transform(const Matrix4x4& matrix) noexcept override;

 private:
  // Calculate the bounding box
  Aabb calcBoundingBox(const Vector3* distance, const Vector3* curvature) const noexcept;

  //! Initialize
  void initialize(const Point3& vertex0,
                  const Point3& vertex1,
                  const Point3& vertex2,
                  const Vector3& normal0,
                  const Vector3& normal1,
                  const Vector3& normal2) noexcept;

  //! Return the normal
  Vector3 normal(const double eta, const double xi) const noexcept;

  //! Return the surface point
  Point3 point(const double eta, const double xi) const noexcept;


  Vector3 c_[6]; //!< Vector coefficients of the patch
};

//! \} Core

} // namespace nanairo

#include "smoothed_mesh-inl.hpp"

#endif // NANAIRO_SMOOTHED_MESH_HPP
