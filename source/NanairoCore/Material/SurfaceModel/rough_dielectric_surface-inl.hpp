/*!
  \file rough_dielectric_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_ROUGH_DIELECTRIC_SURFACE_INL_HPP_
#define _NANAIRO_ROUGH_DIELECTRIC_SURFACE_INL_HPP_

#include "rough_dielectric_surface.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "microfacet.hpp"
#include "microfacet_ggx.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Bxdf/ggx_dielectric_bsdf.hpp"
#include "NanairoCore/Material/Texture/texture.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/floating_point.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto RoughDielectricSurface::makeGgxDielectricBsdf(
    const Point2& texture_coordinate,
    const bool is_reverse_face,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const -> ShaderPointer<kSampleSize>
{
  // Get the roughness
  constexpr Float threshold = 0.001;
  Float roughness = roughness_->floatValue(texture_coordinate);
  roughness = (threshold < roughness) 
      ? roughness * roughness
      : threshold * threshold; 

  // Evaluate the refractive index
  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];
  const Float n = (is_reverse_face) 
      ? 1.0 / eta_.getByWavelength(wavelength)
      : eta_.getByWavelength(wavelength);

  // Make GGX BSDF
  using Bsdf = GgxDielectricBsdf<kSampleSize>;
  auto bsdf = memory_pool.allocate<Bsdf>(roughness, n);
  return ShaderPointer<kSampleSize>(bsdf);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SurfaceModel::ShaderPointer<kSampleSize> makeGgxDielectricBsdf(
    const SurfaceModel* surface,
    const Point2& texture_coordinate,
    const bool is_reverse_face,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool)
{
  using zisc::cast;

  auto dielectric_surface = cast<const RoughDielectricSurface*>(surface);
  return dielectric_surface->makeGgxDielectricBsdf(texture_coordinate,
                                                   is_reverse_face,
                                                   wavelengths,
                                                   memory_pool);
}

} // namespace nanairo

#endif // _NANAIRO_ROUGH_DIELECTRIC_SURFACE_INL_HPP_
