/*!
  \file ggx_conductor_brdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
Float GgxConductorBrdf<kSampleSize>::evaluatePdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept
{
  const Float pdf = evaluateGgxReflectionPdf(roughness_, *vin, *vout, normal);
  return pdf;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto GgxConductorBrdf<kSampleSize>::evaluateRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept -> Spectra
{
  const auto& r0 = reflectance_0deg_;
  const auto f = evaluateGgxReflectance(roughness_, *vin, *vout, normal, r0);
  return f;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto GgxConductorBrdf<kSampleSize>::evaluateRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept -> std::tuple<Spectra, Float>
{
  const auto& r0 = reflectance_0deg_;
  Float pdf = 0.0;
  const auto f = 
      evaluateGgxReflectance(roughness_, *vin, *vout, normal, r0, &pdf);
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
  Float cos_theta_ni = 0.0,
        cos_theta_mi = 0.0,
        cos_theta_nm = 0.0;
  const auto m_normal =
      sampleGgxMicrofacetNormal(roughness_, *vin, normal, sampler,
                                &cos_theta_ni, &cos_theta_mi, &cos_theta_nm);
  ZISC_ASSERT(0.0 <= cos_theta_ni * cos_theta_mi,
              "Microfacet normal must be in the same hemisphere as normal.");

  // Evaluate fresnel term
  const auto& r0 = reflectance_0deg_;
  const auto fresnel = solveFresnelConductorEquation(cos_theta_mi, r0);

  // Get the reflection direction
  const auto vout =
      getMicrofacetReflectionDirection(*vin, m_normal, cos_theta_mi);

  // Evaluate the weight
  const Float cos_theta_no = zisc::dot(normal, vout.direction());
  const Float cos_theta_mo = cos_theta_mi;
  const auto weight =
      fresnel * 
      evaluateGgxWeight(roughness_, cos_theta_ni, cos_theta_no,
                        cos_theta_mi, cos_theta_mo, cos_theta_nm);
  ZISC_ASSERT(!hasNegativeFloat(weight), "Weight must not contain negative.");

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
