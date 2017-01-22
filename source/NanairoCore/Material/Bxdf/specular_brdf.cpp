/*!
  \file specular_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "specular_brdf.hpp"
// Standard C++ library
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/fresnel.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SpecularBrdf::SpecularBrdf(const SampledSpectra& fresnel_0deg) noexcept :
    fresnel_0deg_{fresnel_0deg}
{
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> SpecularBrdf::sample(
    const Vector3* vin,
    const Vector3& normal,
    const WavelengthSamples& /* wavelengths */,
    Sampler& /* sampler */) const noexcept
{
  const Float cos_ni = -zisc::dot(normal, *vin);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  const auto vout = Fresnel::calcReflectionDirection(*vin, normal);
  const auto weight = Fresnel::evalFresnel(cos_ni, fresnel_0deg_);
  return std::make_tuple(SampledDirection{vout, 1.0}, std::move(weight));
}

/*!
  \details
  No detailed.
  */
bool SpecularBrdf::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo
