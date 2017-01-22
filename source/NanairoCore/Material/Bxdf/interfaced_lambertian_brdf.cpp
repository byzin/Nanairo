/*!
  \file interfaced_lambertian_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

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
InterfacedLambertianBrdf::InterfacedLambertianBrdf(
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
Float InterfacedLambertianBrdf::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const WavelengthSamples& /* wavelengths */) const noexcept
{
  return LayeredDiffuse::evalPdf(roughness_, *vin, *vout, normal, n_, k_d_, ri_);
}

/*!
  \details
  No detailed.
  */
SampledSpectra InterfacedLambertianBrdf::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const WavelengthSamples& wavelengths) const noexcept
{
  const Float f = LayeredDiffuse::evalReflectance(roughness_, *vin, *vout, normal,
                                                  n_, k_d_, ri_, *sampler_);
  SampledSpectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> InterfacedLambertianBrdf::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const WavelengthSamples& wavelengths) const noexcept
{
  Float pdf = 0.0;
  const Float f = LayeredDiffuse::evalReflectance(roughness_, *vin, *vout, normal,
                                                  n_, k_d_, ri_, *sampler_, &pdf);
  SampledSpectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  return std::make_tuple(std::move(radiance), pdf);
}

/*!
  */
std::tuple<SampledDirection, SampledSpectra> InterfacedLambertianBrdf::sample(
    const Vector3* vin,
    const Vector3& normal,
    const WavelengthSamples& wavelengths,
    Sampler& sampler) const noexcept
{
  const auto result = LayeredDiffuse::sample(roughness_, *vin, normal,
                                             n_, k_d_, ri_, sampler);
  const Float w = std::get<1>(result);
  SampledSpectra weight{wavelengths};
  weight.setIntensity(wavelengths.primaryWavelengthIndex(), w);
  return std::make_tuple(std::move(std::get<0>(result)), std::move(weight));
}

/*!
  \details
  No detailed.
  */
bool InterfacedLambertianBrdf::wavelengthIsSelected() const noexcept
{
  return true;
}

} // namespace nanairo
