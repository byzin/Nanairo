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
#include "shape.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/DataStructure/aabb.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo  {

/*!
  \details
  No detailed.
  */
FlatMesh::FlatMesh(const Point3& vertex1,
                   const Point3& vertex2,
                   const Point3& vertex3) noexcept :
    vertex_{vertex1}
{
  initialize(vertex2, vertex3);
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
Float FlatMesh::getTraversalCost() const noexcept
{
  return 1.0;
}

/*!
  \details
  Please see "Fast Ray-Triangle Intersections by Coordinate Transformation"
  */
bool FlatMesh::testIntersection(const Ray& ray,
                                IntersectionInfo* intersection) const noexcept
{
  const auto& ray_origin = ray.origin();
  const auto& ray_dir = ray.direction();

  const Float dz = to_local_(2, 0) * ray_dir[0] +
                   to_local_(2, 1) * ray_dir[1] +
                   to_local_(2, 2) * ray_dir[2];
  if (dz == 0.0)
    return false;

  const Float oz = to_local_(2, 0) * ray_origin[0] +
                   to_local_(2, 1) * ray_origin[1] +
                   to_local_(2, 2) * ray_origin[2] +
                   to_local_(2, 3);
  const Float t = -oz / dz;
  if (!zisc::isInOpenBounds(t, 0.0, intersection->rayDistance()))
    return false;

  const auto point = ray.origin() + t * ray.direction();
  const Float u = to_local_(0, 0) * point[0] +
                  to_local_(0, 1) * point[1] +
                  to_local_(0, 2) * point[2] +
                  to_local_(0, 3);
  const Float v = to_local_(1, 0) * point[0] +
                  to_local_(1, 1) * point[1] +
                  to_local_(1, 2) * point[2] +
                  to_local_(1, 3);
  const Float w = 1.0 - (u + v);
  const bool is_hit = (0.0 < u) && (0.0 < v) && (0.0 < w);
  if (is_hit) {
    // Set the intersection info
    intersection->setPoint(point);
    const Float cos_theta = zisc::dot(normal_, ray.direction());
    intersection->setReverseFace(0.0 < cos_theta);
    intersection->setNormal(normal_);
    intersection->setRayDistance(t);
    intersection->setTextureCoordinate(textureCoordinate(u, v));
  }
  return is_hit;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledPoint, Vector3, Point2> FlatMesh::samplePoint(Sampler& sampler) const noexcept
{
  Float u = sampler.sample();
  Float v = sampler.sample();
  if (1.0 < (u + v)) {
    u = 1.0 - u;
    v = 1.0 - v;
  }
  const auto point = vertex_ + u * edge_[0] + v * edge_[1];
  return std::make_tuple(SampledPoint{point, surfaceArea()},
                         normal_,
                         textureCoordinate(u, v));
}

/*!
  \details
  No detailed.
  */
void FlatMesh::transform(const Matrix4x4& matrix) noexcept
{
  Transformation::affineTransform(matrix, &vertex_);
  Transformation::affineTransform(matrix, &edge_[0]);
  Transformation::affineTransform(matrix, &edge_[1]);
  Transformation::affineTransform(matrix, &normal_);
  normal_ = normal_.normalized();

  const auto area = 0.5 * zisc::cross(edge_[0], edge_[1]).norm();
  setSurfaceArea(area);
  setToLocalMatrix();
}

// private member function

/*!
  \details
  No detailed.
  */
void FlatMesh::initialize(const Point3& vertex2, const Point3& vertex3) noexcept
{
  edge_[0] = vertex2 - vertex_;
  edge_[1] = vertex3 - vertex_;
  setNormal();
  setToLocalMatrix();
}

/*!
  \details
  No detailed.
  */
void FlatMesh::setNormal() noexcept
{
  normal_ = (zisc::cross(edge_[0], edge_[1])).normalized();
  ZISC_ASSERT(isUnitVector(normal_), "Normal isn't unit vector.");
}

/*!
  */
void FlatMesh::setToLocalMatrix() noexcept
{
  const auto& e1 = edge_[0];
  const auto& e2 = edge_[1];
  const Vector3 v1 = *zisc::treatAs<const Vector3*>(&vertex_);
  const Vector3 v2 = v1 + e1;
  const Vector3 v3 = v1 + e2;
  const Vector3 normal = zisc::cross(e1, e2);

  using zisc::abs;
  if ((abs(normal[1]) < abs(normal[0])) && (abs(normal[2]) < abs(normal[0]))) {
    to_local_(0, 0) = 0.0;
    to_local_(1, 0) = 0.0;
    to_local_(2, 0) = 1.0;
    to_local_(0, 1) = e2[2] / normal[0];
    to_local_(1, 1) = -e1[2] / normal[0];
    to_local_(2, 1) = normal[1] / normal[0];
    to_local_(0, 2) = -e2[1] / normal[0];
    to_local_(1, 2) = e1[1] / normal[0];
    to_local_(2, 2) = normal[2] / normal[0];
    to_local_(0, 3) = zisc::cross(v3, v1)[0] / normal[0];
    to_local_(1, 3) = -zisc::cross(v2, v1)[0] / normal[0];
    to_local_(2, 3) = -zisc::dot(v1, normal) / normal[0];
  }
  else if (abs(normal[2]) < abs(normal[1])) {
    to_local_(0, 0) = -e2[2] / normal[1];
    to_local_(1, 0) = e1[2] / normal[1];
    to_local_(2, 0) = normal[0] / normal[1];
    to_local_(0, 1) = 0.0;
    to_local_(1, 1) = 0.0;
    to_local_(2, 1) = 1.0;
    to_local_(0, 2) = e2[0] / normal[1];
    to_local_(1, 2) = -e1[0] / normal[1];
    to_local_(2, 2) = normal[2] / normal[1];
    to_local_(0, 3) = zisc::cross(v3, v1)[1] / normal[1];
    to_local_(1, 3) = -zisc::cross(v2, v1)[1] / normal[1];
    to_local_(2, 3) = -zisc::dot(v1, normal) / normal[1];
  }
  else if (0.0 < abs(normal[2])) {
    to_local_(0, 0) = e2[1] / normal[2];
    to_local_(1, 0) = -e1[1] / normal[2];
    to_local_(2, 0) = normal[0] / normal[2];
    to_local_(0, 1) = -e2[0] / normal[2];
    to_local_(1, 1) = e1[0] / normal[2];
    to_local_(2, 1) = normal[1] / normal[2];
    to_local_(0, 2) = 0.0;
    to_local_(1, 2) = 0.0;
    to_local_(2, 2) = 1.0;
    to_local_(0, 3) = zisc::cross(v3, v1)[2] / normal[2];
    to_local_(1, 3) = -zisc::cross(v2, v1)[2] / normal[2];
    to_local_(2, 3) = -zisc::dot(v1, normal) / normal[2];
  }
  else {
    zisc::raiseError("Making world-to-local matrix failed.");
  }
}

} // namespace nanairo
