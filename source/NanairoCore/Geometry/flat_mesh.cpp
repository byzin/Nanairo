/*!
  \file flat_mesh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "flat_mesh.hpp"
// Standard C++ library
#include <cstddef>
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "geometry.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/DataStructure/aabb.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo  {

/*!
  \details
  No detailed.
  */
FlatMesh::FlatMesh(const Point3& vertex0,
                   const Point3& vertex1,
                   const Point3& vertex2) noexcept :
    vertex_{vertex0}
{
  initialize(vertex1, vertex2);
}

/*!
  \details
  No detailed.

  \return AABB
  */
Aabb FlatMesh::boundingBox() const noexcept
{
  auto min_point = vertex_.data();
  auto max_point = vertex_.data();
  Point3 mesh_points[] = {vertex_ + edge_[0], vertex_ + edge_[1]};
  for (const auto& point : mesh_points) {
    min_point = zisc::minElements(min_point, point.data());
    max_point = zisc::maxElements(max_point, point.data());
  }
  return Aabb{Point3{min_point}, Point3{max_point}};
}

/*!
  \details
  No detailed.
  */
std::size_t FlatMesh::geometrySize() const noexcept
{
  return sizeof(FlatMesh);
}

/*!
  \details
  No detailed.
  */
Float FlatMesh::getTraversalCost() const noexcept
{
  return 1.0;
}

/*!
  \details
  Please see the details of this algorithm below URL.
  http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-9-ray-triangle-intersection/m-ller-trumbore-algorithm/
  */
bool FlatMesh::testIntersection(const Ray& ray, IntersectionInfo* intersection) const noexcept
{
  Float t;
  Float barycentric[3];
  if (!calculateBarycentricCoordinate(ray, vertex_, edge_, barycentric, &t))
    return false;

  // Set point
  intersection->setPoint(ray.origin() + t * ray.direction());

  // Set normal
  const Float cos_theta = zisc::dot(normal_, ray.direction());
  intersection->setReverseFace(cos_theta > 0.0);
  intersection->setNormal(normal_);

  // Set texture coordinate
  intersection->setTextureCoordinate(textureCoordinate(barycentric));

  return true;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledPoint, Vector3, Point2> FlatMesh::samplePoint(Sampler& sampler) const noexcept
{
  Float u = sampler.sample(0.0, 1.0);
  Float v = sampler.sample(0.0, 1.0);
  if (u + v > 1.0) {
    u = 1.0 - u;
    v = 1.0 - v;
  }

  const auto point = vertex_ + u * edge_[0] + v * edge_[1];
  const Float barycentric[3] = {u, v, 1.0 - (u + v)};
  return std::make_tuple(SampledPoint{point, surfaceArea()},
                         normal_,
                         textureCoordinate(barycentric));
}

/*!
  \details
  No detailed.
  */
void FlatMesh::transform(const Matrix4x4& matrix) noexcept
{
  affineTransform(matrix, &vertex_);
  affineTransform(matrix, &edge_[0]);
  affineTransform(matrix, &edge_[1]);
  affineTransform(matrix, &normal_);
  normal_ = normal_.normalized();

  const auto area = 0.5 * zisc::cross(edge_[0], edge_[1]).norm();
  setSurfaceArea(area);
}

// private member function

/*!
  \details
  No detailed.
  */
void FlatMesh::initialize(const Point3& vertex1, const Point3& vertex2) noexcept
{
  edge_[0] = vertex1 - vertex_;
  edge_[1] = vertex2 - vertex_;
  setNormal();
}

/*!
  \details
  No detailed.
  */
void FlatMesh::setNormal() noexcept
{
  normal_ = (zisc::cross(edge_[0], edge_[1])).normalized();
  ZISC_ASSERT(isUnitVector(normal_), "Normal must be unit vector.");
}

} // namespace nanairo
