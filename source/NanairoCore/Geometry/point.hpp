/*!
  \file point.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_POINT_HPP
#define NANAIRO_POINT_HPP

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
Point4 makePoint4(const Point3& point, const Float value) noexcept;

//! Take 3d point from 4d point
Point3 takePoint3(const Point4& point) noexcept;

//! \} Core 

} // namespace nanairo

#include "point-inl.hpp"

#endif // NANAIRO_POINT_HPP
