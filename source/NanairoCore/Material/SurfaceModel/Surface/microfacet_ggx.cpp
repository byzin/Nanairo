/*!
  \file microfacet_ggx.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
Float MicrofacetGgx::evalReflectance(const Float roughness_x,
                                     const Float roughness_y,
                                     const Vector3& vin,
                                     const Vector3& vout,
                                     const Vector3& m_normal,
                                     const Float fresnel,
                                     Float* pdf) noexcept
{
  if (fresnel == 0.0)
    return 0.0;

  // Evaluate D
  const Float d = evalD(roughness_x, roughness_y, m_normal);
  if (d == 0.0)
    return 0.0;

  // Evaluate G2
  const Float g2 = evalG2(roughness_x, roughness_y, vin, vout, m_normal);
  if (g2 == 0.0)
    return 0.0;

  // Calculate the reflectance
  const Float cos_ni = vin[2];
  const Float cos_no = vout[2];
  const Float f = (fresnel * g2 * d) / (4.0 * cos_ni * cos_no);
  ZISC_ASSERT(0.0 <= f, "The reflectance isn't positive.");

  // Calculate the reflection pdf
  if (pdf != nullptr)
    *pdf = calcReflectionPdf(roughness_x, roughness_y, vin, m_normal, d);

  return f;
}

/*!
  */
SampledSpectra MicrofacetGgx::evalReflectance(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& vin,
    const Vector3& vout,
    const Vector3& m_normal,
    const SampledSpectra& n,
    const SampledSpectra& eta,
    Float* pdf) noexcept
{
  const auto& wavelengths = n.wavelengths();

  // Evaluate D
  const Float d = evalD(roughness_x, roughness_y, m_normal);
  if (d == 0.0)
    return SampledSpectra{wavelengths};

  // Evaluate G2
  const Float g2 = evalG2(roughness_x, roughness_y, vin, vout, m_normal);
  if (g2 == 0.0)
    return SampledSpectra{wavelengths};

  // Evaluate the fresnel 
  const Float cos_mi = zisc::dot(m_normal, vin);
  const auto fresnel = Fresnel::evalFresnel(n, eta, cos_mi);

  // Calculate the reflectance
  const Float cos_ni = vin[2];
  const Float cos_no = vout[2];
  const auto f = fresnel * (g2 * d / (4.0 * cos_ni * cos_no));
  ZISC_ASSERT(!f.hasNegative(), "The f has negative value.");

  // Calculate the reflection pdf
  if (pdf != nullptr)
    *pdf = calcReflectionPdf(roughness_x, roughness_y, vin, m_normal, d);

  return f;
}

/*!
  */
Float MicrofacetGgx::evalTransmittance(const Float roughness_x,
                                       const Float roughness_y,
                                       const Vector3& vin,
                                       const Vector3& vout,
                                       const Vector3& m_normal,
                                       const Float n,
                                       const Float fresnel,
                                       Float* pdf) noexcept
{
  if (fresnel == 1.0)
    return 0.0;

  // Evaluate D
  const Float d = evalD(roughness_x, roughness_y, m_normal);
  if (d == 0.0)
    return 0.0;

  // Evaluate G2
  const Float g2 = evalG2(roughness_x, roughness_y, vin, vout, m_normal);
  if (g2 == 0.0)
    return 0.0;

  // Calculate the transmittance
  const Float cos_ni = vin[2];
  const Float cos_no = vout[2];
  const Float cos_mi = zisc::dot(m_normal, vin);
  const Float cos_mo = zisc::dot(m_normal, vout);
  const Float t = (zisc::abs(cos_mi * cos_mo) * zisc::power<2>(n)) /
                  (zisc::abs(cos_ni * cos_no) * zisc::power<2>(cos_mi + n * cos_mo));
  const Float f = t * (1.0 - fresnel) * g2 * d;
  ZISC_ASSERT(0.0 < f, "The transmittance isn't positive.");

  // Calculate the refraction pdf
  if (pdf != nullptr)
    *pdf = calcRefractionPdf(roughness_x, roughness_y, vin, vout, m_normal, n, d);

  return f;
}

