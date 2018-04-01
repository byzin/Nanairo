/*!
  \file specular_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/fresnel.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SpecularBrdf::SpecularBrdf(const SampledSpectra& n,
                           const SampledSpectra& eta) noexcept :
    n_{n},
    eta_{eta}
{
}

/*!
  */
bool SpecularBrdf::isReflective() const noexcept
{
  return true;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> SpecularBrdf::sample(
    const Vector3* vin,
    const WavelengthSamples& /* wavelengths */,
    Sampler& /* sampler */,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto vin_d = -(*vin);
  const Float cos_ni = zisc::dot(info->normal(), vin_d);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  const auto vout = Fresnel::calcReflectionDirection(vin_d, info->normal());
  const auto fresnel = Fresnel::evalFresnel(n_, eta_, cos_ni);
  return std::make_tuple(SampledDirection{vout, 1.0}, fresnel);
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
