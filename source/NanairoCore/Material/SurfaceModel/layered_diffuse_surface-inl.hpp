/*!
  \file layered_diffuse_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LAYERED_DIFFUSE_SURFACE_INL_HPP
#define NANAIRO_LAYERED_DIFFUSE_SURFACE_INL_HPP

#include "layered_diffuse_surface.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Bxdf/interfaced_lambertian_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto LayeredDiffuseSurface::makeInterfacedLambertianBrdf(
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    Sampler& sampler,
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

  // Evaluate the refractive index
  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];
  const Float n = eta_.getByWavelength(wavelength);
  const Float ri = ri_.getByWavelength(wavelength);

  // Get the reflectance
  const Float k_d = reflectance_->reflectiveValue(texture_coordinate, wavelength);
  ZISC_ASSERT(zisc::isInClosedBounds(k_d, 0.0, 1.0), "Reflectance isn't [0, 1].");

  // Make a interfaced lambertian BRDF
  using Brdf = InterfacedLambertianBrdf<kSampleSize>;
  auto brdf = memory_pool.allocate<Brdf>(k_d, roughness, n, ri, sampler);
  return ShaderPointer<kSampleSize>(brdf);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SurfaceModel::makeInterfacedLambertianBrdf(
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    Sampler& sampler,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer<kSampleSize>
{
  auto s = zisc::cast<const LayeredDiffuseSurface*>(this);
  return s->makeInterfacedLambertianBrdf(texture_coordinate,
                                         wavelengths,
                                         sampler,
                                         memory_pool);
}

} // namespace nanairo

#endif // NANAIRO_LAYERED_DIFFUSE_SURFACE_INL_HPP
