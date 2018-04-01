/*!
  \file shape_point.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SHAPE_POINT_HPP
#define NANAIRO_SHAPE_POINT_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class ShapePoint
{
 public:
  //! Create uninitialized point
  ShapePoint() noexcept;

  //! Create a point data
  ShapePoint(const SampledPoint& point,
             const Vector3& normal,
             const Vector3& tangent,
             const Vector3& bitangent,
             const Point2& uv,
             const Point2& st) noexcept;


  //! Return the bitangent vector
  const Vector3& bitangent() const noexcept;

  //! Return the inverse pdf at the point
  Float inversePdf() const noexcept;

  //! Return the normal
  const Vector3& normal() const noexcept;

  //! Return the pdf at the point
  Float pdf() const noexcept;

  //! Return the point
  const Point3& point() const noexcept;

  //! Return the sampled point
  const SampledPoint& sampledPoint() const noexcept;

  //! Set the bitangent vector
  void setBitangent(const Vector3& bitangent) noexcept;

  //! Set the normal 
  void setNormal(const Vector3& normal) noexcept;

  //! Set the pdf at the point
  void setPdf(const Float pdf) noexcept;

  //! Set the point
  void setPoint(const Point3& point) noexcept;

  //! Set the sampled point
  void setSampledPoint(const SampledPoint& point) noexcept;

  //! Set the st coordinate
  void setSt(const Point2& st) noexcept;

  //! Set the tangent vector
  void setTangent(const Vector3& tangent) noexcept;

  //! Set the st coordinate
  void setUv(const Point2& uv) noexcept;

  //! Return the ST coordinate
  const Point2& st() const noexcept;

  //! Return the tangent vector
  const Vector3& tangent() const noexcept;

  //! Return the UV coordinate
  const Point2& uv() const noexcept;

 private:
  SampledPoint point_;
  Vector3 normal_;
  Vector3 tangent_;
  Vector3 bitangent_;
  Point2 uv_; //!< Texture coordinate
  Point2 st_; //!< Shape coordinate
};

//! \} Core

} // namespace nanairo

#include "shape_point-inl.hpp"

#endif // NANAIRO_SHAPE_POINT_HPP
