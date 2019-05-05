/*!
  \file intersection_info.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_INTERSECTION_INFO_HPP
#define NANAIRO_INTERSECTION_INFO_HPP

// Standard C++ library
#include <array>
// Nanairo
#include "NanairoCore/Data/shape_point.hpp"
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
  IntersectionInfo(const Object* object, const ShapePoint& point) noexcept;


  //! Return the bitangent vector
  const Vector3& bitangent() const noexcept;

  //! Return the face normal which is oriented to the front side
  Vector3 frontNormal() const noexcept;

  //! Check if the intersection point is back face
  bool isBackFace() const noexcept;

  //! Check if there is a intersected object
  bool isIntersected() const noexcept;

  //! Return the oriented normal direction
  const Vector3& normal() const noexcept;

  //! Return the intersected object
  const Object* object() const noexcept;

  //! Return the intersection point
  const Point3& point() const noexcept;

  //! Return the ray distance from the origin
  Float rayDistance() const noexcept;

  //! Set the intersection point to as back face
  void setAsBackFace(const bool is_back_face) noexcept;

  //! Set the bitangent vector
  void setBitangent(const Vector3& bitangent) noexcept;

  //! Set normal direction
  void setNormal(const Vector3& normal) noexcept;

  //! Set intersected object
  void setObject(const Object* object) noexcept;

  //! Set intersection point
  void setPoint(const Point3& point) noexcept;

  //! Set the ray distance from the origin
  void setRayDistance(const Float distance) noexcept;

  //! Set the shape point
  void setShapePoint(const ShapePoint& point) noexcept;

  //! Set the st coordinate
  void setSt(const Point2& st) noexcept;

  //! Set the tangent vector
  void setTangent(const Vector3& tangent) noexcept;

  //! Set texture coordinate
  void setUv(const Point2& uv) noexcept;

  //! Return the shape point
  const ShapePoint& shapePoint() const noexcept;

  //! Return the st coordinate
  const Point2& st() const noexcept;

  //! Return the tangent vector
  const Vector3& tangent() const noexcept;

  //! Return the texture coordinate
  const Point2& uv() const noexcept;

 private:
  const Object* object_;
  ShapePoint point_;
  Float ray_distance_;
  uint8 is_back_face_;
  std::array<uint8, 7> padding_;
};

//! \} Core

} // namespace nanairo

#include "intersection_info-inl.hpp"

#endif // NANAIRO_INTERSECTION_INFO_HPP
