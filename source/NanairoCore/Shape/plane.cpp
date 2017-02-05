/*!
  \file plane.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "plane.hpp"
// Standard C++ library
#include <cstddef>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "shape.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"
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
    top_left_{-0.5, 0.0, -0.5},
    normal_{0.0, 1.0, 0.0},
    axis1_{1.0, 0.0, 0.0},
    axis2_{0.0, 0.0, 1.0},
    square_width_{1.0},
    square_height_{1.0}
{
  initialize();
}

/*!
 \details
 No detailed.
 */
Aabb Plane::boundingBox() const noexcept
{
  auto min_point = top_left_.data();
  auto max_point = top_left_.data();
  Point3 plane_points[] = {top_left_ + axis1_,
                           top_left_ + axis2_,
                           top_left_ + axis1_ + axis2_};
  for (const auto& point : plane_points) {
    min_point = zisc::minElements(min_point, point.data());
    max_point = zisc::maxElements(max_point, point.data());
  }
  return Aabb{Point3{min_point}, Point3{max_point}};
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
 \details
  Please see the details of this algorithm below RUL.
 http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-plane-and-ray-disk-intersection/
 */
bool Plane::testIntersection(const Ray& ray,
                             IntersectionInfo* intersection) const noexcept
{
  // Rayが平面と交差するまでの時間tを求めます
  const Float cos_theta = zisc::dot(normal_, ray.direction());
  // 平面の法線とレイの方向が垂直な関係にある場合は当たらない
  if (cos_theta == 0.0)
    return false;

  const Float t = zisc::dot(top_left_ - ray.origin(), normal_) / cos_theta;
  if (!zisc::isInOpenBounds(t, 0.0, intersection->rayDistance()))
    return false;

  //  交点が矩形の内側にあるか確認します
  const auto point = ray.origin() + (t * ray.direction());
  const auto am = point - top_left_;
  const Float dot_axis1_am = zisc::dot(am, axis1_);
  const Float dot_axis2_am = zisc::dot(am, axis2_);
  const bool is_hit = zisc::isInClosedBounds(dot_axis1_am, 0.0, square_width_) &&
                      zisc::isInClosedBounds(dot_axis2_am, 0.0, square_height_);
  if (is_hit) {
    // Set the intersection info
    intersection->setPoint(point);
    intersection->setReverseFace(0.0 < cos_theta);
    intersection->setNormal(normal_);
    intersection->setRayDistance(t);
    const Float u = dot_axis1_am * inverse_square_width_;
    const Float v = dot_axis2_am * inverse_square_height_;
    ZISC_ASSERT(zisc::isInClosedBounds(u, 0.0, 1.0),
                "Texture coordinate u is must be [0, 1].");
    ZISC_ASSERT(zisc::isInClosedBounds(v, 0.0, 1.0),
                "Texture coordinate v is must be [0, 1].");
    intersection->setTextureCoordinate(Point2{u, v});
  }
  return is_hit;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledPoint, Vector3, Point2> Plane::samplePoint(
    Sampler& sampler) const noexcept
{
  const Float u = sampler.sample();
  const Float v = sampler.sample();
  const auto point = top_left_ + u * axis1_ + v * axis2_;
  return std::make_tuple(SampledPoint{point, surfaceArea()}, normal_, Point2{u, v});
}

/*!
 \details
 No detailed.
 */
void Plane::transform(const Matrix4x4& matrix) noexcept
{
  Transformation::affineTransform(matrix, &top_left_);
  Transformation::affineTransform(matrix, &axis1_);
  Transformation::affineTransform(matrix, &axis2_);

  square_width_ = axis1_.squareNorm();
  square_height_ = axis2_.squareNorm();
  ZISC_ASSERT(0.0 < square_width_, "The width of the plane is minus.");
  ZISC_ASSERT(0.0 < square_height_, "The height of the plane is minus.");
  inverse_square_width_ = zisc::invert(square_width_);
  inverse_square_height_ = zisc::invert(square_height_);
  normal_ = cross(axis2_, axis1_).normalized();
  ZISC_ASSERT(isUnitVector(normal_), "Normal isn't unit vector.");

  const auto area = zisc::cross(axis1_, axis2_).norm();
  setSurfaceArea(area);
}

/*!
  \details
  No detailed.
  */
void Plane::initialize() noexcept
{
  setSurfaceArea(axis1_.norm() * axis2_.norm());
}

} // namespace nanairo
