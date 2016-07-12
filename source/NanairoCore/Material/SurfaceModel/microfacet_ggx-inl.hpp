/*!
  \file microfacet_ggx-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

namespace ggx_v_cavity {
Float evaluateGgxG1(const Float, const Float, const Float, const Float) noexcept;
Float evaluateGgxG2(const Float, const Float, const Float, 
                    const Float, const Float, const Float) noexcept;
Float evaluateGgxWeight(const Float, const Float, const Float,
                        const Float, const Float, const Float) noexcept;
} // namespace ggx_v_cavity

namespace ggx_smith {
Float evaluateGgxG1(const Float, const Float, const Float, const Float) noexcept;
Float evaluateGgxG2(const Float, const Float, const Float, 
                    const Float, const Float, const Float) noexcept;
Float evaluateGgxWeight(const Float, const Float, const Float,
                        const Float, const Float, const Float) noexcept;
} // namespace ggx_v_cavity

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxD(const Float roughness, const Float cos_theta_nm) noexcept
{
  ZISC_ASSERT(0.0 <= cos_theta_nm, 
              "Microfacet normal must be in the same hemisphere as normal.");

  const Float a2 = roughness * roughness;
  const Float cos2_theta_nm = cos_theta_nm * cos_theta_nm;
  const Float tmp = (a2 - 1.0) * cos2_theta_nm + 1.0;
  const Float d = a2 / (zisc::kPi<Float> * tmp * tmp);
  ZISC_ASSERT(0.0 <= d, "GGX D must be positive.");
  return d;
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxG1(const Float roughness,
                    const Float cos_theta_n,
                    const Float cos_theta_m,
                    const Float cos_theta_nm) noexcept
{
#if defined(NANAIRO_GGX_V_CAVITY)
  return ggx_v_cavity::
      evaluateGgxG1(roughness, cos_theta_n, cos_theta_m, cos_theta_nm);
#elif defined(NANAIRO_GGX_SMITH)
  return ggx_smith::
      evaluateGgxG1(roughness, cos_theta_n, cos_theta_m, cos_theta_nm);
#else
  static_assert(false, "\"evaluateGgxG1\" isn't implemented.");
  return 0.0;
#endif
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxG2(const Float roughness,
                    const Float cos_theta_ni,
                    const Float cos_theta_no,
                    const Float cos_theta_mi,
                    const Float cos_theta_mo,
                    const Float cos_theta_nm) noexcept
{
#if defined(NANAIRO_GGX_V_CAVITY)
  return ggx_v_cavity::
      evaluateGgxG2(roughness, cos_theta_ni, cos_theta_no, 
                    cos_theta_mi, cos_theta_mo, cos_theta_nm);
#elif defined(NANAIRO_GGX_SMITH)
  return ggx_smith::
      evaluateGgxG2(roughness, cos_theta_ni, cos_theta_no, 
                    cos_theta_mi, cos_theta_mo, cos_theta_nm);
#else
  static_assert(false, "\"evaluateGgxG2\" isn't implemented.");
  return 0.0;
#endif
}

namespace inner {

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxReflectionPdf(const Float roughness,
                               const Float d,
                               const Float cos_theta_ni,
                               const Float cos_theta_mi,
                               const Float cos_theta_nm) noexcept
{
  const Float g1 = 
      evaluateGgxG1(roughness, cos_theta_ni, cos_theta_mi, cos_theta_nm);
  if (g1 == 0.0)
    return 0.0;

  const Float inverse_jacobian =
      getMicrofacetReflectionInverseJacobian(cos_theta_mi);

  const Float pdf = (g1 * cos_theta_mi * d) / (cos_theta_ni * inverse_jacobian);
  ZISC_ASSERT(0.0 < pdf, "PDF must be positive.");
  return pdf;
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxRefractionPdf(const Float roughness,
                               const Float d,
                               const Float cos_theta_ni,
                               const Float cos_theta_mi,
                               const Float cos_theta_mo,
                               const Float cos_theta_nm,
                               const Float n) noexcept
{
  const Float g1 = 
      evaluateGgxG1(roughness, cos_theta_ni, cos_theta_mi, cos_theta_nm);
  if (g1 == 0.0)
    return 0.0;

  const Float inverse_jacobian =
      getMicrofacetRefractionInverseJacobian(cos_theta_mi, cos_theta_mo, n);

  const Float pdf = (g1 * cos_theta_mi * d) / (cos_theta_ni * inverse_jacobian);
  ZISC_ASSERT(0.0 < pdf, "PDF must be positive.");
  return pdf;
}

} // namespace inner

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
SampledSpectra<kSampleSize> evaluateGgxReflectance(
    const Float roughness,
    const Vector3& vin,
    const Vector3& vout,
    const Vector3& normal,
    const SampledSpectra<kSampleSize>& r0,
    Float* pdf) noexcept
{
  const auto& wavelengths = r0.wavelengths();

  // Calculate reflection half vector
  const auto m_normal = getMicrofacetReflectionHalfVector(vin, vout);

  const Float cos_theta_ni = -zisc::dot(normal, vin);
  const Float cos_theta_no = zisc::dot(normal, vout);
  const Float cos_theta_mi = -zisc::dot(m_normal, vin);
  const Float cos_theta_mo = cos_theta_mi;
  const Float cos_theta_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_ni),
              "Cos theta_{ni} must be [0, 1].");
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_no),
              "Cos theta_{no} must be [0, 1].");
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_nm),
              "Cos theta_{nm} must be [0, 1].");
  ZISC_ASSERT(0.0 <= cos_theta_ni * cos_theta_mi,
              "Microfacet normal must be in the same hemisphere as normal.");

  // Evaluate D
  const Float d = evaluateGgxD(roughness, cos_theta_nm);
  if (d == 0.0)
    return SampledSpectra<kSampleSize>{wavelengths};

  // Evaluate G2(i, o, m)
  const Float g2 = evaluateGgxG2(roughness, cos_theta_ni, cos_theta_no,
                                 cos_theta_mi, cos_theta_mo, cos_theta_nm);
  if (g2 == 0.0)
    return SampledSpectra<kSampleSize>{wavelengths};

  // Evaluate the fresnel reflectance
  const auto fresnel = solveFresnelConductorEquation(cos_theta_mi, r0);

  // Calculate the pdf
  if (pdf != nullptr) {
    *pdf = inner::evaluateGgxReflectionPdf(roughness, d, cos_theta_ni, 
                                           cos_theta_mi, cos_theta_nm);
  }

  // Calculate reflectance
  const auto f = fresnel * (g2 * d / (4.0 * cos_theta_ni * cos_theta_no));
  ZISC_ASSERT(!hasNegativeFloat(f), "Reflectance must be positive.");
  return f;
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxReflectionPdf(const Float roughness,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Vector3& normal) noexcept
{
  // Calculate reflection half vector
  const auto m_normal = getMicrofacetReflectionHalfVector(vin, vout);

  const Float cos_theta_ni = -zisc::dot(normal, vin);
  const Float cos_theta_mi = -zisc::dot(m_normal, vin);
  const Float cos_theta_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_ni),
              "Cos theta_{ni} must be [0, 1].");
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_nm),
              "Cos theta_{nm} must be [0, 1].");
  ZISC_ASSERT(0.0 <= cos_theta_ni * cos_theta_mi, 
              "Microfacet normal must be in the same hemisphere as normal.");

  const Float d = evaluateGgxD(roughness, cos_theta_nm);
  if (d == 0.0)
    return 0.0;

  return inner::evaluateGgxReflectionPdf(roughness, d, cos_theta_ni,
                                         cos_theta_mi, cos_theta_nm);
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxDielectricReflectionPdf(const Float roughness,
                                         const Vector3& vin,
                                         const Vector3& vout,
                                         const Vector3& normal,
                                         const Float n) noexcept
{
  // Calculate reflection half vector
  const auto m_normal = getMicrofacetReflectionHalfVector(vin, vout);

  const Float cos_theta_ni = -zisc::dot(normal, vin);
  const Float cos_theta_mi = -zisc::dot(m_normal, vin);
  const Float cos_theta_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_ni),
              "Cos theta_{ni} must be [0, 1].");
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_nm),
              "Cos theta_{nm} must be [0, 1].");
  ZISC_ASSERT(0.0 <= cos_theta_ni * cos_theta_mi, 
              "Microfacet normal must be in the same hemisphere as normal.");

  const Float d = evaluateGgxD(roughness, cos_theta_nm);
  if (d == 0.0)
    return 0.0;

  // Evaluate the fresnel term
  const auto result = evaluateFresnelG(n, cos_theta_mi);
  const bool is_not_perfect_reflection = std::get<0>(result);
  const Float g = std::get<1>(result);
  const Float fresnel = (is_not_perfect_reflection)
      ? solveFresnelDielectricEquation(cos_theta_mi, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(isBetweenZeroAndOneFloat(fresnel),
              "Fresnel reflectance must be [0, 1].");
  if (fresnel == 0.0)
    return 0.0;

  return fresnel * 
         inner::evaluateGgxReflectionPdf(roughness, d, cos_theta_ni,
                                         cos_theta_mi, cos_theta_nm);
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxDielectricRefractionPdf(const Float roughness,
                                         const Vector3& vin,
                                         const Vector3& vout,
                                         const Vector3& normal,
                                         const Float n) noexcept
{
  // Calculate refraction half vector
  const auto m_normal = getMicrofacetRefractionHalfVector(vin, vout, n);

  const Float cos_theta_ni = -zisc::dot(normal, vin);
  const Float cos_theta_mi = -zisc::dot(m_normal, vin);
  const Float cos_theta_mo = zisc::dot(m_normal, vout);
  const Float cos_theta_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_ni),
              "Cos theta_{ni} must be [0, 1].");
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_nm),
              "Cos theta_{nm} must be [0, 1].");
  ZISC_ASSERT(0.0 <= cos_theta_ni * cos_theta_mi, 
              "Microfacet normal must be in the same hemisphere as normal.");

  const Float d = evaluateGgxD(roughness, cos_theta_nm);
  if (d == 0.0)
    return 0.0;

  // Evaluate the fresnel term
  const auto result = evaluateFresnelG(n, cos_theta_mi);
  const bool is_not_perfect_reflection = std::get<0>(result);
  const Float g = std::get<1>(result);
  const Float fresnel = (is_not_perfect_reflection)
      ? solveFresnelDielectricEquation(cos_theta_mi, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(isBetweenZeroAndOneFloat(fresnel),
              "Fresnel reflectance must be [0, 1].");
  if (fresnel == 1.0)
    return 0.0;

  return (1.0 - fresnel) * 
         inner::evaluateGgxRefractionPdf(roughness, d, cos_theta_ni,
                                         cos_theta_mi, cos_theta_mo,
                                         cos_theta_nm, n);
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxWeight(const Float roughness,
                        const Float cos_theta_ni,
                        const Float cos_theta_no,
                        const Float cos_theta_mi,
                        const Float cos_theta_mo,
                        const Float cos_theta_nm) noexcept
{
#if defined(NANAIRO_GGX_V_CAVITY)
  return ggx_v_cavity::
    evaluateGgxWeight(roughness, cos_theta_ni, cos_theta_no,
                      cos_theta_mi, cos_theta_mo, cos_theta_nm);
#elif defined(NANAIRO_GGX_SMITH)
  return ggx_smith::
    evaluateGgxWeight(roughness, cos_theta_ni, cos_theta_no,
                      cos_theta_mi, cos_theta_mo, cos_theta_nm);
#else
  static_assert(false, \"evaluateGgxWeight\" isn't implemented.");
  return 0.0;
#endif
}

namespace ggx_v_cavity {

///*!
//  \details
//  No detailed.
//  */
//inline
//Float evaluateGgxG1(const Float /* roughness */,
//                    const Float cos_theta_omega,
//                    const Float cos_theta_m,
//                    const Float cos_theta_momega)
//{
//  if (cos_theta_momega * cos_theta_omega <= 0.0)
//    return 0.0;
//
//  const Float g = zisc::abs(2.0 * cos_theta_m * cos_theta_omega / cos_theta_momega);
//  return zisc::min(g, 1.0);
//}
//
///*!
//  \details
//  No detailed.
//  */
//inline
//Float evaluateGgxG2(const Float roughness,
//                    const Float cos_theta_i,
//                    const Float cos_theta_o,
//                    const Float cos_theta_m,
//                    const Float cos_theta_mi,
//                    const Float cos_theta_mo)
//{
//  const Float g1i = evaluateGgxG1(roughness, cos_theta_i, cos_theta_m, cos_theta_mi);
//  const Float g1o = evaluateGgxG1(roughness, cos_theta_o, cos_theta_m, cos_theta_mo);
//  return (0.0 < cos_theta_o)
//      ? zisc::min(g1i, g1o)
//      : zisc::max(g1i + g1o - 1.0, 0.0);
//}
//
///*!
//  \details
//  No detailed.
//  */
//inline
//Float evaluateGgxWeight(const Float roughness,
//                        const Float cos_theta_i,
//                        const Float cos_theta_o,
//                        const Float cos_theta_m,
//                        const Float cos_theta_mi,
//                        const Float cos_theta_mo)
//{
//  const Float weight =  evaluateGgxG2(roughness, cos_theta_i, cos_theta_o,
//                                      cos_theta_m, cos_theta_mi, cos_theta_mo) /
//                        evaluateGgxG1(roughness, cos_theta_i, 
//                                      cos_theta_m, cos_theta_mi);
//  return zisc::clamp(weight, 0.0, 1.0);
//}

} // namespace ggx_v_cavity

