/*!
  \file microfacet_ggx.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "microfacet_ggx.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "fresnel.hpp"
#include "microfacet.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Utility/floating_point.hpp"

namespace nanairo {

namespace ggx_v_cavity {
Vector3 sampleGgxMicrofacetNormal(const Float, const Vector3&, Sampler&);
} // namespace ggx_v_cavity
namespace ggx_smith {
Vector3 sampleGgxMicrofacetNormal(const Float, const Vector3&, Sampler&);
} // namespace ggx_smith

/*!
  \details
  No detailed.
  */
Float evaluateGgxReflectance(const Float roughness,
                             const Vector3& vin,
                             const Vector3& vout,
                             const Vector3& normal,
                             const Float n,
                             const Float cos_theta_no,
                             Float* pdf)
{
  // Calculate nanairoion half vector
  const auto m_normal = getMicrofacetReflectionHalfVector(vin, vout);

  const Float cos_theta_ni = -zisc::dot(normal, vin);
  const Float cos_theta_mi = -zisc::dot(m_normal, vin);
  const Float cos_theta_mo = cos_theta_mi;
  const Float cos_theta_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_ni),
               "Cos theta_{ni} must be [0, 1].");
  ZISC_ASSERT(isBetweenZeroAndOneFloat(cos_theta_nm),
               "Cos theta_{nm} must be [0, 1].");
  ZISC_ASSERT(0.0 <= cos_theta_ni * cos_theta_mi, 
              "Microfacet normal must be in the same hemisphere as normal.");

  // Evaluate D
  const Float d = evaluateGgxD(roughness, cos_theta_nm);
  if (d == 0.0)
    return 0.0;

  // Evaluate G2(i, o, m)
  const Float g2 = evaluateGgxG2(roughness, cos_theta_ni, cos_theta_no,
                                 cos_theta_mi, cos_theta_mo, cos_theta_nm);
  if (g2 == 0.0)
    return 0.0;

  // Evaluate the fresnel reflectance
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

  // Calculate the pdf
  if (pdf != nullptr) {
    *pdf = fresnel *
           inner::evaluateGgxReflectionPdf(roughness, d, cos_theta_ni,
                                           cos_theta_mi, cos_theta_nm);
  }

  // Calculate reflectance
  const Float f = (fresnel * g2 * d) / (4.0 * cos_theta_ni * cos_theta_no);
  ZISC_ASSERT(!isNegativeFloat(f), "Reflectance must be positive.");
  return f;
}

/*!
  \details
  No detailed.
  */
Float evaluateGgxTransmittance(const Float roughness,
                               const Vector3& vin,
                               const Vector3& vout,
                               const Vector3& normal,
                               const Float n,
                               const Float cos_theta_no,
                               Float* pdf)
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

  // Evaluate D
  const Float d = evaluateGgxD(roughness, cos_theta_nm);
  if (d == 0.0)
    return 0.0;

  // Evaluate G2(i, o, m)
  const Float g2 = evaluateGgxG2(roughness, cos_theta_ni, cos_theta_no,
                                 cos_theta_mi, cos_theta_mo, cos_theta_nm);
  if (g2 == 0.0)
    return 0.0;

  // Evaluate the fresnel reflectance
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


  // Calculate the pdf
  if (pdf != nullptr) {
    *pdf = (1.0 - fresnel) *
           inner::evaluateGgxRefractionPdf(roughness, d, cos_theta_ni, 
                                           cos_theta_mi, cos_theta_mo, 
                                           cos_theta_nm, n);
  }

  // Calculate transmittance
  const Float k1 = (cos_theta_mi * cos_theta_mo) / 
                   (cos_theta_ni * cos_theta_no);
  ZISC_ASSERT(!isNegativeFloat(k1), "The k1 must be positive.");
  const Float k2 = (n * n) / zisc::power<2>(cos_theta_mi + n * cos_theta_mo);
  const Float k = k1 * k2;
  const Float f = (1.0 - fresnel) * k * g2 * d;
  ZISC_ASSERT(!isNegativeFloat(f), "Transmittance must be positive.");
  return f;
}

