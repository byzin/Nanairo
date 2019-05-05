/*!
  \file non_directional_light.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "non_directional_light.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
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
NonDirectionalLight::NonDirectionalLight(
    const SampledSpectra& radiant_exitance) noexcept :
      radiant_exitance_{radiant_exitance}
{
}

/*!
  \details
  No detailed.
  */
Float NonDirectionalLight::evalPdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
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
SampledSpectra NonDirectionalLight::evalRadiance(
    const Vector3* /* vin */,
    const Vector3* /* vout */,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* /* info */) const noexcept
{
  const auto radiance = Diffuse::evalRadiance(radiant_exitance_);
  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> NonDirectionalLight::evalRadianceAndPdf(
    const Vector3* /* vin */,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto radiance = Diffuse::evalRadiance(radiant_exitance_);
  const auto pdf = Diffuse::evalPdf(*vout, info->normal());
  return std::make_tuple(radiance, pdf);
}

/*!
  */
bool NonDirectionalLight::isEmissive() const noexcept
{
  return true;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> NonDirectionalLight::sample(
    const Vector3* /* vin */,
    const WavelengthSamples& /* wavelengths */,
    Sampler& sampler,
    PathState& path_state,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto vout = Diffuse::sample(info->shapePoint(), sampler, path_state);
  return std::make_tuple(vout, radiant_exitance_);
}

/*!
  \details
  No detailed.
  */
bool NonDirectionalLight::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo
