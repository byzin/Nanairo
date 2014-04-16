/*!
  \file point.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_POINT_HPP_
#define _NANAIRO_POINT_HPP_

// Zisc
#include "zisc/point.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core 
//! \{

using Point2 = zisc::Point<Float, 2>;
using Point3 = zisc::Point<Float, 3>;
using Point4 = zisc::Point<Float, 4>;

//! Make 4d point from 3d point
Point4 makePoint4(const Point3& point, const Float value);

//! Take 3d point from 4d point
Point3 takePoint3(const Point4& point);

//! \} Core 

} // namespace nanairo

#include "point-inl.hpp"

#endif // _NANAIRO_POINT_HPP_
