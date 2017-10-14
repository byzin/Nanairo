/*!
  \file intersection_info.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_INTERSECTION_INFO_HPP
#define NANAIRO_INTERSECTION_INFO_HPP

// Standard C++ library
#include <array>
// Nanairo
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class Object;

//! \addtogroup Core
//! \{

/*!
  \brief The information of ray-geometry intersection point
  \details
  No detailed.
  */
class IntersectionInfo
{
 public:
  //! Create a empty data
  IntersectionInfo() noexcept;

  //! Create a intersection data
  IntersectionInfo(const Point3& point,
                   const Vector3& normal,
                   const Object* object,
                   const bool is_reverse_face) noexcept;

  //! Create a intersection data
  IntersectionInfo(const Point3& point,
                   const Vector3& normal,
                   const Point2& texture_coordinate,
                   const Object* object,
                   const bool is_reverse_face) noexcept;


  //! Check if there is a intersected object
  bool isIntersected() const noexcept;

  //! Check if the intersection point is reverse face
  bool isReverseFace() const noexcept;

  //! Return the oriented normal direction
  const Vector3& normal() const noexcept;

  //! Return the intersected object
  const Object* object() const noexcept;

  //! Return the intersection point
  const Point3& point() const noexcept;

  //! Return the ray distance from the origin
  Float rayDistance() const noexcept;

  //! Set reverse face
  void setReverseFace(const bool is_reverse_face) noexcept;

  //! Set normal direction
  void setNormal(const Vector3& normal) noexcept;

  //! Set intersected object
  void setObject(const Object* object) noexcept;

  //! Set intersection point
  void setPoint(const Point3& point) noexcept;

  //! Set the ray distance from the origin
  void setRayDistance(const Float distance) noexcept;

  //! Set the st coordinate
  void setSt(const Point2& st) noexcept;

  //! Set texture coordinate
  void setTextureCoordinate(const Point2& texture_coordinate) noexcept;

  //! Return the st coordinate
  const Point2& st() const noexcept;

  //! Return the texture coordinate
  const Point2& textureCoordinate() const noexcept;

 private:
  Point3 point_;
  Vector3 normal_;
  Point2 texture_coordinate_;
  Point2 st_coordinate_;
  const Object* object_;
  Float ray_distance_;
  uint8 is_reverse_face_;
  std::array<uint8, 7> padding_;
};

//! \} Core

} // namespace nanairo

#include "intersection_info-inl.hpp"

#endif // NANAIRO_INTERSECTION_INFO_HPP
