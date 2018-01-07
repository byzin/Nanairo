/*!
  \file layered_diffuse.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "layered_diffuse.hpp"
// Standard C++ library
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "diffuse.hpp"
#include "fresnel.hpp"
#include "microfacet.hpp"
#include "microfacet_ggx.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
Float LayeredDiffuse::evalPdf(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& vin,
    const Vector3& vout,
    const Float n,
    const Float k_d,
    const Float re) noexcept
{
  const Float ri = toRi(n, re);
  // Calculate the microfacet normal
  const auto m_normal = Microfacet::calcReflectionHalfVector(vin, vout);
  // Evaluate the pdf of the glossy term
  const Float pdf_s = MicrofacetGgx::evalReflectionPdf(roughness_x,
                                                       roughness_y,
                                                       vin,
                                                       m_normal);
  // Evaluate the pdf of the body term
  const Float cos_no = vout[2];
  const Float pdf_b = evalBodyPdf(cos_no);
  // Evaluate the pdf of the reflection direction
  const Float rs = re;
  const Float rb = calcTotalBodyReflectance(n, k_d, ri);
  const Float ps = rs / (rs + rb); // The probability of the glossy term
  const Float pdf = ps * pdf_s + (1.0 - ps) * pdf_b;
  ZISC_ASSERT(0.0 <= pdf,
              "The pdf is negative: PDF=", pdf, " PDFs=", pdf_s, " PDFb=", pdf_b);
  return pdf;
}

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
Float LayeredDiffuse::evalReflectance(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& vin,
    const Vector3& vout,
    const Float n,
    const Float k_d,
    const Float re,
    Sampler& sampler,
    Float* pdf) noexcept
{
  const Float ri = toRi(n, re);
  // Calculate the microfacet normal
  const auto m_normal = Microfacet::calcReflectionHalfVector(vin, vout);
  // Evaluate the reflectance and the pdf of the glossy term
  const Float cos_mi = zisc::dot(m_normal, vin);
  const Float fresnel = Fresnel::evalFresnel(n, cos_mi);
  const Float f_s = MicrofacetGgx::evalReflectance(roughness_x,
                                                   roughness_y,
                                                   vin,
                                                   vout,
                                                   m_normal,
                                                   fresnel,
                                                   pdf);
  const Float pdf_s = (pdf != nullptr) ? *pdf : 0.0;
  // Evaluate the reflectance and the pdf of the body term
  const Float f_b = evalBodyReflectance(roughness_x,
                                        roughness_y,
                                        vin,
                                        vout,
                                        n,
                                        k_d,
                                        ri,
                                        sampler,
                                        pdf);
  const Float pdf_b = (pdf != nullptr) ? *pdf : 0.0;
  // Evaluate the pdf of the reflection direction
  if (pdf != nullptr) {
    const Float rs = re;
    const Float rb = calcTotalBodyReflectance(n, k_d, ri);
    const Float ps = rs / (rs + rb); // The probability of the glossy term
    *pdf = ps * pdf_s + (1.0 - ps) * pdf_b;
    ZISC_ASSERT(0.0 <= *pdf,
                "The pdf is negative: PDF=", *pdf, " PDFs=", pdf_s, " PDFb=", pdf_b);
  }
  const Float f = f_s + f_b;
  return f;
}

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
std::tuple<SampledDirection, Float> LayeredDiffuse::sample(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& vin,
    const Float n,
    const Float k_d,
    const Float re,
    Sampler& sampler) noexcept
{
  const Float ri = toRi(n, re);
  const Float u = sampler.sample();
  // Calculate the probability of the reflection direction sampler
  const Float rs = re;
  const Float rb = calcTotalBodyReflectance(n, k_d, ri);
  const Float ps = rs / (rs + rb); // The probability of the glossy term
  // Sample a reflection direction
  Vector3 m_normal;
  SampledDirection sampled_vout;
  if (u < ps) { // Glossy term sampling
    // Sample a reflection direction
    const auto sampled_m_normal = MicrofacetGgx::sampleNormal(roughness_x,
                                                              roughness_y,
                                                              vin,
                                                              sampler,
                                                              false);
    sampled_vout = Microfacet::calcReflectionDirection(vin, sampled_m_normal);
    // Store the microfacet normal
    m_normal = sampled_m_normal.direction();
  }
  else { // Body term sampling
    // Sample a reflection direction
    sampled_vout = Diffuse::sample(sampler);
    // Store the microfacet normal
    const auto& vout = sampled_vout.direction();
    m_normal = Microfacet::calcReflectionHalfVector(vin, vout);
  }

  const auto& vout = sampled_vout.direction();
  const Float cos_no = vout[2];
  Float weight = 0.0;
  if (0.0 < cos_no) {
    // Evaluate the reflectance and the pdf of the glossy term
    const Float cos_mi = zisc::dot(m_normal, vin);
    const Float fresnel = Fresnel::evalFresnel(n, cos_mi);
    Float pdf_s = 0.0;
    const Float f_s = MicrofacetGgx::evalReflectance(roughness_x,
                                                     roughness_y,
                                                     vin,
                                                     vout,
                                                     m_normal,
                                                     fresnel,
                                                     &pdf_s);
    // Evaluate the reflectance and the pdf of the body term
    Float pdf_b = 0.0;
    const Float f_b = evalBodyReflectance(roughness_x,
                                          roughness_y,
                                          vin,
                                          vout,
                                          n,
                                          k_d,
                                          ri,
                                          sampler,
                                          &pdf_b);
    // Evaluate the pdf of the reflection direction
    const Float pdf = ps * pdf_s + (1.0 - ps) * pdf_b;
    ZISC_ASSERT(0.0 <= pdf,
                "The pdf is negative: PDF=", pdf, " PDFs=", pdf_s, " PDFb=", pdf_b);
    sampled_vout.setPdf(pdf);
    // Evaluate the weight of the reflection
    weight = (f_s + f_b) * cos_no * sampled_vout.inversePdf();
    ZISC_ASSERT(0.0 <= weight, "The weight is negative.");
  }
  else {
    sampled_vout.setPdf(0.0);
  }

  return std::make_tuple(sampled_vout, weight);
}

/*!
  /details
  n = n_transmission_side / n_incident_side
  */
