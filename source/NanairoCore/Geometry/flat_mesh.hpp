/*!
  \file flat_mesh.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_FLAT_MESH_HPP_
#define _NANAIRO_FLAT_MESH_HPP_

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
  No detailed.
  */
class FlatMesh : public TriangleMesh
{
 public:
  //! Create flat mesh
  FlatMesh(const Point3& vertex0, const Point3& vertex1, const Point3& vertex2);


  //! Return the bounding box
  Aabb boundingBox() const override;

  //! Return the flat mesh size
  std::size_t geometrySize() const override;

  //! Return the cost of a ray-triangle intersection test
  Float getTraversalCost() const override;

  //! Test ray-triangle intersection
  bool testIntersection(const Ray& ray, 
                        IntersectionInfo* intersection) const override;

  //! Sample a point randomly on the surface of the triangle
  std::tuple<SampledPoint, Vector3> samplePoint(Sampler& sampler) const override;

  //! Apply affine transformation
  void transform(const Matrix4x4& matrix) override;

 private:
  void initialize(const Point3& vertex1, const Point3& vertex2);

  //! Set normal vector
  void setNormal();


  Point3 vertex_;
  Vector3 edge_[2]; // v0 to v1 and v0 to v2 edge vectors
  Vector3 normal_;
};

//! \} Core

} // namespace nanairo

#endif // _NANAIRO_FLAT_MESH_HPP_
