/*!
  \file flat_triangle.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "flat_triangle.hpp"
// Standard C++ library
#include <array>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/matrix.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "shape.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/intersection_test_result.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Data/shape_point.hpp"
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
FlatTriangle::FlatTriangle(const Point3& vertex1,
                           const Point3& vertex2,
                           const Point3& vertex3) noexcept :
    vertex0_{vertex1},
    edge_{{Vector3{vertex2 - vertex1}, Vector3{vertex3 - vertex1}}},
    uv0_{0.0, 0.0},
    uv_edge_{{Vector2{0.0, 0.0}, Vector2{0.0, 0.0}}}
{
  initialize();
}

/*!
  \details
  No detailed.

  \return AABB
  */
Aabb FlatTriangle::boundingBox() const noexcept
{
  const auto& v = vertex0();
  const auto& e = edge();

  auto min_point = v.data();
  auto max_point = v.data();
  Point3 points[] = {v + e[0], v + e[1]};
  for (const auto& point : points) {
    min_point = zisc::minElements(min_point, point.data());
    max_point = zisc::maxElements(max_point, point.data());
  }
  return Aabb{Point3{min_point}, Point3{max_point}};
}

/*!
  */
ShapePoint FlatTriangle::getPoint(const Point2& st) const noexcept
{
  const auto& v = vertex0();
  const auto& e = edge();

  const auto point = v + st[0] * e[0] + st[1] * e[1];
  const auto uv = calcUv(st);

  const auto tangents = Transformation::calcDefaultTangent(normal());
  const auto& tangent = std::get<0>(tangents);
  const auto& bitangent = std::get<1>(tangents);

  return ShapePoint{SampledPoint{point, surfaceArea()},
                    normal(),
                    tangent,
                    bitangent,
                    uv,
                    st};
}

/*!
  \details
  No detailed.
  */
Float FlatTriangle::getTraversalCost() const noexcept
{
  return 1.0;
}

/*!
  \details
  Please see "Fast Ray-Triangle Intersections by Coordinate Transformation"
  */
IntersectionTestResult FlatTriangle::testIntersection(
    const Ray& ray,
    IntersectionInfo* intersection) const noexcept
{
  const auto& to_canonical = toCanonicalMatrix();

  const Float dz = zisc::dot(to_canonical.row1_xyz_, ray.direction());
  const Float oz = zisc::dot(to_canonical.row1_xyz_.data(), ray.origin().data()) +
                   to_canonical.row1_w_;
  if (dz == 0.0)
    return IntersectionTestResult{};

  const Float t = -oz / dz;
  if (!zisc::isInOpenBounds(t, 0.0, intersection->rayDistance()))
    return IntersectionTestResult{};

  const auto point = ray.origin() + t * ray.direction();
  const Point2 st{zisc::dot(to_canonical.row2_xyz_.data(), point.data()) +
                  to_canonical.row2_w_,
                  zisc::dot(to_canonical.row3_xyz_.data(), point.data()) +
                  to_canonical.row3_w_};
  const Float u = 1.0 - (st[0] + st[1]);
  const bool is_hit = (0.0 < st[0]) && (0.0 < st[1]) && (0.0 < u);
  if (is_hit) {
    const Float cos_theta = -zisc::dot(normal(), ray.direction());
    const bool is_back_face = cos_theta < 0.0;

    const auto n = (!is_back_face) ? normal() : -normal();
    const auto tangents = Transformation::calcDefaultTangent(n);
    const auto& tangent = std::get<0>(tangents);
    const auto& bitangent = std::get<1>(tangents);

    intersection->setPoint(point);
    intersection->setNormal(n);
    intersection->setTangent(tangent);
    intersection->setBitangent(bitangent);
    intersection->setAsBackFace(is_back_face);
    intersection->setRayDistance(t);
    intersection->setSt(st);
    intersection->setUv(calcUv(st));
  }
  return (is_hit)
      ? IntersectionTestResult{t}
      : IntersectionTestResult{};
}

/*!
  \details
  No detailed.
  */
ShapePoint FlatTriangle::samplePoint(Sampler& sampler) const noexcept
{
  const auto& v = vertex0();
  const auto& e = edge();

  Point2 st{sampler.sample(), sampler.sample()};
  if (1.0 < (st[0] + st[1])) {
    st[0] = 1.0 - st[0];
    st[1] = 1.0 - st[1];
  }
  const auto point = v + st[0] * e[0] + st[1] * e[1];
  const auto uv = calcUv(st);

  const auto tangents = Transformation::calcDefaultTangent(normal());
  const auto& tangent = std::get<0>(tangents);
  const auto& bitangent = std::get<1>(tangents);

  return ShapePoint{SampledPoint{point, surfaceArea()},
                    normal(),
                    tangent,
                    bitangent,
                    uv,
                    st};
}

/*!
  */
