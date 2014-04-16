/*!
  \file point-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_POINT_INL_HPP_
#define _NANAIRO_POINT_INL_HPP_

#include "point.hpp"
// Zisc
#include "zisc/point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Point4 makePoint4(const Point3& point, const Float value)
{
  return Point4{point[0], point[1], point[2], value};
}

/*!
  \details
  No detailed.
  */
inline
Point3 takePoint3(const Point4& point)
{
  return Point3{point[0], point[1], point[2]};
}

} // namespace nanairo

#endif // _NANAIRO_POINT_INL_HPP_
