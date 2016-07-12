/*!
  \file ggx_dielectric_bsdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GGX_DIELECTRIC_BSDF_INL_HPP
#define NANAIRO_GGX_DIELECTRIC_BSDF_INL_HPP

#include "ggx_dielectric_bsdf.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/microfacet_ggx.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/floating_point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
GgxDielectricBsdf<kSampleSize>::GgxDielectricBsdf(const Float roughness,
                                                  const Float n) noexcept :
    roughness_{roughness},
    n_{n}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
Float GgxDielectricBsdf<kSampleSize>::evaluatePdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept
{
  const Float cos_theta_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_theta_no);
  const Float pdf = (is_reflection)
      ? evaluateGgxDielectricReflectionPdf(roughness_, *vin, *vout, normal, n_)
      : evaluateGgxDielectricRefractionPdf(roughness_, *vin, *vout, normal, n_);
  return pdf;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto GgxDielectricBsdf<kSampleSize>::evaluateRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& wavelengths) const noexcept -> Spectra
{
  const Float cos_theta_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_theta_no);
  const Float f = (is_reflection)
      ? evaluateGgxReflectance(roughness_, *vin, *vout, normal, n_, 
                               cos_theta_no)
      : evaluateGgxTransmittance(roughness_, *vin, *vout, normal, n_,
                                 cos_theta_no);

  Spectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  return radiance;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto GgxDielectricBsdf<kSampleSize>::evaluateRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& wavelengths) const noexcept -> std::tuple<Spectra, Float>
{
  const Float cos_theta_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_theta_no);
  Float pdf = 0.0;
  const Float f = (is_reflection)
      ? evaluateGgxReflectance(roughness_, *vin, *vout, normal, n_, 
                               cos_theta_no, &pdf)
      : evaluateGgxTransmittance(roughness_, *vin, *vout, normal, n_,
                                 cos_theta_no, &pdf);

  Spectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  return std::make_tuple(std::move(radiance), pdf);
}

template <uint kSampleSize>
auto GgxDielectricBsdf<kSampleSize>::sample(
    const Vector3* vin,
    const Vector3& normal,
    const Wavelengths& wavelengths,
    Sampler& sampler) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  // Sample a microfacet normal
  Float cos_theta_ni = 0.0,
        cos_theta_mi = 0.0,
        cos_theta_nm = 0.0;
  const auto m_normal =
      sampleGgxMicrofacetNormal(roughness_, *vin, normal, sampler,
                                &cos_theta_ni, &cos_theta_mi, &cos_theta_nm);
  ZISC_ASSERT(0.0 <= cos_theta_ni * cos_theta_mi,
              "Microfacet normal must be in the same hemisphere as normal.");

  // Evaluate the fresnel term
  const auto result = evaluateFresnelG(n_, cos_theta_mi);
  const bool is_not_perfect_reflection = std::get<0>(result);
  const Float g = std::get<1>(result);
  const Float fresnel = (is_not_perfect_reflection)
      ? solveFresnelDielectricEquation(cos_theta_mi, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(isBetweenZeroAndOneFloat(fresnel),
              "Fresnel reflectance must be [0, 1].");

  // Determine a reflection or a refraction
  const bool is_reflection = (sampler.sample(0.0, 1.0) < fresnel);
  auto vout = (is_reflection)
      ? getMicrofacetReflectionDirection(*vin, m_normal, cos_theta_mi)
      : getMicrofacetRefractionDirection(*vin, m_normal, cos_theta_mi, n_, g);
  vout.setInversePdf((is_reflection)
      ? vout.inversePdf() / fresnel
      : vout.inversePdf() / (1.0 - fresnel));

  // Evaluate the weight
  const Float cos_theta_no = zisc::dot(normal, vout.direction());
  const Float cos_theta_mo = zisc::dot(m_normal.direction(), vout.direction());
  const Float w = evaluateGgxWeight(roughness_, cos_theta_ni, cos_theta_no,
                                    cos_theta_mi, cos_theta_mo, cos_theta_nm);
  ZISC_ASSERT(!isNegativeFloat(w), "Weight must not contain negative.");
  Spectra weight{wavelengths};
  weight.setIntensity(wavelengths.primaryWavelengthIndex(), w);

  return std::make_tuple(std::move(vout), std::move(weight));
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool GgxDielectricBsdf<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return true;
}

} // namespace nanairo

#endif // NANAIRO_GGX_DIELECTRIC_BSDF_INL_HPP
