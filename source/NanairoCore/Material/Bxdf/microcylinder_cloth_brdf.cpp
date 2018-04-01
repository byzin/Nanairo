/*!
  \file microcylinder_cloth_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "microcylinder_cloth_brdf.hpp"
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
#include "NanairoCore/Material/SurfaceModel/cloth_surface.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/microcylinder.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

#include "NanairoCore/Material/SurfaceModel/Surface/diffuse.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
MicrocylinderClothBrdf::MicrocylinderClothBrdf(
    const ClothSurface* surface,
    const SampledSpectra& reflectance) noexcept :
        reflectance_{reflectance},
        surface_{surface}
{
}

/*!
  \details
  No detailed.
  */
Float MicrocylinderClothBrdf::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
//  const Float k_d = surface_->kD();
//  const Float gamma_r = surface_->gammaR();
//  const Float gamma_v = surface_->gammaV();
//  return Microcylinder::evalPdf(*vin, *vout, normal, k_d, gamma_r, gamma_v);
  
  const Float pdf = Diffuse::evalPdf(*vout, info->normal());
  return pdf;
}

/*!
  \details
  No detailed.
  */
SampledSpectra MicrocylinderClothBrdf::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& wavelengths,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
//  const Float eta = surface_->eta();
//  const Float k_d = surface_->kD();
//  const Float gamma_r = surface_->gammaR();
//  const Float gamma_v = surface_->gammaV();
//  const Float rho = surface_->rho();
//  return Microcylinder::evalReflectance(*vin, *vout, normal, reflectance_,
//                                        eta, k_d, gamma_r, gamma_v, rho);

  const auto radiance = Diffuse::evalRadiance(SampledSpectra{wavelengths, 0.8});
  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> MicrocylinderClothBrdf::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& wavelengths,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
//  const Float eta = surface_->eta();
//  const Float k_d = surface_->kD();
//  const Float gamma_r = surface_->gammaR();
//  const Float gamma_v = surface_->gammaV();
//  const Float rho = surface_->rho();
//  Float pdf;
//  const auto radiance =
//      Microcylinder::evalReflectance(*vin, *vout, normal, reflectance_,
//                                     eta, k_d, gamma_r, gamma_v, rho, &pdf);
//  return std::make_tuple(std::move(radiance), pdf);

  const auto radiance = Diffuse::evalRadiance(SampledSpectra{wavelengths, 0.8});
  const auto pdf = Diffuse::evalPdf(*vout, info->normal());
  return std::make_tuple(radiance, pdf);
}

/*!
  */
bool MicrocylinderClothBrdf::isReflective() const noexcept
{
  return true;
}

std::tuple<SampledDirection, SampledSpectra> MicrocylinderClothBrdf::sample(
    const Vector3* vin,
    const WavelengthSamples& wavelengths,
    Sampler& sampler,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");

//  const Float eta = surface_->eta();
//  const Float k_d = surface_->kD();
//  const Float gamma_r = surface_->gammaR();
//  const Float gamma_v = surface_->gammaV();
//  const Float rho = surface_->rho();
//  return Microcylinder::sample(*vin, normal, reflectance_,
//                               eta, k_d, gamma_r, gamma_v, rho, sampler);

  const auto vout = Diffuse::sample(info->shapePoint(), sampler);
  return std::make_tuple(vout, SampledSpectra{wavelengths, 0.8});
}

/*!
  \details
  No detailed.
  */
bool MicrocylinderClothBrdf::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo
