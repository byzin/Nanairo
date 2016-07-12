/*!
  \file unit.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_UNIT_HPP
#define ZISC_UNIT_HPP

// Standard C++ library
#include <cstddef>

namespace zisc {

//! Convert degree to radian
template <typename Float>
constexpr Float toRadian(const Float angle) noexcept;

//! Convert radian to degree
template <typename Float>
constexpr Float toAngle(const Float radian) noexcept;

//! Return the Mega byte size
constexpr double toMegaByte(const std::size_t byte) noexcept;

} // namespace zisc

#include "unit-inl.hpp"

#endif // ZISC_UNIT_HPP
