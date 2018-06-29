/*!
  \file lambert_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "lambert_brdf.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/diffuse.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"


namespace nanairo {

/*!
  \details
  No detailed.
  */
LambertBrdf::LambertBrdf(const SampledSpectra& reflectance) noexcept :
    reflectance_{reflectance}
{
}

/*!
  \details
  No detailed.
  */
Float LambertBrdf::evalPdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const WavelengthSamples& /* wavelemgths */,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const Float pdf = Diffuse::evalPdf(*vout, info->normal());
  return pdf;
}

/*!
  \details
  No detailed.
  */
SampledSpectra LambertBrdf::evalRadiance(
    const Vector3* /* vin */,
    const Vector3* /* vout */,
    const WavelengthSamples& /* wavelemgths */,
    const IntersectionInfo* /* info */) const noexcept
{
  const auto radiance = Diffuse::evalRadiance(reflectance_);
  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> LambertBrdf::evalRadianceAndPdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const WavelengthSamples& /* wavelemgths */,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto radiance = Diffuse::evalRadiance(reflectance_);
  const auto pdf = Diffuse::evalPdf(*vout, info->normal());
  return std::make_tuple(radiance, pdf);
}

/*!
  */
bool LambertBrdf::isReflective() const noexcept
{
  return true;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> LambertBrdf::sample(
    const Vector3* /* vin */,
    const WavelengthSamples& /* wavelemgths */,
    Sampler& sampler,
    PathState& path_state,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto vout = Diffuse::sample(info->shapePoint(), sampler, path_state);
  return std::make_tuple(vout, reflectance_);
}

/*!
  \details
  No detailed.
  */
bool LambertBrdf::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo
