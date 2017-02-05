/*!
  \file ray-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RAY_INL_HPP
#define NANAIRO_RAY_INL_HPP

#include "ray.hpp"
// Standard C++ library
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
    is_alive_{false}
{
}

/*!
  \details
  No detailed.
  */
inline
Ray::Ray(const Point3& origin, const Vector3& direction) noexcept :
    origin_{origin},
    direction_{direction},
    is_alive_{true}
{
  setInverseDirection();
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
const Vector3& Ray::inverseDirection() const noexcept
{
  return inverse_direction_;
}

/*!
  \details
  No detailed.
  */
inline
bool Ray::isAlive() const noexcept
{
  return is_alive_;
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
  setInverseDirection();
}

/*!
  \details
  No detailed.
  */
inline
void Ray::setInverseDirection() noexcept
{
  constexpr Float max = std::numeric_limits<Float>::max();
  inverse_direction_[0] = (direction_[0] != 0.0) ? zisc::invert(direction_[0]) : max;
  inverse_direction_[1] = (direction_[1] != 0.0) ? zisc::invert(direction_[1]) : max;
  inverse_direction_[2] = (direction_[2] != 0.0) ? zisc::invert(direction_[2]) : max;
  sign_[0] = (inverse_direction_[0] < 0.0) ? 1 : 0;
  sign_[1] = (inverse_direction_[1] < 0.0) ? 1 : 0;
  sign_[2] = (inverse_direction_[2] < 0.0) ? 1 : 0;
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
  is_alive_ = is_alive;
}

/*!
  \details
  No detailed.
  */
inline
const uint8* Ray::sign() const noexcept
{
  return sign_;
}

} // namespace nanairo

#endif // NANAIRO_RAY_INL_HPP
