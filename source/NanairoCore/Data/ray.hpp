/*!
  \file ray.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RAY_HPP
#define NANAIRO_RAY_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Ray
{
 public:
  //! Create a dead ray
  Ray() noexcept;

  //! Create ray
  Ray(const Point3& origin, const Vector3& direction) noexcept;


  //! Return the direction
  const Vector3& direction() const noexcept;

  //! Return the inverse direction vector
  const Vector3& inverseDirection() const noexcept;

  //! Check if the ray is alive
  bool isAlive() const noexcept;

  //! Return the origin
  const Point3& origin() const noexcept;

  //! Set ray alival
  void setAlive(const bool is_alive) noexcept;

  //! Set ray direction
  void setDirection(const Vector3& direction) noexcept;

  //! Set ray origin
  void setOrigin(const Point3& origin) noexcept;

  //! Return the direction sign
  const uint8* sign() const noexcept;

 private:
  //! Set inverse direction.
  void setInverseDirection() noexcept;

  Point3 origin_;
  Vector3 direction_,
          inverse_direction_;
  uint8 sign_[3];
  bool is_alive_;
};

//! \} Core

} // namespace nanairo

#include "ray-inl.hpp"

#endif // NANAIRO_RAY_HPP
