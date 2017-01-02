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
template <uint kSampleSize>
SampledSpectra<kSampleSize> MicrofacetGgx::evalReflectance(
    const Float roughness,
    const Vector3& vin,
    const Vector3& vout,
    const Vector3& normal,
    const SampledSpectra<kSampleSize>& r0,
    Float* pdf) noexcept
{
  const auto& wavelengths = r0.wavelengths();

  // Calculate reflection half vector
  const auto m_normal = calcReflectionHalfVector(vin, vout);

  const Float cos_ni = -zisc::dot(normal, vin);
  const Float cos_no = zisc::dot(normal, vout);
  const Float cos_mi = -zisc::dot(m_normal, vin);
  const Float cos_mo = cos_mi;
  const Float cos_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(zisc::isInBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInBounds(cos_no, 0.0, 1.0), "cos_no isn't [0, 1].");
  ZISC_ASSERT(zisc::isInBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");
  ZISC_ASSERT(0.0 <= cos_ni * cos_mi,
              "Microfacet normal isn't in the same hemisphere as normal.");

  // Evaluate D
  const Float d = evalD(roughness, cos_nm);
  if (d == 0.0)
    return SampledSpectra<kSampleSize>{wavelengths};

  // Evaluate G2(i, o, m)
  const Float g2 = evalG2(roughness, cos_ni, cos_no, cos_mi, cos_mo, cos_nm);
  if (g2 == 0.0)
    return SampledSpectra<kSampleSize>{wavelengths};

  // Evaluate the fresnel reflectance
  const auto fresnel = Fresnel::evalConductorEquation(cos_mi, r0);

  // Calculate the pdf
  if (pdf != nullptr)
    *pdf = calcReflectionPdf(roughness, d, cos_ni, cos_mi, cos_nm);

  // Calculate reflectance
  const auto f = fresnel * (g2 * d / (4.0 * cos_ni * cos_no));
  ZISC_ASSERT(!f.hasNegative(), "Reflectance isn't positive.");
  return f;
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
  ZISC_ASSERT(zisc::isInBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");
  ZISC_ASSERT(0.0 <= cos_ni * cos_mi,
              "Microfacet normal isn't in the same hemisphere as normal.");

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
Float MicrofacetGgx::evalReflectionPdf(const Float roughness,
                                       const Vector3& vin,
                                       const Vector3& vout,
                                       const Vector3& normal,
                                       const Float n) noexcept
{
  // Calculate reflection half vector
  const auto m_normal = calcReflectionHalfVector(vin, vout);

  const Float cos_ni = -zisc::dot(normal, vin);
  const Float cos_mi = -zisc::dot(m_normal, vin);
  const Float cos_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(zisc::isInBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");
  ZISC_ASSERT(0.0 <= cos_ni * cos_mi,
              "Microfacet normal isn't in the same hemisphere as normal.");

  const Float d = evalD(roughness, cos_nm);
  if (d == 0.0)
    return 0.0;

  // Evaluate the fresnel term
  const auto result = Fresnel::evalG(n, cos_mi);
  const bool is_perfect_reflection = !std::get<0>(result);
  const Float g = std::get<1>(result);
  const Float fresnel = (!is_perfect_reflection)
      ? Fresnel::evalDielectricEquation(cos_mi, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(zisc::isInBounds(fresnel, 0.0, 1.0),
              "Fresnel reflectance isn't [0, 1].");
  if (fresnel == 0.0)
    return 0.0;

  return fresnel * calcReflectionPdf(roughness, d, cos_ni, cos_mi, cos_nm);
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
  ZISC_ASSERT(zisc::isInBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");
  ZISC_ASSERT(0.0 <= cos_ni * cos_mi,
              "Microfacet normal isn't in the same hemisphere as normal.");

  const Float d = evalD(roughness, cos_nm);
  if (d == 0.0)
    return 0.0;

  // Evaluate the fresnel term
  const auto result = Fresnel::evalG(n, cos_mi);
  const bool is_perfect_reflection = !std::get<0>(result);
  const Float g = std::get<1>(result);
  const Float fresnel = (!is_perfect_reflection)
      ? Fresnel::evalDielectricEquation(cos_mi, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(zisc::isInBounds(fresnel, 0.0, 1.0),
              "Fresnel reflectance isn't [0, 1].");
  if (fresnel == 1.0)
    return 0.0;

  return (1.0 - fresnel) *
         calcRefractionPdf(roughness, d, cos_ni, cos_mi, cos_mo, cos_nm, n);
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

  const Float pdf = (g1 * cos_mi * d) / (cos_ni * inverse_jacobian);
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

  const Float pdf = (g1 * cos_mi * d) / (cos_ni * inverse_jacobian);
  ZISC_ASSERT(0.0 < pdf, "PDF isn't positive.");
  return pdf;
}

/*!
  */
inline
SampledGgxNormal::SampledGgxNormal(const SampledDirection& microfacet_normal,
                                   const Float cos_ni,
                                   const Float cos_mi,
                                   const Float cos_nm) noexcept :
    microfacet_normal_{microfacet_normal},
    cos_ni_{cos_ni},
    cos_mi_{cos_mi},
    cos_nm_{cos_nm}
{
}

/*!
  */
inline
Float SampledGgxNormal::cosNi() const noexcept
{
  return cos_ni_;
}

/*!
  */
inline
Float SampledGgxNormal::cosMi() const noexcept
{
  return cos_mi_;
}

/*!
  */
inline
Float SampledGgxNormal::cosNm() const noexcept
{
  return cos_nm_;
}

/*!
  */
inline
const SampledDirection& SampledGgxNormal::microfacetNormal() const noexcept
{
  return microfacet_normal_;
}

} // namespace nanairo

#endif // NANAIRO_MICROFACET_GGX_INL_HPP
