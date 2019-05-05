/*!
  \file microfacet_ggx-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROFACET_GGX_INL_HPP
#define NANAIRO_MICROFACET_GGX_INL_HPP

#include "microfacet_ggx.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "microfacet.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  */
inline
Float MicrofacetGgx::evalD(const Float roughness_x,
                           const Float roughness_y,
                           const Vector3& m_normal) noexcept
{
  ZISC_ASSERT(0.0 <= m_normal[2],
              "The microfacet normal isn't in the upper hemisphere.");
  Float t = zisc::power<2>(m_normal[0]) / zisc::power<2>(roughness_x) +
            zisc::power<2>(m_normal[1]) / zisc::power<2>(roughness_y);
  t = zisc::power<2>(zisc::power<2>(m_normal[2]) + t);
  t = zisc::kPi<Float> * roughness_x * roughness_y * t;
  const Float d = zisc::invert(t);
  ZISC_ASSERT(0.0 <= d, "GGX D is negative.");
  return d;
}

/*!
  */
inline
Float MicrofacetGgx::evalG1(const Float roughness_x,
                            const Float roughness_y,
                            const Vector3& v,
                            const Vector3& m_normal) noexcept
{
  using MSurface = Microsurface<kMicrosurface>;
  return MSurface::evalG1(roughness_x, roughness_y, v, m_normal);
}

/*!
  */
inline
Float MicrofacetGgx::evalG2(const Float roughness_x,
                            const Float roughness_y,
                            const Vector3& vin,
                            const Vector3& vout,
                            const Vector3& m_normal) noexcept
{
  using MSurface = Microsurface<kMicrosurface>;
  return MSurface::evalG2(roughness_x, roughness_y, vin, vout, m_normal);
}

/*!
  */
inline
Float MicrofacetGgx::evalReflectionPdf(const Float roughness_x,
                                       const Float roughness_y,
                                       const Vector3& vin,
                                       const Vector3& m_normal) noexcept
{
  const Float d = evalD(roughness_x, roughness_y, m_normal);
  return (d != 0.0)
      ? calcReflectionPdf(roughness_x, roughness_y, vin, m_normal, d)
      : 0.0;
}

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
inline
Float MicrofacetGgx::evalRefractionPdf(const Float roughness_x,
                                       const Float roughness_y,
                                       const Vector3& vin,
                                       const Vector3& vout,
                                       const Vector3& m_normal,
                                       const Float n) noexcept
{
  const Float d = evalD(roughness_x, roughness_y, m_normal);
  return (d != 0.0)
      ? calcRefractionPdf(roughness_x, roughness_y, vin, vout, m_normal, n, d)
      : 0.0;
}

/*!
  */
template <> inline
Float MicrofacetGgx::SmithGgxMicrosurface::evalWeight(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& /* vin */,
    const Vector3& vout,
    const Vector3& m_normal) noexcept
{
  constexpr auto microsurface = Microfacet::MicrosurfaceProfile::kSmith;
  using MSurface = Microfacet::Microsurface<microsurface>;
  return MSurface::evalG1(roughness_x, roughness_y, vout, m_normal);
}

/*!
  */
inline
Float MicrofacetGgx::evalWeight(const Float roughness_x,
                                const Float roughness_y,
                                const Vector3& vin,
                                const Vector3& vout,
                                const Vector3& m_normal) noexcept
{
  using MSurface = GgxMicrosurface<kMicrosurface>;
  return MSurface::evalWeight(roughness_x, roughness_y, vin, vout, m_normal);
}

/*!
  */
inline
Float MicrofacetGgx::calcReflectionPdf(const Float roughness_x,
                                       const Float roughness_y,
                                       const Vector3& vin,
                                       const Vector3& m_normal,
                                       const Float d) noexcept
{
  Float pdf = 0.0;
  const Float g1 = evalG1(roughness_x, roughness_y, vin, m_normal);
  if (0.0 < g1) {
    const Float cos_ni = vin[2];
    const Float cos_mi = zisc::dot(m_normal, vin);
    const Float inverse_jacobian = calcReflectionInverseJacobian(cos_mi);
    pdf = zisc::abs(cos_mi / (cos_ni * inverse_jacobian)) * (g1 * d);
    ZISC_ASSERT(0.0 < pdf, "The pdf isn't positive.");
  }
  return pdf;
}

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
inline
Float MicrofacetGgx::calcRefractionPdf(const Float roughness_x,
                                       const Float roughness_y,
                                       const Vector3& vin,
                                       const Vector3& vout,
                                       const Vector3& m_normal,
                                       const Float n,
                                       const Float d) noexcept
{
  Float pdf = 0.0;
  const Float g1 = evalG1(roughness_x, roughness_y, vin, m_normal);
  if (0.0 < g1) {
    const Float cos_ni = vin[2];
    const Float cos_mi = zisc::dot(m_normal, vin);
    const Float cos_mo = zisc::dot(m_normal, vout);
    const Float inverse_jacobian = calcRefractionInverseJacobian(cos_mi, cos_mo, n);
    pdf = zisc::abs(cos_mi / (cos_ni * inverse_jacobian)) * (g1 * d);
    ZISC_ASSERT(0.0 < pdf, "The pdf isn't positive.");
  }
  return pdf;
}

} // namespace nanairo

#endif // NANAIRO_MICROFACET_GGX_INL_HPP