/*!
  \details
  No detailed.
  */
SampledDirection sampleGgxMicrofacetNormal(const Float roughness,
                                           const Vector3& vin,
                                           const Vector3& normal,
                                           Sampler& sampler,
                                           Float* cos_ni,
                                           Float* cos_mi,
                                           Float* cos_nm)
{
  // Change of basis of the incident vector
  const auto transformation = makeChangeOfBasisMatrixToLocal(normal);
  const auto incident_vector = transformation * -vin;
  ZISC_ASSERT(isUnitVector(incident_vector), 
              "Incident vector is not unit vector.");

  auto m_normal = 
#if defined(_NANAIRO_GGX_V_CAVITY_)
      ggx_v_cavity::sampleGgxMicrofacetNormal(roughness, 
                                              incident_vector, 
                                              sampler);
#elif defined(_NANAIRO_GGX_SMITH_)
      ggx_smith::sampleGgxMicrofacetNormal(roughness, 
                                           incident_vector, 
                                           sampler);
#else
  static_assert(false, "\"sampleGgxMicrofacetNormal\" isn't implemented.");
#endif

  *cos_ni = incident_vector[2];
  *cos_mi = zisc::dot(m_normal, incident_vector);
  *cos_nm = m_normal[2];
  ZISC_ASSERT(isBetweenZeroAndOneFloat(*cos_ni),
              "Cos theta_{ni} must be [0, 1].");
  ZISC_ASSERT(isBetweenZeroAndOneFloat(*cos_mi),
              "Cos theta_{mi} must be [0, 1].");
  ZISC_ASSERT(isBetweenZeroAndOneFloat(*cos_nm),
              "Cos theta_{nm} must be [0, 1].");

  // Calculate the pdf of the microfacet normal
  const Float g1 = evaluateGgxG1(roughness, *cos_ni, *cos_mi, *cos_nm);
  const Float d = evaluateGgxD(roughness, *cos_nm);
  const Float inverse_pdf = *cos_ni / (*cos_mi * d * g1);
  ZISC_VALUE_ASSERT(0.0 < inverse_pdf, d, "PDF must be positive.");

  m_normal = transformation.transposedMatrix() * m_normal;
  ZISC_ASSERT(isUnitVector(m_normal),
              "Microfacet normal is not unit vector.");
  return SampledDirection{m_normal, inverse_pdf};
}

namespace ggx_v_cavity {

/*!
  \details
  No detailed.
  */
//SampledDirection sampleGgxMicrofacetNormal(const Float roughness,
//                                           const Vector3& vin,
//                                           Sampler& sampler,
//                                           Float* cos_mi)
//{
//  const Float u1 = sampler.sample(0.0, 1.0);
//  const Float u2 = sampler.sample(0.0, 1.0);
//
//  // Sample microfacet normal
//  const Float t = roughness * zisc::sqrt(u1 / (1.0 - u1));
//  const Float phi = 2.0 * zisc::kPi * u2;
//  const Float x = t * zisc::cos(phi);
//  const Float y = t * zisc::sin(phi);
//  auto microfacet_normal = Vector3{-x, -y, 1.0}.normalized();
//  Vector3 reverse_normal{-microfacet_normal[0],
//                         -microfacet_normal[1],
//                         microfacet_normal[2]};
//
//  // Swap normal
//  Float cos_theta_mi = zisc::dot(vin, microfacet_normal);
//  const Float cos_theta_hi = zisc::dot(vin, reverse_normal);
//  const Float clamped_cos_mi = zisc::clamp(cos_theta_mi, 0.0, 1.0);
//  const Float clamped_cos_hi = zisc::clamp(cos_theta_hi, 0.0, 1.0);
//  const Float u = sampler.sample(0.0, 1.0);
//  if (u < (clamped_cos_hi / (clamped_cos_mi + clamped_cos_hi))) {
//    microfacet_normal = reverse_normal;
//    cos_theta_mi = cos_theta_hi;
//  }
//  NANAIRO_CHECK_UNIT_VECTOR(microfacet_normal);
//
//  *cos_mi = cos_theta_mi;
//
//  //! \todo Calculate microfacet normal PDF
//  constexpr Float inverse_pdf = 1.0;
//  return SampledDirection{microfacet_normal, inverse_pdf};
//}

} // namespace ggx_v_cavity

