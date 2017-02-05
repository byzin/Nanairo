/*!
  \file non_directional_light.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "non_directional_light.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

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
SampledSpectra NonDirectionalLight::evalRadiance(
    const Vector3* /* vin */,
    const Vector3* /* vout */,
    const Vector3& /* normal */,
    const WavelengthSamples& /* wavelengths */) const noexcept
{
  constexpr Float k = zisc::invert(zisc::kPi<Float>);
  return k * radiant_exitance_;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> NonDirectionalLight::sample(
    const Vector3* /* vin */,
    const Vector3& normal,
    const WavelengthSamples& /* wavelengths */,
    Sampler& sampler) const noexcept
{
  const auto vout = SampledDirection::sampleOnHemisphere<1>(normal, sampler);
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
