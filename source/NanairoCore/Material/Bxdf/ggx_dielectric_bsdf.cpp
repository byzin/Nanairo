/*!
  \file ggx_dielectric_bsdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

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
GgxDielectricBsdf::GgxDielectricBsdf(const Float roughness,
                                     const Float n) noexcept :
    roughness_{roughness},
    n_{n}
{
}

/*!
  \details
  No detailed.
  */
Float GgxDielectricBsdf::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const WavelengthSamples& /* wavelengths */) const noexcept
{
  const Float cos_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_no);
  Float pdf = 0.0;
  if (is_reflection) { // Reflection direction pdf
    const auto m_normal = Microfacet::calcReflectionHalfVector(*vin, *vout);
    const Float cos_mi = -zisc::dot(m_normal, *vin);
    const Float fresnel = Fresnel::evalFresnel(n_, cos_mi);
    pdf = fresnel *
          MicrofacetGgx::evalReflectionPdf(roughness_, *vin, *vout, normal);
  }
  else { // Refraction direction pdf
    const auto m_normal = Microfacet::calcRefractionHalfVector(*vin, *vout, n_);
    const Float cos_mi = -zisc::dot(m_normal, *vin);
    const Float fresnel = Fresnel::evalFresnel(n_, cos_mi);
    pdf = (1.0 - fresnel) *
          MicrofacetGgx::evalRefractionPdf(roughness_, *vin, *vout, normal, n_);
  }
  return pdf;
}

/*!
  \details
  No detailed.
  */
SampledSpectra GgxDielectricBsdf::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const WavelengthSamples& wavelengths) const noexcept
{
  const Float cos_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_no);
  const Float f = (is_reflection)
      ? MicrofacetGgx::evalReflectance(roughness_, *vin, *vout, normal, n_)
      : MicrofacetGgx::evalTransmittance(roughness_, *vin, *vout, normal, n_);

  SampledSpectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> GgxDielectricBsdf::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const WavelengthSamples& wavelengths) const noexcept
{
  const Float cos_no = zisc::dot(normal, *vout);
  const bool is_reflection = (0.0 < cos_no);
  Float pdf = 0.0;
  const Float f = (is_reflection)
      ? MicrofacetGgx::evalReflectance(roughness_, *vin, *vout, normal, n_, &pdf)
      : MicrofacetGgx::evalTransmittance(roughness_, *vin, *vout, normal, n_, &pdf);

  SampledSpectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);

  // Calculate the pdf
  if (is_reflection) { // Reflection direction pdf
    const auto m_normal = Microfacet::calcReflectionHalfVector(*vin, *vout);
    const Float cos_mi = -zisc::dot(m_normal, *vin);
    const Float fresnel = Fresnel::evalFresnel(n_, cos_mi);
    pdf = fresnel * pdf;
  }
  else { // Refraction direction pdf
    const auto m_normal = Microfacet::calcRefractionHalfVector(*vin, *vout, n_);
    const Float cos_mi = -zisc::dot(m_normal, *vin);
    const Float fresnel = Fresnel::evalFresnel(n_, cos_mi);
    pdf = (1.0 - fresnel) * pdf;
  }

  return std::make_tuple(std::move(radiance), pdf);
}

/*!
  */
std::tuple<SampledDirection, SampledSpectra> GgxDielectricBsdf::sample(
    const Vector3* vin,
    const Vector3& normal,
    const WavelengthSamples& wavelengths,
    Sampler& sampler) const noexcept
{
  // Sample a microfacet normal
  const auto m_normal = MicrofacetGgx::sampleNormal(roughness_,
                                                    *vin,
                                                    normal,
                                                    sampler);
  const Float cos_ni = -zisc::dot(normal, *vin),
              cos_mi = -zisc::dot(m_normal.direction(), *vin);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0),
              "The cos(ni) isn't [0.0, 1.0].");
  ZISC_ASSERT(zisc::isInClosedBounds(cos_mi, 0.0, 1.0),
              "The cos(mi) isn't [0.0, 1.0].");

  // Evaluate the fresnel term
  const auto g_result = Fresnel::evalG(n_, cos_mi);
  const bool is_perfect_reflection = !std::get<0>(g_result);
  const Float g = std::get<1>(g_result);
  const Float fresnel = (!is_perfect_reflection)
      ? Fresnel::evalFresnelFromG(cos_mi, g)
      : 1.0; // Perfect reflection

  // Determine a reflection or a refraction
  const bool is_reflection = (sampler.sample() < fresnel);
  auto vout = (is_reflection)
      ? Microfacet::calcReflectionDirection(*vin, m_normal)
      : Microfacet::calcRefractionDirection(*vin, m_normal, n_, g);

  SampledSpectra weight{wavelengths};
  const Float cos_no = zisc::dot(normal, vout.direction());
  if ((is_reflection && (0.0 < cos_no)) ||
      (!is_reflection && (cos_no < 0.0))) {
    vout.setInversePdf((is_reflection)
        ? vout.inversePdf() / fresnel
        : vout.inversePdf() / (1.0 - fresnel));

    // Evaluate the weight
    const Float cos_mo = zisc::dot(m_normal.direction(), vout.direction());
    const Float cos_nm = zisc::dot(m_normal.direction(), normal);
    const Float w = MicrofacetGgx::evalWeight(roughness_,
                                              cos_ni, cos_no,
                                              cos_mi, cos_mo,
                                              cos_nm);
    ZISC_ASSERT(0.0 <= w, "The weight is negative.");
    weight.setIntensity(wavelengths.primaryWavelengthIndex(), w);
  }
  else {
    vout.setPdf(0.0);
  }
  return std::make_tuple(std::move(vout), std::move(weight));
}

/*!
  \details
  No detailed.
  */
bool GgxDielectricBsdf::wavelengthIsSelected() const noexcept
{
  return true;
}

} // namespace nanairo