namespace ggx_smith {

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxG1(const Float roughness, 
                    const Float cos_theta_n, 
                    const Float cos_theta_m,
                    const Float /* cos_theta_nm */) noexcept
{
//  ZISC_ASSERT(0.0 <= cos_theta_n * cos_theta_m,
//              "Microfacet normal must be same direction as normal.");
  if (cos_theta_n * cos_theta_m < 0.0)
    return 0.0;

  const Float roughness2 = roughness * roughness;
  const Float cos2_theta_n = cos_theta_n * cos_theta_n;
  const Float t = roughness2 + (1.0 - roughness2) * cos2_theta_n;
  const Float cos_n = zisc::abs(cos_theta_n);
  const Float g1 = 2.0 * cos_n / (cos_n + zisc::sqrt(t));
  ZISC_ASSERT(isBetweenZeroAndOneFloat(g1), "GGX G1 must be [0, 1].");
  return g1;
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxG2(const Float roughness,
                    const Float cos_theta_ni,
                    const Float cos_theta_no,
                    const Float cos_theta_mi,
                    const Float cos_theta_mo,
                    const Float cos_theta_nm) noexcept
{
  const Float g2 = 
      evaluateGgxG1(roughness, cos_theta_ni, cos_theta_mi, cos_theta_nm) *
      evaluateGgxG1(roughness, cos_theta_no, cos_theta_mo, cos_theta_nm);
  ZISC_ASSERT( isBetweenZeroAndOneFloat(g2), "GGX G2 must be [0, 1].");

  return g2;
}

/*!
  \details
  No detailed.
  */
inline
Float evaluateGgxWeight(const Float roughness,
                        const Float /* cos_theta_ni */,
                        const Float cos_theta_no,
                        const Float /* cos_theta_mi */,
                        const Float cos_theta_mo,
                        const Float cos_theta_nm) noexcept
{
  return evaluateGgxG1(roughness, cos_theta_no, cos_theta_mo, cos_theta_nm);
}

/*!
  \details
  No detailed.
  */
inline
Vector3 stretchGgxMicrosurface(const Float roughness, const Vector3& vin) noexcept
{
  const Vector3 vin_dash{roughness * vin[0], roughness * vin[1], vin[2]};
  return vin_dash.normalized();
}

} // namespace ggx_smith

} // namespace nanairo

#endif // NANAIRO_MICROFACET_GGX_INL_HPP
