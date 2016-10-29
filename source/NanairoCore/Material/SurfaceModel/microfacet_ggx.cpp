/*!
  \file microfacet_ggx.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Float MicrofacetGgx::evalReflectance(const Float roughness,
                                     const Vector3& vin,
                                     const Vector3& vout,
                                     const Vector3& normal,
                                     const Float n,
                                     const Float cos_no,
                                     Float* pdf) noexcept
{
  // Calculate nanairoion half vector
  const auto m_normal = Microfacet::calcReflectionHalfVector(vin, vout);

  const Float cos_ni = -zisc::dot(normal, vin);
  const Float cos_mi = -zisc::dot(m_normal, vin);
  const Float cos_mo = cos_mi;
  const Float cos_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(zisc::isInBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");
  ZISC_ASSERT(0.0 <= cos_ni * cos_mi,
              "Microfacet normal isn't in the same hemisphere as normal.");

  // Evaluate D
  const Float d = evalD(roughness, cos_nm);
  if (d == 0.0)
    return 0.0;

  // Evaluate G2(i, o, m)
  const Float g2 = evalG2(roughness, cos_ni, cos_no, cos_mi, cos_mo, cos_nm);
  if (g2 == 0.0)
    return 0.0;

  // Evaluate the fresnel reflectance
  const auto result = Fresnel::evalG(n, cos_mi);
  const bool is_perfect_reflection = !std::get<0>(result);
  const Float g = std::get<1>(result);
  const Float fresnel = (!is_perfect_reflection)
      ? Fresnel::evalDielectricEquation(cos_mi, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(zisc::isInClosedBounds(fresnel, 0.0, 1.0),
              "Fresnel reflectance isn't [0, 1].");
  if (fresnel == 0.0)
    return 0.0;

  // Calculate the pdf
  if (pdf != nullptr)
    *pdf = fresnel * calcReflectionPdf(roughness, d, cos_ni, cos_mi, cos_nm);

  // Calculate reflectance
  const Float f = (fresnel * g2 * d) / (4.0 * cos_ni * cos_no);
  ZISC_ASSERT(0.0 <= f, "Reflectance isn't positive.");
  return f;
}

/*!
  \details
  No detailed.
  */
