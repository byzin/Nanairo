/*!
  \file microfacet_ggx-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
  \details
  No detailed.
  */
inline
Float MicrofacetGgx::evalD(const Float roughness, const Float cos_nm) noexcept
{
  ZISC_ASSERT(0.0 <= cos_nm,
              "Microfacet normal isn't in the same hemisphere as normal.");
  const Float a2 = zisc::power<2>(roughness);
  const Float cos2_nm = zisc::power<2>(cos_nm);
  const Float tmp = (a2 - 1.0) * cos2_nm + 1.0;
  const Float d = a2 / (zisc::kPi<Float> * zisc::power<2>(tmp));
  ZISC_ASSERT(0.0 <= d, "GGX D is negative.");
  return d;
}

/*!
  \details
  No detailed.
  */
inline
Float MicrofacetGgx::evalReflectionPdf(const Float roughness,
                                       const Vector3& vin,
                                       const Vector3& vout,
                                       const Vector3& normal) noexcept
{
  // Calculate reflection half vector
  const auto m_normal = calcReflectionHalfVector(vin, vout);

  const Float cos_ni = -zisc::dot(normal, vin);
  const Float cos_mi = -zisc::dot(m_normal, vin);
  const Float cos_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_mi, 0.0, 1.0), "cos_mi isn't [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");

  const Float d = evalD(roughness, cos_nm);
  if (d == 0.0)
    return 0.0;

  return calcReflectionPdf(roughness, d, cos_ni, cos_mi, cos_nm);
}

/*!
  \details
  No detailed.
  */
inline
Float MicrofacetGgx::evalRefractionPdf(const Float roughness,
                                       const Vector3& vin,
                                       const Vector3& vout,
                                       const Vector3& normal,
                                       const Float n) noexcept
{
  // Calculate refraction half vector
  const auto m_normal = calcRefractionHalfVector(vin, vout, n);

  const Float cos_ni = -zisc::dot(normal, vin);
  const Float cos_mi = -zisc::dot(m_normal, vin);
  const Float cos_mo = zisc::dot(m_normal, vout);
  const Float cos_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_mi, 0.0, 1.0), "cos_mi isn't [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");

  const Float d = evalD(roughness, cos_nm);
  if (d == 0.0)
    return 0.0;

  return calcRefractionPdf(roughness, d, cos_ni, cos_mi, cos_mo, cos_nm, n);
}

/*!
  \details
  No detailed.
  */
template <> inline
Float MicrofacetGgx::Smith::evalG1(const Float roughness,
                                   const Float cos_n,
                                   const Float cos_m,
                                   const Float /* cos_nm */) noexcept
{
  if (cos_n * cos_m < 0.0)
    return 0.0;

  const Float roughness2 = zisc::power<2>(roughness);
  const Float cos_vn = zisc::abs(cos_n);
  const Float cos2_vn = zisc::power<2>(cos_vn);
  const Float t = cos2_vn + roughness2 * (1.0 - cos2_vn);
  const Float g1 = (2.0 * cos_vn) / (cos_vn + zisc::sqrt(t));
  ZISC_ASSERT(zisc::isInClosedBounds(g1, 0.0, 1.0), "GGX G1 isn't [0, 1].");
  return g1;
}

/*!
  \details
  No detailed.
  */
template <> inline
Float MicrofacetGgx::Smith::evalG2(const Float roughness,
                                   const Float cos_ni,
                                   const Float cos_no,
                                   const Float cos_mi,
                                   const Float cos_mo,
                                   const Float cos_nm) noexcept
{
  const Float g2 = evalG1(roughness, cos_ni, cos_mi, cos_nm) *
                   evalG1(roughness, cos_no, cos_mo, cos_nm);
  ZISC_ASSERT(zisc::isInClosedBounds(g2, 0.0, 1.0), "GGX G2 isn't [0, 1].");
  return g2;
}

/*!
  \details
  No detailed.
  */
template <> inline
Float MicrofacetGgx::Smith::evalWeight(const Float roughness,
                                       const Float /* cos_ni */,
                                       const Float cos_no,
                                       const Float /* cos_mi */,
                                       const Float cos_mo,
                                       const Float cos_nm) noexcept
{
  return evalG1(roughness, cos_no, cos_mo, cos_nm);
}

/*!
  \details
  No detailed.
  */
