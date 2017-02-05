/*!
  \file layered_diffuse.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "layered_diffuse.hpp"
// Standard C++ library
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "microfacet_ggx.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  */
Float LayeredDiffuse::evalPdf(const Float roughness,
                              const Vector3& vin,
                              const Vector3& vout,
                              const Vector3& normal,
                              const Float n,
                              const Float k_d,
                              const Float ri) noexcept
{
  // Calculate the probability of the reflection direction sampler
  const Float rs = toRe(n, ri);
  const Float rb = calcTotalBodyReflectance(n, k_d, ri);
  const Float ps = rs / (rs + rb); // The probability of glossy term
  // Calculate the pdf
  const Float cos_no = zisc::dot(normal, vout);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_no, 0.0, 1.0),
              "The cos(no) isn't [0, 1].");
  const Float pdf_s = MicrofacetGgx::evalReflectionPdf(roughness,
                                                       vin,
                                                       vout,
                                                       normal);
  const Float pdf_b = evalBodyPdf(cos_no);
  const Float pdf = ps * pdf_s + (1.0 - ps) * pdf_b;
  return pdf;
}

/*!
  */
Float LayeredDiffuse::evalReflectance(const Float roughness,
                                      const Vector3& vin,
                                      const Vector3& vout,
                                      const Vector3& normal,
                                      const Float n,
                                      const Float k_d,
                                      const Float ri,
                                      Sampler& sampler,
                                      Float* pdf) noexcept
{
  Float pdf_s = 0.0,
        pdf_b = 0.0;
  Float* p_pdf_s = (pdf != nullptr) ? &pdf_s : nullptr;
  const Float f_s = MicrofacetGgx::evalReflectance(roughness, vin, vout, normal,
                                                   n, p_pdf_s);
  Float* p_pdf_b = (pdf != nullptr) ? &pdf_b : nullptr;
  const Float f_b = evalBodyReflectance(roughness, vin, vout, normal,
                                        n, k_d, ri, sampler, p_pdf_b);
  const Float f = f_s + f_b;

  // Calculate the pdf
  if (pdf != nullptr) {
    // Calculate the probability of the reflection direction sampler
    const Float rs = toRe(n, ri);
    const Float rb = calcTotalBodyReflectance(n, k_d, ri);
    const Float ps = rs / (rs + rb); // The probability of glossy term
    // Evaluate the pdf
    *pdf = ps * pdf_s + (1.0 - ps) * pdf_b;
  }

  return f;
}

/*!
  */
std::tuple<SampledDirection, Float> LayeredDiffuse::sample(const Float roughness,
                                                           const Vector3& vin,
                                                           const Vector3& normal,
                                                           const Float n,
                                                           const Float k_d,
                                                           const Float ri,
                                                           Sampler& sampler) noexcept
{
  // Sample a reflection direction
  const auto sampled_vout = sampleReflectionDirection(roughness, vin, normal,
                                                      n, k_d, ri, sampler);
  const auto& vout = sampled_vout.direction();
  Float weight = 0.0;
  const Float cos_no = zisc::dot(normal, vout);
  if (0.0 < cos_no) {
    // Evaluate the reflectance
    const Float f_s = MicrofacetGgx::evalReflectance(roughness, vin, vout, normal, n);
    const Float f_b = evalBodyReflectance(roughness, vin, vout, normal,
                                          n, k_d, ri, sampler);
    const Float f = f_s + f_b;
    // Calculate the direction weight
    weight = (f * cos_no) * sampled_vout.inversePdf();
  }
  return std::make_tuple(std::move(sampled_vout), weight);
}

/*!
  */
Float LayeredDiffuse::calcRe(const Float n) noexcept
{
  const Float n2 = zisc::power<2>(n);
  const Float n3 = zisc::power<3>(n);
  const Float n4 = zisc::power<2>(n2);
  // Calculate the internal reflectance
  const Float term1 = (2.0 * n3 * (n2 + 2.0 * n - 1.0)) /
                      ((n2 + 1.0) * (n4 - 1.0));
  const Float term2 = ((n - 1.0) * (3.0 * n + 1.0)) /
                      (6.0 * zisc::power<2>(n + 1.0));
  const Float term3 = std::log(n) *
                      (8.0 * n4 * (n4 + 1.0)) /
                      ((n2 + 1.0) * zisc::power<2>(n4 - 1.0));
  const Float term4 = std::log((n - 1.0) / (n + 1.0)) *
                      (n2 * zisc::power<2>(n2 - 1.0)) /
                      zisc::power<3>(n2 + 1.0);
  const Float re = 0.5 - term1 + term2 + term3 + term4;
  ZISC_ASSERT(zisc::isInClosedBounds(re, 0.0, 1.0),
              "The internal reflectance is out of range [0, 1]: ", re);
  return re;
}

/*!
  */
