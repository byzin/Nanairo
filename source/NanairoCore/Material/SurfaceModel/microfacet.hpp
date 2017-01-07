/*!
  \file microfacet.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROFACET_HPP
#define NANAIRO_MICROFACET_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

// Forward declaration
class Sampler;
class SampledDirection;

//! \addtogroup Core
//! \{

/*!
  */
class Microfacet
{
 public:
  //! Calculate microfacet reflection direction
  static SampledDirection calcReflectionDirection(
      const Vector3& vin,
      const SampledDirection& sampled_m_normal) noexcept;

  //! Calculate microfacet reflection half-vector
  static Vector3 calcReflectionHalfVector(const Vector3& vin,
                                          const Vector3& vout) noexcept;

  //! Calculate microfacet reflection inverse jacobian
  static Float calcReflectionInverseJacobian(const Float cos_mi) noexcept;

  //! Calculate microfacet refraction direction
  static SampledDirection calcRefractionDirection(
      const Vector3& vin,
      const SampledDirection& sampled_m_normal,
      const Float n,
      const Float g) noexcept;

  //! Calculate microfacet reflection half-vector
  static Vector3 calcRefractionHalfVector(const Vector3& vin,
                                          const Vector3& vout,
                                          const Float n) noexcept;

  //! Calculate microfacet reflection inverse jacobian
  static Float calcRefractionInverseJacobian(const Float cos_mi,
                                             const Float cos_mo,
                                             const Float n) noexcept;
};

//! \} Core

} // namespace nanairo

#include "microfacet-inl.hpp"

#endif // NANAIRO_MICROFACET_HPP
