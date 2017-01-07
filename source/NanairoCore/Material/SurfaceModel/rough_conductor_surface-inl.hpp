/*!
  \file rough_conductor_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_ROUGH_CONDUCTOR_SURFACE_INL_HPP
#define NANAIRO_ROUGH_CONDUCTOR_SURFACE_INL_HPP

#include "rough_conductor_surface.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Bxdf/ggx_conductor_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

 /*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto RoughConductorSurface::makeGgxConductorBrdf(
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer<kSampleSize>
{
  // Get the roughness
  constexpr Float min_roughness = 0.001;
  Float roughness = roughness_->floatValue(texture_coordinate);
  roughness = (min_roughness < roughness)
      ? roughness * roughness
      : min_roughness * min_roughness;
  ZISC_ASSERT(zisc::isInClosedBounds(roughness, 0.0, 1.0),
              "The roughness is out of the range [0, 1].");

  const auto fresnel_0deg = sample(fresnel_0deg_, wavelengths);

  // Make GGX BRDF
  using Brdf = GgxConductorBrdf<kSampleSize>;
  auto brdf = memory_pool.allocate<Brdf>(roughness, fresnel_0deg);
  return ShaderPointer<kSampleSize>{brdf};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SurfaceModel::makeGgxConductorBrdf(
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer<kSampleSize>
{
  auto s = zisc::cast<const RoughConductorSurface*>(this);
  return s->makeGgxConductorBrdf(texture_coordinate, wavelengths, memory_pool);
}

} // namespace nanairo

#endif // NANAIRO_ROUGH_CONDUCTOR_SURFACE_INL_HPP
