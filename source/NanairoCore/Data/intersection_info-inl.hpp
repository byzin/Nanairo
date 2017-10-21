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
#include <array>
#include <limits>
// Nanairo
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
IntersectionInfo::IntersectionInfo() noexcept :
    object_{nullptr},
    ray_distance_{0.0},
    is_back_face_{kFalse}
{
  // Avoid warnings
  static_cast<void>(padding_);
}

/*!
  */
inline
IntersectionInfo::IntersectionInfo(const Object* object,
                                   const ShapePoint& point) noexcept :
    object_{object},
    point_{point},
    ray_distance_{0.0},
    is_back_face_{kFalse}
{
}

/*!
  */
inline
const Vector3& IntersectionInfo::bitangent() const noexcept
{
  return point_.bitangent();
}

/*!
  */
inline
Vector3 IntersectionInfo::frontNormal() const noexcept
{
  const auto n = (isBackFace()) ? -normal() : normal();
  return n;
}

/*!
  \details
  No detailed.
  */
inline
bool IntersectionInfo::isBackFace() const noexcept
{
  const bool is_back_face = (is_back_face_ == kTrue);
  return is_back_face;
}

/*!
  \details
  No detailed.
  */
inline
bool IntersectionInfo::isIntersected() const noexcept
{
  const bool is_intersected = object_ != nullptr;
  return is_intersected;
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& IntersectionInfo::normal() const noexcept
{
  return point_.normal();
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
  return point_.point();
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
void IntersectionInfo::setAsBackFace(const bool is_back_face) noexcept
{
  is_back_face_ = (is_back_face) ? kTrue : kFalse;
}

/*!
  */
inline
void IntersectionInfo::setBitangent(const Vector3& bitangent) noexcept
{
  point_.setBitangent(bitangent);
}

/*!
  \details
  No detailed.
  */
inline
void IntersectionInfo::setNormal(const Vector3& normal) noexcept
{
  point_.setNormal(normal);
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
  point_.setPoint(point);
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
void IntersectionInfo::setShapePoint(const ShapePoint& point) noexcept
{
  point_ = point;
}

/*!
  */
inline
void IntersectionInfo::setSt(const Point2& st) noexcept
{
  point_.setSt(st);
}

/*!
  */
inline
void IntersectionInfo::setTangent(const Vector3& tangent) noexcept
{
  point_.setTangent(tangent);
}

/*!
  \details
  No detailed.
  */
inline
void IntersectionInfo::setUv(const Point2& uv) noexcept
{
  point_.setUv(uv);
}

/*!
  */
inline
const ShapePoint& IntersectionInfo::shapePoint() const noexcept
{
  return point_;
}

/*!
  */
inline
const Point2& IntersectionInfo::st() const noexcept
{
  return point_.st();
}

/*!
  */
inline
const Vector3& IntersectionInfo::tangent() const noexcept
{
  return point_.tangent();
}

/*!
  \details
  No detailed.
  */
inline
const Point2& IntersectionInfo::uv() const noexcept
{
  return point_.uv();
}

} // namespace nanairo

#endif // NANAIRO_INTERSECTION_INFO_INL_HPP
