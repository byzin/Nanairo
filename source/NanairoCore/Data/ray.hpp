/*!
  \file ray.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RAY_HPP_
#define _NANAIRO_RAY_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

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
  Ray();

  //! Create ray
  Ray(const Point3& origin, const Vector3& direction);


  //! Return the direction
  const Vector3& direction() const;

  //! Return the inverse direction vector
  const Vector3& inverseDirection() const;

  //! Check if the ray is alive
  bool isAlive() const;

  //! Return the origin
  const Point3& origin() const;

  //! Set ray alival
  void setAlive(const bool is_alive);

  //! Set ray direction
  void setDirection(const Vector3& direction);

  //! Set ray origin
  void setOrigin(const Point3& origin);

  //! Return the direction sign
  const uint8* sign() const;

 private:
  //! Set inverse direction.
  void setInverseDirection();

  Point3 origin_;
  Vector3 direction_,
          inverse_direction_;
  uint8 sign_[3];
  bool is_alive_;
};

//! \} Core

} // namespace nanairo

#include "ray-inl.hpp"

#endif // _NANAIRO_RAY_HPP_
