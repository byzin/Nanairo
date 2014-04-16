/*!
  \file smooth_conductor_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SMOOTH_CONDUCTOR_SURFACE_INL_HPP_
#define _NANAIRO_SMOOTH_CONDUCTOR_SURFACE_INL_HPP_

#include "smooth_conductor_surface.hpp"
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Bxdf/specular_brdf.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SmoothConductorSurface::makeSpecularBrdf(
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const -> ShaderPointer<kSampleSize>
{
  const auto reflectance_0deg = sample(reflectance_0deg_, wavelengths);

  using Brdf = SpecularBrdf<kSampleSize>;
  auto brdf = memory_pool.allocate<Brdf>(reflectance_0deg);
  return ShaderPointer<kSampleSize>{brdf};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SurfaceModel::ShaderPointer<kSampleSize> makeSpecularBrdf(
    const SurfaceModel* surface,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool)
{
  using zisc::cast;

  auto s = cast<const SmoothConductorSurface*>(surface);
  return s->makeSpecularBrdf(wavelengths, memory_pool);
}

} // namespace nanairo

#endif // _NANAIRO_SMOOTH_CONDUCTOR_SURFACE_INL_HPP_