Float LayeredDiffuse::evalBodyReflectance(const Float roughness,
                                          const Vector3& vin,
                                          const Vector3& vout,
                                          const Vector3& normal,
                                          const Float n,
                                          const Float k_d,
                                          const Float ri,
                                          Sampler& sampler,
                                          Float* pdf) noexcept
{
  // Sample a microfacet normal
  const auto m_normal = MicrofacetGgx::sampleNormal(roughness,
                                                    vin,
                                                    normal,
                                                    sampler,
                                                    false);
  const Float cos_no = zisc::dot(normal, vout),
              cos_mo = zisc::dot(m_normal.direction(), vout);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_no, 0.0, 1.0),
              "The cos(no) isn't [0, 1].");
  Float f = 0.0;
  if (0.0 < cos_mo) {
    // Evaluate the weight of microfacet
    const Float cos_ni = -zisc::dot(normal, vin),
                cos_mi = -zisc::dot(m_normal.direction(), vin);
    const Float cos_nm = zisc::dot(normal, m_normal.direction());
    ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0),
                "The cos(ni) isn't [0, 1].");
    ZISC_ASSERT(zisc::isInClosedBounds(cos_mi, 0.0, 1.0),
                "The cos(mi) isn't [0, 1].");
//    ZISC_ASSERT(zisc::isInClosedBounds(cos_nm, 0.0, 1.0),
//                "The cos(nm) isn't [0, 1].");
    const Float w = MicrofacetGgx::evalWeight(roughness,
                                              cos_ni, cos_no,
                                              cos_mi, cos_mo,
                                              cos_nm);
    // Evaluate the pure body reflectance
    const Float fb = evalPureBodyReflectance(vin, vout, normal, n, k_d, ri);
    // Evaluate the body reflectance
    f = (cos_mo / cos_no) * fb * w;
    // Calculate the pdf of the reflection direction
  }
  if (pdf != nullptr)
    *pdf = evalBodyPdf(cos_no);
  return f;
}

/*!
  */
Float LayeredDiffuse::evalPureBodyReflectance(const Vector3& vin,
                                              const Vector3& vout,
                                              const Vector3& normal,
                                              const Float n,
                                              const Float k_d,
                                              const Float ri) noexcept
{
  const Float cos_ni = -zisc::dot(normal, vin);
  const Float cos_no = zisc::dot(normal, vout);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0),
              "The cos(ni) isn't [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_no, 0.0, 1.0),
              "The cos(no) isn't [0, 1].");

  const Float t_i = 1.0 - Fresnel::evalFresnel(n, cos_ni);
  if (t_i == 0.0)
    return 0.0;
  const Float t_o = 1.0 - Fresnel::evalFresnel(n, cos_no);
  if (t_o == 0.0)
    return 0.0;

  constexpr Float pi = zisc::kPi<Float>;
  const Float k = k_d / (pi * zisc::power<2>(n) * (1.0 - k_d * ri));

  const auto reflectance = k * t_i * t_o;
  ZISC_ASSERT(0.0 <= reflectance, "The reflectance is minus.");
  return reflectance;
}

/*!
  */
SampledDirection LayeredDiffuse::sampleReflectionDirection(const Float roughness,
                                                           const Vector3& vin,
                                                           const Vector3& normal,
                                                           const Float n,
                                                           const Float k_d,
                                                           const Float ri,
                                                           Sampler& sampler) noexcept
{
  // Calculate the probability of the reflection direction sampler
  const Float rs = toRe(n, ri);
  const Float rb = calcTotalBodyReflectance(n, k_d, ri);
  const Float ps = rs / (rs + rb); // The probability of glossy term
  // Sample a reflection direction
  SampledDirection sampled_vout;
  const Float u = sampler.sample();
  if (u < ps) { // Glossy term sampling
    const auto m_normal = MicrofacetGgx::sampleNormal(roughness,
                                                      vin,
                                                      normal,
                                                      sampler);
    sampled_vout = Microfacet::calcReflectionDirection(vin, m_normal);
    const auto& vout = sampled_vout.direction();
    Float pdf = 0.0;
    const Float cos_no = zisc::dot(normal, vout);
    if (0.0 < cos_no) {
      const Float pdf_s = sampled_vout.pdf();
      const Float pdf_b = evalBodyPdf(cos_no);
      pdf = ps * pdf_s + (1.0 - ps) * pdf_b;
    }
    sampled_vout.setPdf(pdf);
  }
  else { // Body term sampling
    sampled_vout = SampledDirection::sampleOnHemisphere<1>(normal, sampler);
    const auto& vout = sampled_vout.direction();
    const Float pdf_b = sampled_vout.pdf();
    const Float pdf_s = MicrofacetGgx::evalReflectionPdf(roughness,
                                                         vin,
                                                         vout,
                                                         normal);
    const Float pdf = ps * pdf_s + (1.0 - ps) * pdf_b;
    sampled_vout.setPdf(pdf);
  }
  return sampled_vout;
}

} // namespace nanairo
