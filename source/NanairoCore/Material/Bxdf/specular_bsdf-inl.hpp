/*!
  \file specular_bsdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECULAR_BSDF_INL_HPP
#define NANAIRO_SPECULAR_BSDF_INL_HPP

#include "specular_bsdf.hpp"
// Standard C++ library
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/fresnel.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
class Sampler;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SpecularBsdf<kSampleSize>::SpecularBsdf(const Float n) noexcept :
  n_{n}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto SpecularBsdf<kSampleSize>::sample(
    const Vector3* vin,
    const Vector3& normal,
    const Wavelengths& wavelengths,
    Sampler& sampler) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  // Evaluate the fresnel term
  const Float cos_ni = -zisc::dot(normal, *vin);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  const auto result = Fresnel::evalG(n_, cos_ni);
  const bool is_perfect_reflection = !std::get<0>(result);
  const Float g = std::get<1>(result);
  const Float fresnel = (!is_perfect_reflection)
      ? Fresnel::evalDielectricEquation(cos_ni, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(zisc::isInClosedBounds(fresnel, 0.0, 1.0),
              "Fresnel reflectance isn't [0, 1].");

  // Determine a reflection or a refraction
  const bool is_reflection = (sampler.sample(0.0, 1.0) < fresnel);
  const auto vout = (is_reflection)
      ? Fresnel::calcReflectionDirection(*vin, normal)
      : Fresnel::calcRefractionDirection(*vin, normal, n_, g);

  Spectra weight{wavelengths};
  weight.setIntensity(wavelengths.primaryWavelengthIndex(), 1.0);

  return std::make_tuple(SampledDirection{vout, 1.0}, std::move(weight));
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
bool SpecularBsdf<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return true;
}

} // namespace nanairo

#endif // NANAIRO_SPECULAR_BSDF_INL_HPP
