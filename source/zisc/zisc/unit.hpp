/*!
  \file unit.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_UNIT_HPP_
#define _ZISC_UNIT_HPP_

// Standard C++ library
#include <cstddef>

namespace zisc {

//! Convert degree to radian
template <typename Float>
constexpr Float toRadian(const Float angle);

//! Convert radian to degree
template <typename Float>
constexpr Float toAngle(const Float radian);

//! Return the Mega byte size
constexpr double toMegaByte(const std::size_t byte);

} // namespace zisc

#include "unit-inl.hpp"

#endif // _ZISC_UNIT_HPP_
