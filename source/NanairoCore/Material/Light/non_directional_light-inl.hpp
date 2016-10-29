/*!
  \file non_directional_light-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_NON_DIRECTIONAL_LIGHT_INL_HPP
#define NANAIRO_NON_DIRECTIONAL_LIGHT_INL_HPP

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

// Forward declaration
class Sampler;
template <uint> class WavelengthSamples;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
NonDirectionalLight<kSampleSize>::NonDirectionalLight(
    const Spectra& radiant_exitance) noexcept :
      radiant_exitance_{radiant_exitance}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto NonDirectionalLight<kSampleSize>::evalRadiance(
    const Vector3* /* vin */,
    const Vector3* /* vout */,
    const Vector3& /* normal */,
    const Wavelengths& /* wavelengths */) const noexcept -> Spectra
{
  constexpr Float k = 1.0 / zisc::kPi<Float>;
  return k * radiant_exitance_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto NonDirectionalLight<kSampleSize>::sample(
    const Vector3* /* vin */,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */,
    Sampler& sampler) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  const auto vout = SampledDirection::sampleOnHemisphere<1>(normal, sampler);
  return std::make_tuple(vout, radiant_exitance_);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
bool NonDirectionalLight<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo

#endif // NANAIRO_NON_DIRECTIONAL_LIGHT_INL_HPP
