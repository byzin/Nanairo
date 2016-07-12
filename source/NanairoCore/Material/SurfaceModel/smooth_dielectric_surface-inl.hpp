/*!
  \file smooth_dielectric_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTH_DIELECTRIC_SURFACE_INL_HPP
#define NANAIRO_SMOOTH_DIELECTRIC_SURFACE_INL_HPP

#include "smooth_dielectric_surface.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Bxdf/specular_bsdf.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SmoothDielectricSurface::makeSpecularBsdf(
    const bool is_reverse_face,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer<kSampleSize>
{
  const auto wavelength = 
      wavelengths[wavelengths.primaryWavelengthIndex()];
  const Float n = (is_reverse_face)
      ? 1.0 / eta_.getByWavelength(wavelength)
      : eta_.getByWavelength(wavelength);

  using Bsdf = SpecularBsdf<kSampleSize>;
  auto bsdf = memory_pool.allocate<Bsdf>(n);
  return ShaderPointer<kSampleSize>{bsdf};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SurfaceModel::ShaderPointer<kSampleSize> makeSpecularBsdf(
    const SurfaceModel* surface,
    const bool is_reverse_face,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) noexcept
{
  using zisc::cast;

  auto s = cast<const SmoothDielectricSurface*>(surface);
  return s->makeSpecularBsdf(is_reverse_face, wavelengths, memory_pool);
}

} // namespace nanairo

#endif // NANAIRO_SMOOTH_DIELECTRIC_SURFACE_INL_HPP
