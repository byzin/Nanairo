/*!
  \file ggx_conductor_brdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GGX_CONDUCTOR_BRDF_INL_HPP
#define NANAIRO_GGX_CONDUCTOR_BRDF_INL_HPP

#include "ggx_conductor_brdf.hpp"
// Sstandard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
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
GgxConductorBrdf<kSampleSize>::GgxConductorBrdf(
    const Float roughness,
    const Spectra& reflectance_0deg) noexcept :
        reflectance_0deg_{reflectance_0deg},
        roughness_{roughness}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
Float GgxConductorBrdf<kSampleSize>::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept
{
  const Float pdf = MicrofacetGgx::evalReflectionPdf(roughness_, *vin, *vout, normal);
  return pdf;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto GgxConductorBrdf<kSampleSize>::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept -> Spectra
{
  const auto& r0 = reflectance_0deg_;
  const auto f = MicrofacetGgx::evalReflectance(roughness_, *vin, *vout, normal, r0);
  return f;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto GgxConductorBrdf<kSampleSize>::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept -> std::tuple<Spectra, Float>
{
  const auto& r0 = reflectance_0deg_;
  Float pdf = 0.0;
  const auto f =
      MicrofacetGgx::evalReflectance(roughness_, *vin, *vout, normal, r0, &pdf);
  return std::make_tuple(std::move(f), pdf);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto GgxConductorBrdf<kSampleSize>::sample(
    const Vector3* vin,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */,
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

  // Evaluate fresnel term
  const auto& r0 = reflectance_0deg_;
  const auto fresnel = Fresnel::evalConductorEquation(cos_mi, r0);

  // Get the reflection direction
  const auto vout = Microfacet::calcReflectionDirection(*vin, m_normal);

  // Evaluate the weight
  const Float cos_no = zisc::dot(normal, vout.direction());
  const Float cos_mo = cos_mi;
  const auto weight = fresnel * MicrofacetGgx::evalWeight(roughness_, cos_ni, cos_no,
                                                          cos_mi, cos_mo, cos_nm);
  ZISC_ASSERT(!weight.hasNegative(), "Weight contains negative.");

  return std::make_tuple(std::move(vout), std::move(weight));
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
bool GgxConductorBrdf<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo

#endif // NANAIRO_GGX_CONDUCTOR_BRDF_INL_HPP
