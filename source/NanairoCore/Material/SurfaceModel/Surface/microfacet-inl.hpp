/*!
  \file microfacet-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
  */
template <> inline
Float Microfacet::SmithMicrosurface::evalG1(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& v,
    const Vector3& m_normal) noexcept
{
  Float g1 = 0.0;
  const Float cos_mv = zisc::dot(m_normal, v);
  if (!((0.0 < v[2]) ^ (0.0 < cos_mv))) {
    const Float r2t2 = Microfacet::calcRoughness2Tan2(roughness_x,
                                                      roughness_y,
                                                      v);
    g1 = 2.0 / (1.0 + zisc::sqrt(1.0 + r2t2));
  }
  ZISC_ASSERT(zisc::isInClosedBounds(g1, 0.0, 1.0), "GGX G1 isn't [0, 1].");
  return g1;
}

/*!
  */
template <> inline
Float Microfacet::SmithMicrosurface::evalG2(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& vin,
    const Vector3& vout,
    const Vector3& m_normal) noexcept
{
  const Float g2 = evalG1(roughness_x, roughness_y, vin, m_normal) *
                   evalG1(roughness_x, roughness_y, vout, m_normal);
  ZISC_ASSERT(zisc::isInClosedBounds(g2, 0.0, 1.0), "GGX G2 isn't [0, 1].");
  return g2;
}

/*!
  \details
  No detailed.
  */
inline
SampledDirection Microfacet::calcReflectionDirection(
    const Vector3& vin,
    const SampledDirection& sampled_m_normal) noexcept
{
  // Calculate the reflection direction
  const auto& m_normal = sampled_m_normal.direction();
  const auto vout = Fresnel::calcReflectionDirection(vin, m_normal);
  // Calculate the pdf of the direction
  const Float cos_mi = zisc::dot(m_normal, vin);
  const Float inverse_jacobian = calcReflectionInverseJacobian(cos_mi);
  const Float inverse_pdf = inverse_jacobian * sampled_m_normal.inversePdf();
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
  const auto half_vector = vin + vout;
  return half_vector.normalized();
}

/*!
  \details
  No detailed.
  */
inline
Float Microfacet::calcReflectionInverseJacobian(const Float cos_mi) noexcept
{
  const Float inverse_jacobian = 4.0 * zisc::abs(cos_mi);
  ZISC_ASSERT(0.0 < inverse_jacobian, "Jacobian isn't positive.");
  return inverse_jacobian;
}

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
inline
SampledDirection Microfacet::calcRefractionDirection(
    const Vector3& vin,
    const SampledDirection& sampled_m_normal,
    const Float n,
    const Float g) noexcept
{
  // Calculate the refraction direction
  const auto& m_normal = sampled_m_normal.direction();
  const auto vout = Fresnel::calcRefractionDirection(vin, m_normal, n, g);
  // Calculate the pdf of the direction
  const Float cos_mi = zisc::dot(m_normal, vin);
  const Float cos_mo = zisc::dot(m_normal, vout);
  ZISC_ASSERT(zisc::isInBounds(-cos_mo, 0.0, 1.0), "cos_mo isn't [0, 1].");
  const Float inverse_jacobian = calcRefractionInverseJacobian(cos_mi, cos_mo, n);
  const Float inverse_pdf = inverse_jacobian * sampled_m_normal.inversePdf();
  ZISC_ASSERT(0.0 < inverse_pdf, "PDF isn't positive.");
  return SampledDirection{vout, inverse_pdf};
}

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
inline
Vector3 Microfacet::calcRefractionHalfVector(const Vector3& vin,
                                             const Vector3& vout,
                                             const Float n) noexcept
{
  return -(vin + n * vout).normalized();
}

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
inline
Float Microfacet::calcRefractionInverseJacobian(const Float cos_mi,
                                                const Float cos_mo,
                                                const Float n) noexcept
{
  const Float tmp = cos_mi + n * cos_mo;
  const Float inverse_jacobian = zisc::power<2>(tmp) /
                                 (zisc::power<2>(n) * zisc::abs(cos_mo));
  ZISC_ASSERT(0.0 < inverse_jacobian, "Jacobian isn't positive.");
  return inverse_jacobian;
}

/*!
  */
inline
Float Microfacet::calcRoughness2Tan2(const Float roughness_x,
                                     const Float roughness_y,
                                     const Vector3& v) noexcept
{
  ZISC_ASSERT(v[2] != 0.0, "The v[2] is zero.");
  const Float r2 = (zisc::power<2>(v[0]) * zisc::power<2>(roughness_x) +
                    zisc::power<2>(v[1]) * zisc::power<2>(roughness_y)) /
                   zisc::power<2>(v[2]);
  return r2;
}

} // namespace nanairo

#endif // NANAIRO_MICROFACET_INL_HPP
