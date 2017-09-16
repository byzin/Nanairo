/*!
  \file intersection_info-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_INTERSECTION_INFO_INL_HPP
#define NANAIRO_INTERSECTION_INFO_INL_HPP

#include "intersection_info.hpp"
// Standard C++ library
#include <limits>
// Nanairo
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
IntersectionInfo::IntersectionInfo() noexcept :
    object_{nullptr},
    is_reverse_face_{false}
{
}

/*!
  \details
  No detailed.
  */
inline
IntersectionInfo::IntersectionInfo(const Point3& point,
                                   const Vector3& normal,
                                   const Object* object,
                                   const bool is_reverse_face) noexcept :
    point_{point},
    normal_{is_reverse_face ? -normal : normal},
    object_{object},
    ray_distance_{0.0},
    is_reverse_face_{is_reverse_face}
{
}

/*!
  \details
  No detailed.
  */
inline
IntersectionInfo::IntersectionInfo(const Point3& point,
                                   const Vector3& normal,
                                   const Point2& texture_coordinate,
                                   const Object* object,
                                   const bool is_reverse_face) noexcept :
    point_{point},
    normal_{is_reverse_face ? -normal : normal},
    texture_coordinate_{texture_coordinate},
    object_{object},
    ray_distance_{0.0},
    is_reverse_face_{is_reverse_face}
{
}

/*!
  \details
  No detailed.
  */
inline
bool IntersectionInfo::isIntersected() const noexcept
{
  return object_ != nullptr;
}

/*!
  \details
  No detailed.
  */
inline
bool IntersectionInfo::isReverseFace() const noexcept
{
  return is_reverse_face_;
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& IntersectionInfo::normal() const noexcept
{
  return normal_;
}

/*!
  \details
  No detailed.
  */
inline
const Object* IntersectionInfo::object() const noexcept
{
  return object_;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& IntersectionInfo::point() const noexcept
{
  return point_;
}

/*!
  */
inline
Float IntersectionInfo::rayDistance() const noexcept
{
  return ray_distance_;
}

/*!
  \details
  No detailed.
  */
inline
void IntersectionInfo::setReverseFace(const bool is_reverse_face) noexcept
{
  is_reverse_face_ = is_reverse_face;
}

/*!
  \details
  No detailed.
  */
inline
void IntersectionInfo::setNormal(const Vector3& normal) noexcept
{
  normal_ = is_reverse_face_ ? -normal : normal;
}

/*!
  \details
  No detailed.
  */
inline
void IntersectionInfo::setObject(const Object* object) noexcept
{
  object_ = object;
}

/*!
  \details
  No detailed.
  */
inline
void IntersectionInfo::setPoint(const Point3& point) noexcept
{
  point_ = point;
}

/*!
  */
inline
void IntersectionInfo::setRayDistance(const Float distance) noexcept
{
  ray_distance_ = distance;
}

/*!
  */
inline
void IntersectionInfo::setSt(const Point2& st) noexcept
{
  st_coordinate_ = st;
}

/*!
  \details
  No detailed.
  */
inline
void IntersectionInfo::setTextureCoordinate(const Point2& texture_coordinate) noexcept
{
  texture_coordinate_ = texture_coordinate;
}

/*!
  */
inline
const Point2& IntersectionInfo::st() const noexcept
{
  return st_coordinate_;
}

/*!
  \details
  No detailed.
  */
inline
const Point2& IntersectionInfo::textureCoordinate() const noexcept
{
  return texture_coordinate_;
}

} // namespace nanairo

#endif // NANAIRO_INTERSECTION_INFO_INL_HPP
