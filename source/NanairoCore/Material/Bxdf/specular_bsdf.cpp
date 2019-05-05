/*!
  \file specular_bsdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "specular_bsdf.hpp"
// Standard C++ library
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/fresnel.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SpecularBsdf::SpecularBsdf(const Float n) noexcept :
  n_{n}
{
}

/*!
  */
bool SpecularBsdf::isReflective() const noexcept
{
  return true;
}

/*!
  */
bool SpecularBsdf::isTransmissive() const noexcept
{
  return true;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> SpecularBsdf::sample(
    const Vector3* vin,
    const WavelengthSamples& wavelengths,
    Sampler& sampler,
    PathState& path_state,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  // Evaluate the fresnel term
  const auto vin_d = -(*vin);
  const Float cos_ni = zisc::dot(info->normal(), vin_d);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_ni, 0.0, 1.0), "cos_ni isn't [0, 1].");
  const auto g2 = Fresnel::evalG2(n_, cos_ni);
  const bool is_perfect_reflection = g2 <= 0.0;
  const Float g = (!is_perfect_reflection) ? zisc::sqrt(g2) : 0.0;
  const Float fresnel = (!is_perfect_reflection)
      ? Fresnel::evalFresnelFromG(cos_ni, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(zisc::isInClosedBounds(fresnel, 0.0, 1.0),
              "Fresnel reflectance isn't [0, 1].");

  // Determine a reflection or a refraction
  const bool is_reflection = is_perfect_reflection ||
      (sampler.draw1D(path_state) < fresnel);
  const auto vout = (is_reflection)
      ? Fresnel::calcReflectionDirection(vin_d, info->normal())
      : Fresnel::calcRefractionDirection(vin_d, info->normal(), n_, g);

  SampledSpectra weight{wavelengths};
  weight.setIntensity(wavelengths.primaryWavelengthIndex(), 1.0);

  return std::make_tuple(SampledDirection{vout, 1.0}, weight);
}

/*!
  \details
  No detailed.
  */
bool SpecularBsdf::wavelengthIsSelected() const noexcept
{
  return true;
}

} // namespace nanairo
