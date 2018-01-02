/*!
  \file interfaced_lambertian_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/layered_diffuse.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

#include "NanairoCore/Material/SurfaceModel/Surface/diffuse.hpp"

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
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
//  return LayeredDiffuse::evalPdf(
//      roughness_, *vin, *vout, info->normal(),
//      n_, k_d_, ri_);

  const Float pdf = Diffuse::evalPdf(*vout, info->normal());
  return pdf;
}

/*!
  \details
  No detailed.
  */
SampledSpectra InterfacedLambertianBrdf::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& wavelengths,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
//  const Float f = LayeredDiffuse::evalReflectance(
//      roughness_, *vin, *vout, info->normal(),
//      n_, k_d_, ri_, *sampler_);
//  SampledSpectra radiance{wavelengths};
//  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
//  return radiance;

  const auto radiance = Diffuse::evalRadiance(SampledSpectra{wavelengths, 0.8});
  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> InterfacedLambertianBrdf::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& wavelengths,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
//  Float pdf = 0.0;
//  const Float f = LayeredDiffuse::evalReflectance(
//      roughness_, *vin, *vout, info->normal(),
//      n_, k_d_, ri_, *sampler_, &pdf);
//  SampledSpectra radiance{wavelengths};
//  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
//  return std::make_tuple(std::move(radiance), pdf);

  const auto radiance = Diffuse::evalRadiance(SampledSpectra{wavelengths, 0.8});
  const Float pdf = Diffuse::evalPdf(*vout, info->normal());
  return std::make_tuple(radiance, pdf);
}

/*!
  */
bool InterfacedLambertianBrdf::isReflective() const noexcept
{
  return true;
}

/*!
  */
std::tuple<SampledDirection, SampledSpectra> InterfacedLambertianBrdf::sample(
    const Vector3* vin,
    const WavelengthSamples& wavelengths,
    Sampler& sampler,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
//  const auto result = LayeredDiffuse::sample(roughness_, *vin, info->normal(),
//                                             n_, k_d_, ri_, sampler);
//  const Float w = std::get<1>(result);
//  SampledSpectra weight{wavelengths};
//  weight.setIntensity(wavelengths.primaryWavelengthIndex(), w);
//  return std::make_tuple(std::move(std::get<0>(result)), std::move(weight));

  const auto vout = Diffuse::sample(info->shapePoint(), sampler);
  return std::make_tuple(vout, SampledSpectra{wavelengths, 0.8});
}

/*!
  \details
  No detailed.
  */
bool InterfacedLambertianBrdf::wavelengthIsSelected() const noexcept
{
//  return true;
  return false;
}

} // namespace nanairo
