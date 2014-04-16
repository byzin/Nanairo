/*!
  \file rough_conductor_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_ROUGH_CONDUCTOR_SURFACE_INL_HPP_
#define _NANAIRO_ROUGH_CONDUCTOR_SURFACE_INL_HPP_

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
#include "NanairoCore/Material/Texture/texture.hpp"
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
    MemoryPool& memory_pool) const -> ShaderPointer<kSampleSize>
{
  // Get the roughness
  constexpr Float threshold = 0.001;
  Float roughness = roughness_->floatValue(texture_coordinate);
  roughness = (threshold < roughness) 
      ? roughness * roughness
      : threshold * threshold; 

  const auto reflectance_0deg = sample(reflectance_0deg_, wavelengths);

  // Make GGX BRDF
  using Brdf = GgxConductorBrdf<kSampleSize>;
  auto brdf = memory_pool.allocate<Brdf>(roughness, reflectance_0deg);
  return ShaderPointer<kSampleSize>{brdf};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SurfaceModel::ShaderPointer<kSampleSize> makeGgxConductorBrdf(
    const SurfaceModel* surface,
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool)
{
  using zisc::cast;

  auto conductor_surface = cast<const RoughConductorSurface*>(surface);
  return conductor_surface->makeGgxConductorBrdf(texture_coordinate,
                                                 wavelengths,
                                                 memory_pool);
}

} // namespace nanairo

#endif // _NANAIRO_ROUGH_CONDUCTOR_SURFACE_INL_HPP_
