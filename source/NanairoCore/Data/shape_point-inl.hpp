/*!
  \file shape_point-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SHAPE_POINT_INL_HPP
#define NANAIRO_SHAPE_POINT_INL_HPP

#include "shape_point.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"

namespace nanairo {

/*!
  */
inline
ShapePoint::ShapePoint() noexcept
{
}

/*!
  */
inline
ShapePoint::ShapePoint(const SampledPoint& point,
                       const Vector3& normal,
                       const Point2& uv,
                       const Point2& st) noexcept :
    point_{point},
    normal_{normal},
    uv_{uv},
    st_{st}
{
}

/*!
  */
inline
Float ShapePoint::inversePdf() const noexcept
{
  return point_.inversePdf();
}

/*!
  */
inline
const Vector3& ShapePoint::normal() const noexcept
{
  return normal_;
}

/*!
  */
inline
Float ShapePoint::pdf() const noexcept
{
  return point_.pdf();
}

/*!
  */
inline
const Point3& ShapePoint::point() const noexcept
{
  return point_.point();
}

/*!
  */
inline
const SampledPoint& ShapePoint::sampledPoint() const noexcept
{
  return point_;
}

/*!
  */
inline
void ShapePoint::setNormal(const Vector3& normal) noexcept
{
  normal_ = normal;
}

/*!
  */
inline
void ShapePoint::setPdf(const Float pdf) noexcept
{
  point_.setPdf(pdf);
}

/*!
  */
inline
void ShapePoint::setPoint(const Point3& point) noexcept
{
  point_.setPoint(point);
}

/*!
  */
inline
void ShapePoint::setSampledPoint(const SampledPoint& point) noexcept
{
  point_ = point;
}

/*!
  */
inline
void ShapePoint::setSt(const Point2& st) noexcept
{
  st_ = st;
}

/*!
  */
inline
void ShapePoint::setUv(const Point2& uv) noexcept
{
  uv_ = uv;
}

/*!
  */
inline
const Point2& ShapePoint::st() const noexcept
{
  return st_;
}

/*!
  */
inline
const Point2& ShapePoint::uv() const noexcept
{
  return uv_;
}

} // namespace nanairo

#endif // NANAIRO_SHAPE_POINT_INL_HPP
