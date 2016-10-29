/*!
  \file microfacet-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SampledDirection Microfacet::calcReflectionDirection(
    const Vector3& vin,
    const SampledDirection& microfacet_normal,
    const Float cos_mi) noexcept
{
  const auto& normal = microfacet_normal.direction();
  const auto vout = Fresnel::calcReflectionDirection(vin, normal, cos_mi);
  const Float inverse_jacobian = calcReflectionInverseJacobian(cos_mi);
  const Float inverse_pdf = inverse_jacobian * microfacet_normal.inversePdf();
  ZISC_ASSERT(0.0 < inverse_pdf, "PDF isn't positive.");
  return SampledDirection{vout, inverse_pdf};
}

/*!
  \details
  No detailed.
  */
inline
Vector3 Microfacet::calcReflectionHalfVector(const Vector3& vin,
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
Float Microfacet::calcReflectionInverseJacobian(const Float cos_mi) noexcept
{
  const Float inverse_jacobian = 4.0 * cos_mi;
  ZISC_ASSERT(0.0 < inverse_jacobian, "Jacobian isn't positive.");
  return inverse_jacobian;
}

/*!
  \details
  No detailed.
  */
inline
SampledDirection Microfacet::calcRefractionDirection(
    const Vector3& vin,
    const SampledDirection& microfacet_normal,
    const Float cos_mi,
    const Float n,
    const Float g) noexcept
{
  // Direction
  const auto& m_normal = microfacet_normal.direction();
  const auto vout = Fresnel::calcRefractionDirection(vin, m_normal, cos_mi, n, g);
  // Jacobian
  const Float cos_mo = zisc::dot(m_normal, vout);
  ZISC_ASSERT(zisc::isInBounds(-cos_mo, 0.0, 1.0), "cos_mo isn't [0, 1].");
  const Float inverse_jacobian = calcRefractionInverseJacobian(cos_mi, cos_mo, n);
  const Float inverse_pdf = inverse_jacobian * microfacet_normal.inversePdf();
  ZISC_ASSERT(0.0 < inverse_pdf, "PDF isn't positive.");
  return SampledDirection{vout, inverse_pdf};
}

/*!
  \details
  No detailed.
  */
inline
Vector3 Microfacet::calcRefractionHalfVector(const Vector3& vin,
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
Float Microfacet::calcRefractionInverseJacobian(const Float cos_mi,
                                                const Float cos_mo,
                                                const Float n) noexcept
{
  const Float tmp = cos_mi + n * cos_mo;
  const Float inverse_jacobian = zisc::power<2>(tmp) / (zisc::power<2>(n) * -cos_mo);
  ZISC_ASSERT(0.0 < inverse_jacobian, "Jacobian isn't positive.");
  return inverse_jacobian;
}

} // namespace nanairo

#endif // NANAIRO_MICROFACET_INL_HPP
