/*!
  \file microcylinder_cloth_brdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MICROCYLINDER_CLOTH_BRDF_INL_HPP
#define NANAIRO_MICROCYLINDER_CLOTH_BRDF_INL_HPP

#include "microcylinder_cloth_brdf.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/cloth_surface.hpp"
#include "NanairoCore/Material/SurfaceModel/microcylinder.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
MicrocylinderClothBrdf<kSampleSize>::MicrocylinderClothBrdf(
    const ClothSurface* surface,
    const Spectra& reflectance) noexcept :
        reflectance_{reflectance},
        surface_{surface}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
Float MicrocylinderClothBrdf<kSampleSize>::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept
{
  const Float k_d = surface_->kD();
  const Float gamma_r = surface_->gammaR();
  const Float gamma_v = surface_->gammaV();
  return Microcylinder::evalPdf(*vin, *vout, normal, k_d, gamma_r, gamma_v);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto MicrocylinderClothBrdf<kSampleSize>::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept -> Spectra
{
  const Float eta = surface_->eta();
  const Float k_d = surface_->kD();
  const Float gamma_r = surface_->gammaR();
  const Float gamma_v = surface_->gammaV();
  const Float rho = surface_->rho();
  return Microcylinder::evalReflectance(*vin, *vout, normal, reflectance_,
                                        eta, k_d, gamma_r, gamma_v, rho);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto MicrocylinderClothBrdf<kSampleSize>::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */) const noexcept -> std::tuple<Spectra, Float>
{
  const Float eta = surface_->eta();
  const Float k_d = surface_->kD();
  const Float gamma_r = surface_->gammaR();
  const Float gamma_v = surface_->gammaV();
  const Float rho = surface_->rho();
  Float pdf;
  const auto radiance =
      Microcylinder::evalReflectance(*vin, *vout, normal, reflectance_,
                                     eta, k_d, gamma_r, gamma_v, rho, &pdf);
  return std::make_tuple(std::move(radiance), pdf);
}

template <uint kSampleSize>
auto MicrocylinderClothBrdf<kSampleSize>::sample(
    const Vector3* vin,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */,
    Sampler& sampler) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  const Float eta = surface_->eta();
  const Float k_d = surface_->kD();
  const Float gamma_r = surface_->gammaR();
  const Float gamma_v = surface_->gammaV();
  const Float rho = surface_->rho();
  return Microcylinder::sample(*vin, normal, reflectance_,
                               eta, k_d, gamma_r, gamma_v, rho, sampler);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool MicrocylinderClothBrdf<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo

#endif // NANAIRO_MICROCYLINDER_CLOTH_BRDF_INL_HPP