void FlatTriangle::setUv(const Point2& uv1,
                         const Point2& uv2,
                         const Point2& uv3) noexcept
{
  uv0_ = uv1;
  uv_edge_[0] = uv2 - uv1;
  uv_edge_[1] = uv3 - uv1;
}

// private member function

/*!
  \details
  No detailed.
  */
Vector3 FlatTriangle::calcNormal() const noexcept
{
  const auto n = zisc::cross(edge_[0], edge_[1]).normalized();
  ZISC_ASSERT(isUnitVector(n), "The normal isn't unit vector.");
  return n;
}

/*!
  */
Float FlatTriangle::calcSurfaceArea() const noexcept
{
  const Float area = 0.5 * zisc::cross(edge_[0], edge_[1]).norm();
  return area;
}

/*!
  */
void FlatTriangle::initCanonicalMatrix() noexcept
{
  const auto& e1 = edge()[0];
  const auto& e2 = edge()[1];
  const Vector3 v1 = *zisc::treatAs<const Vector3*>(&vertex0());
  const Vector3 v2 = v1 + e1;
  const Vector3 v3 = v1 + e2;
  const Vector3& n = zisc::cross(e1, e2);

  using zisc::abs;
  if ((abs(n[1]) < abs(n[0])) && (abs(n[2]) < abs(n[0]))) {
    to_canonical_.row1_xyz_[0] = 1.0;
    to_canonical_.row1_xyz_[1] = n[1] / n[0];
    to_canonical_.row1_xyz_[2] = n[2] / n[0];
    to_canonical_.row1_w_ = -zisc::dot(v1, n) / n[0];

    to_canonical_.row2_xyz_[0] = 0.0;
    to_canonical_.row2_xyz_[1] = e2[2] / n[0];
    to_canonical_.row2_xyz_[2] = -e2[1] / n[0];
    to_canonical_.row2_w_ = zisc::cross(v3, v1)[0] / n[0];

    to_canonical_.row3_xyz_[0] = 0.0;
    to_canonical_.row3_xyz_[1] = -e1[2] / n[0];
    to_canonical_.row3_xyz_[2] = e1[1] / n[0];
    to_canonical_.row3_w_ = -zisc::cross(v2, v1)[0] / n[0];
  }
  else if (abs(n[2]) < abs(n[1])) {
    to_canonical_.row1_xyz_[0] = n[0] / n[1];
    to_canonical_.row1_xyz_[1] = 1.0;
    to_canonical_.row1_xyz_[2] = n[2] / n[1];
    to_canonical_.row1_w_ = -zisc::dot(v1, n) / n[1];

    to_canonical_.row2_xyz_[0] = -e2[2] / n[1];
    to_canonical_.row2_xyz_[1] = 0.0;
    to_canonical_.row2_xyz_[2] = e2[0] / n[1];
    to_canonical_.row2_w_ = zisc::cross(v3, v1)[1] / n[1];

    to_canonical_.row3_xyz_[0] = e1[2] / n[1];
    to_canonical_.row3_xyz_[1] = 0.0;
    to_canonical_.row3_xyz_[2] = -e1[0] / n[1];
    to_canonical_.row3_w_ = -zisc::cross(v2, v1)[1] / n[1];
  }
  else if (0.0 < abs(n[2])) {
    to_canonical_.row1_xyz_[0] = n[0] / n[2];
    to_canonical_.row1_xyz_[1] = n[1] / n[2];
    to_canonical_.row1_xyz_[2] = 1.0;
    to_canonical_.row1_w_ = -zisc::dot(v1, n) / n[2];

    to_canonical_.row2_xyz_[0] = e2[1] / n[2];
    to_canonical_.row2_xyz_[1] = -e2[0] / n[2];
    to_canonical_.row2_xyz_[2] = 0.0;
    to_canonical_.row2_w_ = zisc::cross(v3, v1)[2] / n[2];

    to_canonical_.row3_xyz_[0] = -e1[1] / n[2];
    to_canonical_.row3_xyz_[1] = e1[0] / n[2];
    to_canonical_.row3_xyz_[2] = 0.0;
    to_canonical_.row3_w_ = -zisc::cross(v2, v1)[2] / n[2];
  }
  else {
    zisc::raiseError("Making world-to-canonical matrix failed.");
  }
}

/*!
  \details
  No detailed.
  */
void FlatTriangle::initialize() noexcept
{
  normal_ = calcNormal();
  initCanonicalMatrix();
  setSurfaceArea(calcSurfaceArea());
}

/*!
  \details
  No detailed.
  */
void FlatTriangle::transformShape(const Matrix4x4& matrix) noexcept
{
  Transformation::affineTransform(matrix, &vertex0_);
  Transformation::affineTransform(matrix, &edge_[0]);
  Transformation::affineTransform(matrix, &edge_[1]);
  normal_ = calcNormal();
  initCanonicalMatrix();
}


} // namespace nanairo
