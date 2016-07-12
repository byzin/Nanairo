/*!
  \file surface_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SURFACE_MODEL_INL_HPP
#define NANAIRO_SURFACE_MODEL_INL_HPP

#include "surface_model.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "rough_conductor_surface.hpp"
#include "rough_dielectric_surface.hpp"
#include "rough_plastic_surface.hpp"
#include "smooth_conductor_surface.hpp"
#include "smooth_dielectric_surface.hpp"
#include "smooth_diffuse_surface.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Sampler;
template <uint> class WavelengthSamples;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SurfaceModel::makeBxdf(
    const Point2& texture_coordinate,
    const bool is_reverse_face,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer<kSampleSize>
{
  using zisc::cast;

  ShaderPointer<kSampleSize> shader;

  const auto surface_type = type();
  switch (surface_type) {
   case SurfaceType::SmoothDiffuse:
    shader = makeLambertBrdf(this, texture_coordinate,
                             wavelengths, memory_pool);
    break;
   case SurfaceType::SmoothDielectric:
    shader = makeSpecularBsdf(this, is_reverse_face,
                              wavelengths, memory_pool);
    break;
   case SurfaceType::SmoothConductor:
    shader = makeSpecularBrdf(this, wavelengths, memory_pool);
    break;
   case SurfaceType::RoughDielectric:
    shader = makeGgxDielectricBsdf(this, 
                                   texture_coordinate, is_reverse_face,
                                   wavelengths, memory_pool);
    break;
   case SurfaceType::RoughConductor:
    shader = makeGgxConductorBrdf(this, texture_coordinate,
                                  wavelengths, memory_pool);
    break;
   case SurfaceType::RoughPlastic:
   default:
    zisc::raiseError("SurfaceModelError: (type=", cast<int>(surface_type),
                     ") makeBxdf method is not implemented.");
    break;
  }

  return shader;
}

} // namespace nanairo

#endif // NANAIRO_SURFACE_MODEL_INL_HPP