Float LayeredDiffuse::calcRe(const Float n) noexcept
{
  // values
  const Float n2 = zisc::power<2>(n);
  const Float n4 = zisc::power<2>(n2);
  const Float a = n + 1.0;
  const Float b = n - 1.0;
  const Float c = n2 + 1.0;
  const Float d = n2 - 1.0;
  const Float e = n4 - 1.0;
  constexpr Float k = zisc::invert(6.0);

  // Calculate the internal reflectance
  const Float t1 = 2.0 * (n * n2) * e * (zisc::power<2>(a) - 2.0);
  const Float t2 = k * zisc::power<3>(c * b) * (3.0 * n + 1.0);
  const Float t3 = 8.0 * n4 * (n4 + 1.0) * zisc::log(n);
  const Float t4 = n2 * zisc::power<4>(d) * zisc::log(b / a);
  const Float re = 0.5 + (-t1 + t2 + t3 + t4) / (c * zisc::power<2>(e));
  ZISC_ASSERT(zisc::isInClosedBounds(re, 0.0, 1.0),
              "The external reflectance is out of range [0, 1]: ", re);

  return re;
}

/*!
  \details
  n = n_transmission_side / n_incident_side
  */
Float LayeredDiffuse::evalBodyReflectance(
    const Float roughness_x,
    const Float roughness_y,
    const Vector3& vin,
    const Vector3& vout,
    const Float n,
    const Float k_d,
    const Float ri,
    Sampler& sampler,
    Float* pdf) noexcept
{
  const Float cos_no = vout[2];
  // Evaluate the pdf
  if (pdf != nullptr)
    *pdf = evalBodyPdf(cos_no);
  // Sample a microfacet normal
  const auto sampled_m_normal = MicrofacetGgx::sampleNormal(roughness_x,
                                                            roughness_y,
                                                            vin,
                                                            sampler,
                                                            false);
  const auto& m_normal = sampled_m_normal.direction();
  const Float cos_mo = zisc::dot(m_normal, vout);
  Float fb = 0.0;
  if (0.0 < cos_mo) {
    // Evaluate the reflectance
    const Float cos_mi = zisc::dot(m_normal, vin);
    fb = calcPureBodyReflectance(cos_mi, cos_mo, n, k_d, ri);
    if (fb != 0.0) {
      const Float w = MicrofacetGgx::evalWeight(roughness_x,
                                                roughness_y,
                                                vin,
                                                vout,
                                                m_normal);
      fb = (cos_mo / cos_no) * fb * w;
    }
  }
  return fb;
}

/*!
  /details
  n = n_transmission_side / n_incident_side
  */
Float LayeredDiffuse::calcPureBodyReflectance(
    const Float cos_mi,
    const Float cos_mo,
    const Float n,
    const Float k_d,
    const Float ri) noexcept
{
  Float fb = 0.0;
  const Float fresnel_i = Fresnel::evalFresnel(n, cos_mi);
  if (fresnel_i < 1.0) {
    const Float fresnel_o = Fresnel::evalFresnel(n, cos_mo);
    if (fresnel_o < 1.0) {
      constexpr Float pi = zisc::kPi<Float>;
      const Float k = k_d / (pi * zisc::power<2>(n) * (1.0 - k_d * ri));
      fb = k * (1.0 - fresnel_i) * (1.0 - fresnel_o);
      ZISC_ASSERT(0.0 <= fb, "The reflectance is negative.");
    }
  }
  return fb;
}

} // namespace nanairo