namespace ggx_smith {

/*!
  \details
  No detailed.
  */
std::tuple<Float, Float> sampleGgxSlopeXY(const Float cos_theta,
                                          Sampler& sampler)
{
  const Float u1 = sampler.sample(0.0, 1.0);
  const Float u2 = sampler.sample(0.0, 1.0);

  // Special case (normal incidence)
  constexpr Float threshold = 0.99999999;
  if (threshold < cos_theta) {
    const Float r = zisc::sqrt(u1 / (1.0 - u1));
    const Float phi = 2.0 * zisc::kPi * u2;
    return std::make_pair(r * zisc::cos(phi), r * zisc::sin(phi));
  }

  // Precomputations
  const Float tan_theta2 = 1.0 / (cos_theta * cos_theta) - 1.0;
  const Float tan_theta = zisc::sqrt(tan_theta2);

  // Sample slope_x
  const Float a = u1 * (1.0 + zisc::sqrt(1.0 + tan_theta2)) - 1.0;
  const Float b = tan_theta;
  const Float tmp = 1.0 / (a * a - 1.0);
  const Float c = b * tmp;
  const Float d = zisc::sqrt(c * c - (a * a - b * b) * tmp);
  const Float slope_x1 = c - d;
  const Float slope_x2 = c + d;
  const Float slope_x = (a < 0.0 || (1.0 / tan_theta) < slope_x2)
      ? slope_x1
      : slope_x2;

  // Sample slope_y
  const Float t = (0.5 < u2) 
      ? 2.0 * (u2 - 0.5) 
      : 2.0 * (0.5 - u2);
  Float z = (t * (t * (t * 0.27385 - 0.73369) + 0.46341)) / 
            (t * (t * (t * 0.093073 + 0.309420) - 1.0) + 0.597999);
  z = (0.5 < u2) ? z : -z;
  const Float slope_y = z * zisc::sqrt(1.0 + slope_x * slope_x);

  return std::make_tuple(slope_x, slope_y);
}

/*!
  \details
  No detailed.
  */
Vector3 sampleGgxMicrofacetNormal(const Float roughness,
                                  const Vector3& vin,
                                  Sampler& sampler)
{
  // Stretch the incident vector
  const auto vin_dash = stretchGgxMicrosurface(roughness, vin);

  // Sample slope x, y
  const Float cos_theta = vin_dash[2];
  const auto slope = sampleGgxSlopeXY(cos_theta, sampler);

  // Rotate and unstretch the slope
  Float cos_phi = 1.0;
  Float sin_phi = 0.0;
  constexpr Float threshold = 0.99999999;
  if (cos_theta < threshold) {
    const Float sin_theta = zisc::sqrt(1.0 - cos_theta * cos_theta);
    const Float inverse_sin_theta = 1.0 / sin_theta;
    cos_phi = vin_dash[0] * inverse_sin_theta;
    sin_phi = vin_dash[1] * inverse_sin_theta;
  }
  const Float slope_x = std::get<0>(slope);
  const Float slope_y = std::get<1>(slope);
  const Float x_m = roughness * (cos_phi * slope_x - sin_phi * slope_y);
  const Float y_m = roughness * (sin_phi * slope_x + cos_phi * slope_y);

  // Compute a microfacet normal
  auto microfacet_normal = Vector3{-x_m, -y_m, 1.0}.normalized();
  return microfacet_normal;
}

} // namespace ggx_smith

} // namespace nanairo
