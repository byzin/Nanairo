/*!
  \file intersection_info.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_INTERSECTION_INFO_HPP
#define NANAIRO_INTERSECTION_INFO_HPP

// Nanairo
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

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

  //! Set reverse face
  void setReverseFace(const bool is_reverse_face) noexcept;
  
  //! Set normal direction
  void setNormal(const Vector3& normal) noexcept;

  //! Set intersected object
  void setObject(const Object* object) noexcept;

  //! Set intersection point
  void setPoint(const Point3& point) noexcept;

  //! Set texture coordinate
  void setTextureCoordinate(const Point2& texture_coordinate) noexcept;

  //! Return the texture coordinate
  const Point2& textureCoordinate() const noexcept;

 private:
  Point3 point_;
  Vector3 normal_;
  Point2 texture_coordinate_;
  const Object* object_;
  bool is_reverse_face_;
};

//! \} Core

} // namespace nanairo

#include "intersection_info-inl.hpp"

#endif // NANAIRO_INTERSECTION_INFO_HPP