template <> inline
Float MicrofacetGgx::VCavity::evalG1(const Float /* roughness */,
                                     const Float cos_n,
                                     const Float cos_m,
                                     const Float cos_nm) noexcept
{
  if (cos_n * cos_m < 0.0)
    return 0.0;

  constexpr Float g_a = 1.0;
  const Float g_b = 2.0 * zisc::abs((cos_nm * cos_n) / cos_m);
  const Float g1 = zisc::min(g_a, g_b);
  ZISC_ASSERT(zisc::isInClosedBounds(g1, 0.0, 1.0), "GGX G1 isn't [0, 1].");
  return g1;
}

/*!
  \details
  No detailed.
  */
template <> inline
Float MicrofacetGgx::VCavity::evalG2(const Float roughness,
                                     const Float cos_ni,
                                     const Float cos_no,
                                     const Float cos_mi,
                                     const Float cos_mo,
                                     const Float cos_nm) noexcept
{
  const Float gi = evalG1(roughness, cos_ni, cos_mi, cos_nm);
  const Float go = evalG1(roughness, cos_no, cos_mo, cos_nm);
  const Float g2 = (0.0 < cos_no)
      ? zisc::min(gi, go)
      : zisc::max(gi + go - 1.0, 0.0);
  ZISC_ASSERT(zisc::isInClosedBounds(g2, 0.0, 1.0), "GGX G2 isn't [0, 1].");
  return g2;
}

/*!
  \details
  No detailed.
  */
template <> inline
Float MicrofacetGgx::VCavity::evalWeight(const Float roughness,
                                         const Float cos_ni,
                                         const Float cos_no,
                                         const Float cos_mi,
                                         const Float cos_mo,
                                         const Float cos_nm) noexcept
{
  return evalG2(roughness, cos_ni, cos_no, cos_mi, cos_mo, cos_nm) /
         evalG1(roughness, cos_ni, cos_mi, cos_nm);
}

/*!
  \details
  No detailed.
  */
inline
Float MicrofacetGgx::evalG1(const Float roughness,
                            const Float cos_n,
                            const Float cos_m,
                            const Float cos_nm) noexcept
{
  using Method = GgxMethod<kUsedType>;
  return Method::evalG1(roughness, cos_n, cos_m, cos_nm);
}

/*!
  \details
  No detailed.
  */
inline
Float MicrofacetGgx::evalG2(const Float roughness,
                            const Float cos_ni,
                            const Float cos_no,
                            const Float cos_mi,
                            const Float cos_mo,
                            const Float cos_nm) noexcept
{
  using Method = GgxMethod<kUsedType>;
  return Method::evalG2(roughness, cos_ni, cos_no, cos_mi, cos_mo, cos_nm);
}

/*!
  \details
  No detailed.
  */
inline
Float MicrofacetGgx::evalWeight(const Float roughness,
                                const Float cos_ni,
                                const Float cos_no,
                                const Float cos_mi,
                                const Float cos_mo,
                                const Float cos_nm) noexcept
{
  using Method = GgxMethod<kUsedType>;
  return Method::evalWeight(roughness, cos_ni, cos_no, cos_mi, cos_mo, cos_nm);
}

/*!
  \details
  No detailed.
  */
inline
Float MicrofacetGgx::calcReflectionPdf(const Float roughness,
                                       const Float d,
                                       const Float cos_ni,
                                       const Float cos_mi,
                                       const Float cos_nm) noexcept
{
  const Float g1 = evalG1(roughness, cos_ni, cos_mi, cos_nm);
  if (g1 == 0.0)
    return 0.0;

  const Float inverse_jacobian = calcReflectionInverseJacobian(cos_mi);

  const Float pdf = zisc::abs(cos_mi / (cos_ni * inverse_jacobian)) * (g1 * d);
  ZISC_ASSERT(0.0 < pdf, "PDF isn't positive.");
  return pdf;
}

/*!
  \details
  No detailed.
  */
inline
Float MicrofacetGgx::calcRefractionPdf(const Float roughness,
                                       const Float d,
                                       const Float cos_ni,
                                       const Float cos_mi,
                                       const Float cos_mo,
                                       const Float cos_nm,
                                       const Float n) noexcept
{
  const Float g1 = evalG1(roughness, cos_ni, cos_mi, cos_nm);
  if (g1 == 0.0)
    return 0.0;

  const Float inverse_jacobian = calcRefractionInverseJacobian(cos_mi, cos_mo, n);

  const Float pdf = zisc::abs(cos_mi / (cos_ni * inverse_jacobian)) * (g1 * d);
  ZISC_ASSERT(0.0 < pdf, "PDF isn't positive.");
  return pdf;
}

} // namespace nanairo

#endif // NANAIRO_MICROFACET_GGX_INL_HPP
