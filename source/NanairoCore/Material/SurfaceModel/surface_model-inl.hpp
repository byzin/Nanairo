/*!
  \file surface_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SURFACE_MODEL_INL_HPP
#define NANAIRO_SURFACE_MODEL_INL_HPP

#include "surface_model.hpp"
// Standard C++ library
#include <limits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  */
inline
Float SurfaceModel::evalRefractiveIndex(
    const TextureModel* outer_refractive_index_texture,
    const TextureModel* inner_refractive_index_texture,
    const Point2& uv,
    const uint16 wavelength,
    const bool is_back_face) noexcept
{
  const Float n1 = outer_refractive_index_texture->spectraValue(uv, wavelength);
  const Float n2 = inner_refractive_index_texture->spectraValue(uv, wavelength);
  Float n = (is_back_face) ? (n1 / n2) : (n2 / n1);
  n = (n != 1.0) ? n : (1.0 + std::numeric_limits<Float>::epsilon());
  return n;
}

/*!
  */
inline
SampledSpectra SurfaceModel::evalRefractiveIndex(
    const TextureModel* outer_refractive_index_texture,
    const TextureModel* inner_refractive_index_texture,
    const Point2& uv,
    const WavelengthSamples& wavelengths) noexcept
{
  const auto n1 = outer_refractive_index_texture->spectraValue(uv, wavelengths);
  const auto n2 = inner_refractive_index_texture->spectraValue(uv, wavelengths);
  const auto n = n2 / n1;
  return n;
}

/*!
  */
inline
Float SurfaceModel::evalRoughness(
    const TextureModel* roughness_texture,
    const Point2& uv) noexcept
{
  constexpr Float min_roughness = 0.001;
  Float roughness = roughness_texture->grayScaleValue(uv);
  roughness = (min_roughness < roughness)
      ? zisc::power<2>(roughness)
      : zisc::power<2>(min_roughness);
  ZISC_ASSERT(zisc::isInClosedBounds(roughness, 0.0, 1.0),
              "The roughness is out of range [0, 1].");
  return roughness;
}

} // namespace nanairo

#endif // NANAIRO_SURFACE_MODEL_INL_HPP
