/*!
  \file aabb-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_AABB_INL_HPP
#define NANAIRO_AABB_INL_HPP

#include "aabb.hpp"
// Zisc
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_test_result.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Point3 Aabb::centroid() const noexcept
{
  return point_[0] + 0.5 * (point_[1] - point_[0]);
}

/*!
  \details
  Please see the details of this algorithm below URL.
  http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
  */
inline
IntersectionTestResult Aabb::testIntersection(const Ray& ray) const noexcept
{
  const auto& origin = ray.origin();
  const auto& inv_dir = ray.invDirection();
  const auto s = ray.sign();

  Float tmin = 0.0;
  {
    const Point3 min_p{point_[s[0]][0], point_[s[1]][1], point_[s[2]][2]};
    auto t0 = (min_p - origin).data() * inv_dir.data();
    tmin = zisc::max(zisc::max(t0[0], t0[1]), t0[2]);
  }
  Float tmax = 0.0;
  {
    const Point3 max_p{point_[1-s[0]][0], point_[1-s[1]][1], point_[1-s[2]][2]};
    auto t1 = (max_p - origin).data() * inv_dir.data();
    tmax = zisc::min(zisc::min(t1[0], t1[1]), t1[2]);
  }

  const auto result = (tmin <= tmax) ? IntersectionTestResult{tmin}
                                     : IntersectionTestResult{};
  return result;
}

/*!
  \details
  No detailed.
  */
inline
uint Aabb::longestAxis() const noexcept
{
  constexpr uint x = 0;
  constexpr uint y = 1;
  constexpr uint z = 2;
  const auto range = maxPoint() - minPoint();
  return (range[y] < range[x])
    ? (range[z] < range[x])
      ? x
      : z
    : (range[z] < range[y])
      ? y
      : z;
}
 
/*!
 \details
 No detailed.
 */
inline
const Point3& Aabb::maxPoint() const noexcept
{
  return point_[1];
}

/*!
 \details
 No detailed.
 */
inline
const Point3& Aabb::minPoint() const noexcept
{
  return point_[0];
}

/*!
  \details
  No detailed.
  */
inline
Float Aabb::surfaceArea() const noexcept
{
  const auto range = maxPoint() - minPoint();
  return 2.0 * (range[0] * range[1] + range[2] * range[1] + range[0] * range[2]);
}

/*!
  \details
  No detailed.
  */
inline
Aabb combine(const Aabb& a, const Aabb& b) noexcept
{
  const auto min_point = zisc::minElements(a.minPoint().data(), b.minPoint().data());
  const auto max_point = zisc::maxElements(a.maxPoint().data(), b.maxPoint().data());
  return Aabb{Point3{min_point}, Point3{max_point}};
}

} // namespace nanairo

#endif // NANAIRO_AABB_INL_HPP