Float MicrofacetGgx::evalTransmittance(const Float roughness,
                                       const Vector3& vin,
                                       const Vector3& vout,
                                       const Vector3& normal,
                                       const Float n,
                                       const Float cos_no,
                                       Float* pdf) noexcept
{
  // Calculate refraction half vector
  const auto m_normal = Microfacet::calcRefractionHalfVector(vin, vout, n);

  const Float cos_ni = -zisc::dot(normal, vin);
  const Float cos_mi = -zisc::dot(m_normal, vin);
  const Float cos_mo = zisc::dot(m_normal, vout);
  const Float cos_nm = zisc::dot(normal, m_normal);
  ZISC_ASSERT(zisc::isInBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");
  ZISC_ASSERT(0.0 <= cos_ni * cos_mi,
              "Microfacet normal isn't in the same hemisphere as normal.");

  // Evaluate D
  const Float d = evalD(roughness, cos_nm);
  if (d == 0.0)
    return 0.0;

  // Evaluate G2(i, o, m)
  const Float g2 = evalG2(roughness, cos_ni, cos_no, cos_mi, cos_mo, cos_nm);
  if (g2 == 0.0)
    return 0.0;

  // Evaluate the fresnel reflectance
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

  // Calculate the pdf
  if (pdf != nullptr) {
    *pdf = (1.0 - fresnel) *
           calcRefractionPdf(roughness, d, cos_ni, cos_mi, cos_mo, cos_nm, n);
  }

  // Calculate transmittance
  const Float k1 = (cos_mi * cos_mo) / (cos_ni * cos_no);
  ZISC_ASSERT(0.0 < k1, "The k1 isn't positive.");
  const Float k2 = (n * n) / zisc::power<2>(cos_mi + n * cos_mo);
  const Float f = (1.0 - fresnel) * (k1 * k2) * g2 * d;
  ZISC_ASSERT(0.0 < f, "Transmittance isn't positive.");
  return f;
}

/*!
  \details
  No detailed.
  */
SampledGgxNormal SampledGgxNormal::sample(const Float roughness,
                                          const Vector3& vin,
                                          const Vector3& normal,
                                          Sampler& sampler) noexcept
{
  // Change of basis of the incident vector
  const auto transformation = Transformation::makeChangeOfBasisToLocal(normal);
  const auto incident_vector = transformation * -vin;
  ZISC_ASSERT(isUnitVector(incident_vector), "Incident vector isn't unit vector.");

  auto m_normal = sampleMicrofacetNormal(roughness, incident_vector, sampler);

  const Float cos_ni = incident_vector[2];
  const Float cos_mi = zisc::dot(m_normal, incident_vector);
  const Float cos_nm = m_normal[2];
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_mi, 0.0, 1.0), "cos_mi isn't [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_nm, 0.0, 1.0), "cos_nm isn't [0, 1].");

  // Calculate the pdf of the microfacet normal
  const Float g1 = MicrofacetGgx::evalG1(roughness, cos_ni, cos_mi, cos_nm);
  const Float d = MicrofacetGgx::evalD(roughness, cos_nm);
  const Float inverse_pdf = cos_ni / (cos_mi * d * g1);
  ZISC_ASSERT(0.0 < inverse_pdf, "The pdf is negative.");

  m_normal = transformation.transposedMatrix() * m_normal;
  ZISC_ASSERT(isUnitVector(m_normal), "Microfacet normal isn't unit vector.");
  return SampledGgxNormal{SampledDirection{m_normal, inverse_pdf},
                          cos_ni,
                          cos_mi,
                          cos_nm};
}

/*!
  \details
  No detailed.
  */
template <>
Vector3 SampledGgxNormal::Smith::sampleMicrofacetNormal(const Float roughness,
                                                        const Vector3& vin,
                                                        Sampler& sampler) noexcept
{
  // Stretch the incident vector
  const auto vin_dash = smithStretch(roughness, vin);

  // Sample slope x, y
  const Float cos_theta = vin_dash[2];
  const auto slope = smithSampleSlopeXY(cos_theta, sampler);

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

/*!
  \details
  No detailed.
  */
template <MicrofacetGgx::GgxMethodType kMethod>
std::tuple<Float, Float> SampledGgxNormal::GgxMethod<kMethod>::smithSampleSlopeXY(
    const Float cos_theta,
    Sampler& sampler) noexcept
{
  const Float u1 = sampler.sample(0.0, 1.0);
  const Float u2 = sampler.sample(0.0, 1.0);

  // Special case (normal incidence)
  constexpr Float threshold = 0.99999999;
  if (threshold < cos_theta) {
    const Float r = zisc::sqrt(u1 / (1.0 - u1));
    const Float phi = 2.0 * zisc::kPi<Float> * u2;
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
template <MicrofacetGgx::GgxMethodType kMethod> inline
Vector3 SampledGgxNormal::GgxMethod<kMethod>::smithStretch(const Float roughness,
                                                           const Vector3& vin) noexcept
{
  const Vector3 vin_dash{roughness * vin[0], roughness * vin[1], vin[2]};
  return vin_dash.normalized();
}

/*!
  */
inline
Vector3 SampledGgxNormal::sampleMicrofacetNormal(const Float roughness,
                                                 const Vector3& vin,
                                                 Sampler& sampler) noexcept
{
  using SamplerMethod = GgxMethod<MicrofacetGgx::kUsedType>;
  return SamplerMethod::sampleMicrofacetNormal(roughness, vin, sampler);
}

} // namespace nanairo
