/*!
  \file unit-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_UNIT_INL_HPP_
#define _ZISC_UNIT_INL_HPP_

#include "unit.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "math.hpp"
#include "utility.hpp"

namespace zisc {

/*!
  \details
  No detailed.

  \param[in] angle angle
  \return radian
  */
template <typename Float> inline
constexpr Float toRadian(const Float angle)
{
  static_assert(std::is_floating_point<Float>::value, "## Float must be float type.");
  return cast<Float>(kPi / 180.0) * angle;
}

/*!
  \details
  No detailed.

  \param[in] radian radian
  \return degree
  */
template <typename Float> inline
constexpr Float toAngle(const Float radian)
{
  static_assert(std::is_floating_point<Float>::value, "## Float must be float type.");
  return cast<Float>(180.0 / kPi) * radian;
}

/*!
  \details
  No detailed.
  */
constexpr double toMegaByte(const std::size_t byte)
{
  return cast<double>(byte) * (1.0 / (1024.0 * 1024.0));
}

} // namespace zisc

#endif // _ZISC_UNIT_INL_HPP_
