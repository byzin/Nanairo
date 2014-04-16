/*!
  \file ray-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RAY_INL_HPP_
#define _NANAIRO_RAY_INL_HPP_

#include "ray.hpp"
// Standard C++ library
#include <limits>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Ray::Ray() :
    is_alive_{false}
{
}

/*!
  \details
  No detailed.
  */
inline
Ray::Ray(const Point3& origin, const Vector3& direction) :
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
const Vector3& Ray::direction() const
{
  return direction_;
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& Ray::inverseDirection() const
{
  return inverse_direction_;
}

/*!
  \details
  No detailed.
  */
inline
bool Ray::isAlive() const
{
  return is_alive_;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& Ray::origin() const
{
  return origin_;
}

/*!
  \details
  No detailed.
  */
inline
void Ray::setDirection(const Vector3& direction)
{
  direction_ = direction;
  setInverseDirection();
}

/*!
  \details
  No detailed.
  */
inline
void Ray::setInverseDirection()
{
  constexpr Float max = std::numeric_limits<Float>::max();
  inverse_direction_[0] = (direction_[0] != 0.0) ? (1.0 / direction_[0]) : max;
  inverse_direction_[1] = (direction_[1] != 0.0) ? (1.0 / direction_[1]) : max;
  inverse_direction_[2] = (direction_[2] != 0.0) ? (1.0 / direction_[2]) : max;
  sign_[0] = (inverse_direction_[0] < 0.0) ? 1 : 0;
  sign_[1] = (inverse_direction_[1] < 0.0) ? 1 : 0;
  sign_[2] = (inverse_direction_[2] < 0.0) ? 1 : 0;
}

/*!
  \details
  No detailed.
  */
inline
void Ray::setOrigin(const Point3& origin)
{
  origin_ = origin;
}

/*!
  \details
  No detailed.
  */
inline
void Ray::setAlive(const bool is_alive)
{
  is_alive_ = is_alive;
}

/*!
  \details
  No detailed.
  */
inline
const uint8* Ray::sign() const
{
  return sign_;
}

} // namespace nanairo

#endif // _NANAIRO_RAY_INL_HPP_
