/*!
  \file ray.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RAY_HPP
#define NANAIRO_RAY_HPP

// Standard C++ library
#include <array>
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


  //! Return the direction
  const Vector3& direction() const noexcept;

  //! Initialize the inverse direction
  void initInvDirection() noexcept;

  //! Return the inverted direction
  const Vector3& invDirection() const noexcept;

  //! Check if the ray is alive
  bool isAlive() const noexcept;

  //! Make a ray
  template <bool init_inv_direction = true>
  static Ray makeRay(const Point3& origin, const Vector3& direction) noexcept;

  //! Return the origin
  const Point3& origin() const noexcept;

  //! Set ray alival
  void setAlive(const bool is_alive) noexcept;

  //! Set ray direction
  void setDirection(const Vector3& direction) noexcept;

  //! Set ray origin
  void setOrigin(const Point3& origin) noexcept;

  //! Return the direction sign
  std::array<uint8, 3> sign() const noexcept;

 private:
  //! Create ray
  Ray(const Point3& origin, const Vector3& direction) noexcept;

  //! Initialize the ray
  void initialize() noexcept;


  Point3 origin_;
  Vector3 direction_,
          inv_direction_;
  std::array<uint8, 3> sign_;
  uint8 is_alive_;
  std::array<uint8, 4> padding_;
};

//! \} Core

} // namespace nanairo

#include "ray-inl.hpp"

#endif // NANAIRO_RAY_HPP
