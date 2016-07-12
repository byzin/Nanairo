/*!
  \file unit-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_UNIT_INL_HPP
#define ZISC_UNIT_INL_HPP

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
constexpr Float toRadian(const Float angle) noexcept
{
  static_assert(std::is_floating_point<Float>::value,
                "Float isn't floating point type.");
  constexpr auto k = cast<Float>(kPi<long double> / 180.0l);
  return k * angle;
}

/*!
  \details
  No detailed.

  \param[in] radian radian
  \return degree
  */
template <typename Float> inline
constexpr Float toAngle(const Float radian) noexcept
{
  static_assert(std::is_floating_point<Float>::value,
                "Float isn't floating point type.");
  constexpr auto k = cast<Float>(180.0l / kPi<long double>);
  return k * radian;
}

/*!
  \details
  No detailed.
  */
constexpr double toMegaByte(const std::size_t byte) noexcept
{
  return cast<double>(byte) * (1.0 / zisc::power<2>(1024.0));
}

} // namespace zisc

#endif // ZISC_UNIT_INL_HPP
