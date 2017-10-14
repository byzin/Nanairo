/*!
  \file ray-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RAY_INL_HPP
#define NANAIRO_RAY_INL_HPP

#include "ray.hpp"
// Standard C++ library
#include <array>
#include <limits>
// Zisc
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Ray::Ray() noexcept :
    is_alive_{kFalse}
{
  initialize();
}

/*!
  \details
  No detailed.
  */
inline
Ray::Ray(const Point3& origin, const Vector3& direction) noexcept :
    origin_{origin},
    direction_{direction},
    is_alive_{kTrue}
{
  initialize();
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& Ray::direction() const noexcept
{
  return direction_;
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& Ray::invDirection() const noexcept
{
  return inv_direction_;
}

/*!
  \details
  No detailed.
  */
inline
bool Ray::isAlive() const noexcept
{
  return is_alive_ == kTrue;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& Ray::origin() const noexcept
{
  return origin_;
}

/*!
  \details
  No detailed.
  */
inline
void Ray::setDirection(const Vector3& direction) noexcept
{
  direction_ = direction;
  initInvDirection();
}

/*!
  \details
  No detailed.
  */
inline
void Ray::setOrigin(const Point3& origin) noexcept
{
  origin_ = origin;
}

/*!
  \details
  No detailed.
  */
inline
void Ray::setAlive(const bool is_alive) noexcept
{
  is_alive_ = is_alive ? kTrue : kFalse;
}

/*!
  \details
  No detailed.
  */
inline
std::array<uint8, 3> Ray::sign() const noexcept
{
  return sign_;
}

/*!
  */
inline
void Ray::initialize() noexcept
{
  initInvDirection();
  // Avoid warnings
  static_cast<void>(padding_);
}

/*!
  */
inline
void Ray::initInvDirection() noexcept
{
  inv_direction_ = invert(direction());
  // Set the sign of the direction
  static_assert(kTrue == 1, "");
  static_assert(kFalse == 0, "");
  sign_[0] = (inv_direction_[0] < 0.0) ? kTrue : kFalse;
  sign_[1] = (inv_direction_[1] < 0.0) ? kTrue : kFalse;
  sign_[2] = (inv_direction_[2] < 0.0) ? kTrue : kFalse;
}

} // namespace nanairo

#endif // NANAIRO_RAY_INL_HPP
