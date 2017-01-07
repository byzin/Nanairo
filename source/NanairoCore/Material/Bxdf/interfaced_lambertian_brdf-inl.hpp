/*!
  \file interfaced_lambertian_brdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_INTERFACED_LAMBERTIAN_BRDF_INL_HPP
#define NANAIRO_INTERFACED_LAMBERTIAN_BRDF_INL_HPP

#include "interfaced_lambertian_brdf.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/layered_diffuse.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
InterfacedLambertianBrdf<kSampleSize>::InterfacedLambertianBrdf(
    const Float k_d,
    const Float roughness,
    const Float n,
    const Float ri,
    Sampler& sampler) noexcept :
        sampler_{&sampler},
        k_d_{k_d},
        roughness_{roughness},
        n_{n},
        ri_{ri}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
Float InterfacedLambertianBrdf<kSampleSize>::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept
{
  return LayeredDiffuse::evalPdf(roughness_, *vin, *vout, normal, n_, k_d_, ri_);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto InterfacedLambertianBrdf<kSampleSize>::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& wavelengths) const noexcept -> Spectra
{
  const Float f = LayeredDiffuse::evalReflectance(roughness_, *vin, *vout, normal,
                                                  n_, k_d_, ri_, *sampler_);
  Spectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  return radiance;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto InterfacedLambertianBrdf<kSampleSize>::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& wavelengths) const noexcept -> std::tuple<Spectra, Float>
{
  Float pdf = 0.0;
  const Float f = LayeredDiffuse::evalReflectance(roughness_, *vin, *vout, normal,
                                                  n_, k_d_, ri_, *sampler_, &pdf);
  Spectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  return std::make_tuple(std::move(radiance), pdf);
}

template <uint kSampleSize>
auto InterfacedLambertianBrdf<kSampleSize>::sample(
    const Vector3* vin,
    const Vector3& normal,
    const Wavelengths& wavelengths,
    Sampler& sampler) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  const auto result = LayeredDiffuse::sample(roughness_, *vin, normal,
                                             n_, k_d_, ri_, sampler);
  const Float w = std::get<1>(result);
  Spectra weight{wavelengths};
  weight.setIntensity(wavelengths.primaryWavelengthIndex(), w);
  return std::make_tuple(std::move(std::get<0>(result)), std::move(weight));
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool InterfacedLambertianBrdf<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return true;
}

} // namespace nanairo

#endif // NANAIRO_INTERFACED_LAMBERTIAN_BRDF_INL_HPP