/*!
  \details
  No detailed.
  */
SampledDirection MicrofacetGgx::sampleNormal(const Float roughness_x,
                                             const Float roughness_y,
                                             const Vector3& vin,
                                             Sampler& sampler,
                                             const bool calc_pdf) noexcept
{
  auto m_normal = MicrofacetGgx::sampleMicrofacetNormal(roughness_x,
                                                        roughness_y,
                                                        vin,
                                                        sampler);

  // Calculate the pdf of the microfacet normal
  Float inverse_pdf = 0.0;
  if (calc_pdf) {
    const Float cos_ni = vin[2];
    const Float cos_mi = zisc::dot(m_normal, vin);
    ZISC_ASSERT(zisc::isInClosedBounds(cos_mi, 0.0, 1.0), "cos_mi isn't [0, 1].");
    const Float d = MicrofacetGgx::evalD(roughness_x, roughness_y, m_normal);
    const Float g1 = MicrofacetGgx::evalG1(roughness_x, roughness_y, vin, m_normal);
    inverse_pdf = cos_ni / (cos_mi * d * g1);
    ZISC_ASSERT(0.0 < inverse_pdf, "The pdf is negative.");
  }

  return SampledDirection{m_normal, inverse_pdf};
}

/*!
  */
template <>
Vector3 MicrofacetGgx::SmithGgxMicrosurface::sampleMicrofacetNormal(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& vin,
    Sampler& sampler) noexcept
{
  // Stretch the incident vector
  const auto v = Vector3{roughness_x * vin[0],
                         roughness_y * vin[1],
                         vin[2]}.normalized();

  // Orthonormal basis
  const auto t1 = (v[2] != 0.0)
      ? Vector3{v[1], -v[0], 0.0}.normalized()
      : Vector3{1.0, 0.0, 0.0};
  ZISC_ASSERT(zisc::dot(t1, v) < 1.0e-7, "The t1 isn't orthogonal to the v.");
  const auto t2 = zisc::cross(t1, v);
  ZISC_ASSERT(isUnitVector(t2), "The t2 isn't unit vector.");

  // Sample point with polar coordinate
  const Float u1 = sampler.sample();
  const Float u2 = sampler.sample();
  const Float a = zisc::invert(1.0 + v[2]);
  const Float r = zisc::sqrt(u1);
  const Float phi = (u2 < a)
      ? (u2 / a) * zisc::kPi<Float>
      : (1.0 + (u2 - a) / (1.0 - a)) * zisc::kPi<Float>;
  const Float p1 = r * zisc::cos(phi);
  const Float p2 = r * zisc::sin(phi) * ((u2 < a) ? 1.0 : v[2]);

  // Compute normal
  const Float p3 = zisc::sqrt(
      zisc::max(0.0, 1.0 - zisc::power<2>(p1) - zisc::power<2>(p2)));
  Vector3 n = p1 * t1 + p2 * t2 + p3 * v;

  // Unstretch the normal
  n[0] = roughness_x * n[0];
  n[1] = roughness_y * n[1];
  n[2] = zisc::max(0.0, n[2]);
  n = n.normalized();

  ZISC_ASSERT(zisc::isInClosedBounds(n[2], 0.0, 1.0),
              "The microfacet normal isn't in the upper hemisphere.");

  return n;
}

/*!
  */
inline
Vector3 MicrofacetGgx::sampleMicrofacetNormal(const Float roughness_x,
                                              const Float roughness_y,
                                              const Vector3& vin,
                                              Sampler& sampler) noexcept
{
  using MSurface = GgxMicrosurface<kMicrosurface>;
  return MSurface::sampleMicrofacetNormal(roughness_x, roughness_y, vin, sampler);
}

} // namespace nanairo
