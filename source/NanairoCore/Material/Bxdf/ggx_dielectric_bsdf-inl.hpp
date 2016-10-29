/*!
  \file ggx_dielectric_bsdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/microfacet_ggx.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

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
Float GgxDielectricBsdf<kSampleSize>::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept
{
  const Float cos_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_no);
  const Float pdf = (is_reflection)
      ? MicrofacetGgx::evalReflectionPdf(roughness_, *vin, *vout, normal, n_)
      : MicrofacetGgx::evalRefractionPdf(roughness_, *vin, *vout, normal, n_);
  return pdf;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto GgxDielectricBsdf<kSampleSize>::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& wavelengths) const noexcept -> Spectra
{
  const Float cos_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_no);
  const Float f = (is_reflection)
      ? MicrofacetGgx::evalReflectance(roughness_, *vin, *vout, normal, n_, cos_no)
      : MicrofacetGgx::evalTransmittance(roughness_, *vin, *vout, normal, n_, cos_no);

  Spectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  return radiance;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto GgxDielectricBsdf<kSampleSize>::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& wavelengths) const noexcept -> std::tuple<Spectra, Float>
{
  const Float cos_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_no);
  Float pdf = 0.0;
  const Float f = (is_reflection)
      ? MicrofacetGgx::evalReflectance(roughness_, *vin, *vout, normal, n_, cos_no, &pdf)
      : MicrofacetGgx::evalTransmittance(roughness_, *vin, *vout, normal, n_, cos_no, &pdf);

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
  const auto sampled_m_normal =
      SampledGgxNormal::sample(roughness_, *vin, normal, sampler);
  const Float cos_ni = sampled_m_normal.cosNi(),
              cos_mi = sampled_m_normal.cosMi(),
              cos_nm = sampled_m_normal.cosNm();
  const auto& m_normal = sampled_m_normal.microfacetNormal();
  ZISC_ASSERT(0.0 <= cos_ni * cos_mi,
              "Microfacet normal isn't in the same hemisphere as normal.");

  // Evaluate the fresnel term
  const auto result = Fresnel::evalG(n_, cos_mi);
  const bool is_perfect_reflection = !std::get<0>(result);
  const Float g = std::get<1>(result);
  const Float fresnel = (!is_perfect_reflection)
      ? Fresnel::evalDielectricEquation(cos_mi, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(zisc::isInClosedBounds(fresnel, 0.0, 1.0),
              "Fresnel reflectance isn't [0, 1].");

  // Determine a reflection or a refraction
  const bool is_reflection = (sampler.sample(0.0, 1.0) < fresnel);
  auto vout = (is_reflection)
      ? Microfacet::calcReflectionDirection(*vin, m_normal, cos_mi)
      : Microfacet::calcRefractionDirection(*vin, m_normal, cos_mi, n_, g);
  vout.setInversePdf((is_reflection)
      ? vout.inversePdf() / fresnel
      : vout.inversePdf() / (1.0 - fresnel));

  // Evaluate the weight
  const Float cos_no = zisc::dot(normal, vout.direction());
  const Float cos_mo = zisc::dot(m_normal.direction(), vout.direction());
  const Float w = MicrofacetGgx::evalWeight(roughness_, cos_ni, cos_no,
                                            cos_mi, cos_mo, cos_nm);
  ZISC_ASSERT(0.0 <= w, "The weight is negative.");
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
