/*!
  \file smooth_diffuse_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SMOOTH_DIFFUSE_SURFACE_INL_HPP_
#define _NANAIRO_SMOOTH_DIFFUSE_SURFACE_INL_HPP_

#include "smooth_diffuse_surface.hpp"
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Bxdf/lambert_brdf.hpp"
#include "NanairoCore/Material/Texture/texture.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/floating_point.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {
  
/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SmoothDiffuseSurface::makeLambertBrdf(
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const -> ShaderPointer<kSampleSize>
{
  const auto reflectance = 
      reflectance_->spectraValue(texture_coordinate, wavelengths);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(reflectance), 
              "Reflectance must be [0, 1].");

  using Brdf = LambertBrdf<kSampleSize>;
  auto brdf = memory_pool.allocate<Brdf>(reflectance);
  return ShaderPointer<kSampleSize>{brdf};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SurfaceModel::ShaderPointer<kSampleSize> makeLambertBrdf(
    const SurfaceModel* surface,
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool)
{
  using zisc::cast;

  auto s = cast<const SmoothDiffuseSurface*>(surface);
  return s->makeLambertBrdf(texture_coordinate, wavelengths, memory_pool);
}

} // namespace nanairo

#endif // _NANAIRO_SMOOTH_DIFFUSE_SURFACE_INL_HPP_
