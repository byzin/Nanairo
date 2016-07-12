/*!
  \file specular_brdf-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECULAR_BRDF_INL_HPP
#define NANAIRO_SPECULAR_BRDF_INL_HPP

#include "specular_brdf.hpp"
// Standard C++ library
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/fresnel.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SpecularBrdf<kSampleSize>::SpecularBrdf(const Spectra& reflectance_0deg) noexcept :
    reflectance_0deg_{reflectance_0deg}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto SpecularBrdf<kSampleSize>::sample(
    const Vector3* vin,
    const Vector3& normal,
    const Wavelengths& /* wavelengths */,
    Sampler& /* sampler */) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  const Float cos_theta_ni = -zisc::dot(normal, *vin);
  ZISC_ASSERT((0.0 <= cos_theta_ni) && (cos_theta_ni <= 1.0),
              "cos theta_{ni} must be [0, 1].");
  const auto vout = 
      getFresnelReflectionDirection(*vin, normal, cos_theta_ni);
  const auto weight = 
      solveFresnelConductorEquation(cos_theta_ni, reflectance_0deg_);
  return std::make_tuple(SampledDirection{vout, 1.0}, std::move(weight));
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
bool SpecularBrdf<kSampleSize>::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo

#endif // NANAIRO_SPECULAR_BRDF_INL_HPP
