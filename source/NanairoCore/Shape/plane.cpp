/*!
  \file plane.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "plane.hpp"
// Standard C++ library
#include <array>
#include <utility>
// Zisc
#include "zisc/arith_array.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "shape.hpp"
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

namespace nanairo {

/*!
 \details
 No detailed.
 */
Plane::Plane() noexcept :
    vertex0_{-0.5, -0.5, 0.0},
    edge_{{Vector3{1.0, 0.0, 0.0}, Vector3{0.0, 1.0, 0.0}}}
{
  initialize();
}

/*!
 \details
 No detailed.
 */
Aabb Plane::boundingBox() const noexcept
{
  const auto& v = vertex0();
  const auto& e = edge();

  auto min_point = v.data();
  auto max_point = v.data();
  Point3 plane_points[] = {v + e[0], v + e[1], v + e[0] + e[1]};
  for (const auto& point : plane_points) {
    min_point = zisc::minElements(min_point, point.data());
    max_point = zisc::maxElements(max_point, point.data());
  }
  return Aabb{Point3{min_point}, Point3{max_point}};
}

/*!
  */
ShapePoint Plane::getPoint(const Point2& st) const noexcept
{
  const auto& v = vertex0();
  const auto& e = edge();
  const auto point = v + st[0] * e[0] + st[1] * e[1];

  const auto tangents = Transformation::calcDefaultTangent(normal());
  const auto& tangent = std::get<0>(tangents);
  const auto& bitangent = std::get<1>(tangents);

  return ShapePoint{SampledPoint{point, surfaceArea()},
                    normal(),
                    tangent,
                    bitangent,
                    st,
                    st};
}

/*!
  \details
  No detailed.
  */
Float Plane::getTraversalCost() const noexcept
{
  return 1.0;
}

/*!
 */
IntersectionTestResult Plane::testIntersection(
    const Ray& ray,
    IntersectionInfo* intersection) const noexcept
{
  const bool is_hit = testIntersection(vertex0(),
                                       edge(),
                                       normal(),
                                       ray,
                                       intersection->rayDistance(),
                                       nullptr,
                                       intersection);
  return (is_hit)
      ? IntersectionTestResult{intersection->rayDistance()}
      : IntersectionTestResult{};
}

/*!
 \details
  Please see the details of this algorithm below RUL.
  http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-plane-and-ray-disk-intersection/
  */
bool Plane::testIntersection(const Point3& v,
                             const std::array<Vector3, 2>& e,
                             const Vector3& normal,
                             const Ray& ray,
                             const Float max_distance,
                             Point2* st,
                             IntersectionInfo* intersection) noexcept
{
  const Float cos_theta = -zisc::dot(normal, ray.direction());
  // In the case that the ray is parallel to the normal
  if (cos_theta == 0.0)
    return false;
  // Calculate the time that ray hit plane
  const Float t = zisc::dot(normal, ray.origin() - v) / cos_theta;
  if (!zisc::isInOpenBounds(t, 0.0, max_distance))
    return false;
  // Check if the hit point is in the plane
  const auto point = ray.origin() + (t * ray.direction());
  const auto am = point - v;
  const Float x = zisc::dot(am, e[0]);
  const Float y = zisc::dot(am, e[1]);
  const bool is_hit = zisc::isInClosedBounds(x, 0.0, e[0].squareNorm()) &&
                      zisc::isInClosedBounds(y, 0.0, e[1].squareNorm());
  if (is_hit) {
    const Point2 st_coordinate{x / e[0].squareNorm(), y / e[1].squareNorm()};
    if (st != nullptr)
      *st = st_coordinate;
    if (intersection != nullptr) {
      const bool is_back_face = cos_theta < 0.0;

      const auto n = (!is_back_face) ? normal : -normal;
      const auto tangents = Transformation::calcDefaultTangent(n);
      const auto& tangent = std::get<0>(tangents);
      const auto& bitangent = std::get<1>(tangents);

      intersection->setPoint(point);
      intersection->setNormal(n);
      intersection->setTangent(tangent);
      intersection->setBitangent(bitangent);
      intersection->setAsBackFace(is_back_face);
      intersection->setRayDistance(t);
      intersection->setSt(st_coordinate);
      intersection->setUv(st_coordinate);
    }
  }
  return is_hit;
}

/*!
  \details
  No detailed.
  */
ShapePoint Plane::samplePoint(Sampler& sampler) const noexcept
{
  const auto& v = vertex0();
  const auto& e = edge();

  const Point2 st{sampler.sample(),
                  sampler.sample()};
  const auto point = v + st[0] * e[0] + st[1] * e[1];

  const auto tangents = Transformation::calcDefaultTangent(normal());
  const auto& tangent = std::get<0>(tangents);
  const auto& bitangent = std::get<1>(tangents);

  return ShapePoint{SampledPoint{point, surfaceArea()},
                    normal(),
                    tangent,
                    bitangent,
                    st,
                    st};
}

/*!
  */
Vector3 Plane::calcNormal() const noexcept
{
  const auto& e = edge();
  const auto n = cross(e[0], e[1]).normalized();
  ZISC_ASSERT(isUnitVector(n), "Normal isn't unit vector.");
  return n;
}

/*!
  */
Float Plane::calcSurfaceArea() const noexcept
{
  const auto& e = edge();
  const Float surface_area = zisc::cross(e[0], e[1]).norm();
  return surface_area;
}

/*!
  \details
  No detailed.
  */
void Plane::initialize() noexcept
{
  normal_ = calcNormal();
  setSurfaceArea(calcSurfaceArea());
}

/*!
 \details
 No detailed.
 */
void Plane::transformShape(const Matrix4x4& matrix) noexcept
{
  Transformation::affineTransform(matrix, &vertex0_);
  Transformation::affineTransform(matrix, &edge_[0]);
  Transformation::affineTransform(matrix, &edge_[1]);
  normal_ = calcNormal();
}

} // namespace nanairo
