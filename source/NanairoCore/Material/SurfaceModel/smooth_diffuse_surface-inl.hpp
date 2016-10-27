/*!
  \file smooth_diffuse_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTH_DIFFUSE_SURFACE_INL_HPP
#define NANAIRO_SMOOTH_DIFFUSE_SURFACE_INL_HPP

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
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
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
    MemoryPool& memory_pool) const noexcept -> ShaderPointer<kSampleSize>
{
  const auto reflectance =
      reflectance_->reflectiveValue(texture_coordinate, wavelengths);
  ZISC_ASSERT(reflectance.isAllInBounds(0.0, 1.0),
              "Reflectances aren't [0, 1].");

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
    MemoryPool& memory_pool) noexcept
{
  using zisc::cast;

  auto s = cast<const SmoothDiffuseSurface*>(surface);
  return s->makeLambertBrdf(texture_coordinate, wavelengths, memory_pool);
}

} // namespace nanairo

#endif // NANAIRO_SMOOTH_DIFFUSE_SURFACE_INL_HPP
