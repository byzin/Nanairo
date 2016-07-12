/*!
  \file microfacet-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROFACET_INL_HPP
#define NANAIRO_MICROFACET_INL_HPP

#include "microfacet.hpp"
// Standard C++ library
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "fresnel.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Utility/floating_point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SampledDirection getMicrofacetReflectionDirection(
    const Vector3& vin,
    const SampledDirection& microfacet_normal,
    const Float cos_theta_mi) noexcept
{
  const auto& normal = microfacet_normal.direction();
  const auto vout = getFresnelReflectionDirection(vin, normal, cos_theta_mi);
  const Float inverse_jacobian = 
      getMicrofacetReflectionInverseJacobian(cos_theta_mi);
  const Float inverse_pdf = inverse_jacobian * microfacet_normal.inversePdf();
  ZISC_ASSERT(0.0 < inverse_pdf, "PDF must be positive.");
  return SampledDirection{vout, inverse_pdf};
}

/*!
  \details
  No detailed.
  */
inline
Vector3 getMicrofacetReflectionHalfVector(const Vector3& vin, 
                                          const Vector3& vout) noexcept
{
  const auto half_vector = vout - vin;
  return half_vector.normalized();
}

/*!
  \details
  No detailed.
  */
inline
Float getMicrofacetReflectionInverseJacobian(const Float cos_theta_mi) noexcept
{
  const Float inverse_jacobian = 4.0 * cos_theta_mi;
  ZISC_ASSERT(0.0 < inverse_jacobian, "Jacobian must be positive.");
  return inverse_jacobian;
}

/*!
  \details
  No detailed.
  */
inline
SampledDirection getMicrofacetRefractionDirection(
    const Vector3& vin,
    const SampledDirection& microfacet_normal,
    const Float cos_theta_mi,
    const Float n,
    const Float g) noexcept
{
  // Direction
  const auto& m_normal = microfacet_normal.direction();
  const auto vout = 
      getFresnelRefractionDirection(vin, m_normal, cos_theta_mi, n, g);
  // Jacobian
  const Float cos_theta_mo = zisc::dot(m_normal, vout);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(-cos_theta_mo),
              "Cos theta_{mo} must be [0, 1].");
  const Float inverse_jacobian = 
      getMicrofacetRefractionInverseJacobian(cos_theta_mi, 
                                             cos_theta_mo, 
                                             n);
  const Float inverse_pdf = inverse_jacobian * microfacet_normal.inversePdf();
  ZISC_ASSERT(0.0 < inverse_pdf, "PDF must be positive.");

  return SampledDirection{vout, inverse_pdf};
}

/*!
  \details
  No detailed.
  */
inline
Vector3 getMicrofacetRefractionHalfVector(const Vector3& vin,
                                          const Vector3& vout,
                                          const Float n) noexcept
{
  return (vin - n * vout).normalized();
}

/*!
  \details
  No detailed.
  */
inline
Float getMicrofacetRefractionInverseJacobian(const Float cos_theta_mi,
                                             const Float cos_theta_mo,
                                             const Float n) noexcept
{
  const Float tmp = cos_theta_mi + n * cos_theta_mo;
  const Float inverse_jacobian = (tmp * tmp) / (n * n * -cos_theta_mo);
  ZISC_ASSERT(0.0 < inverse_jacobian, "Jacobian must be positive.");
  return inverse_jacobian;
}

} // namespace nanairo

#endif // NANAIRO_MICROFACET_INL_HPP
