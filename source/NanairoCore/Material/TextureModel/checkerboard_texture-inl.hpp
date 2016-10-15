/*!
  \file checkerboard_texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CHECKERBOARD_TEXTURE_INL_HPP
#define NANAIRO_CHECKERBOARD_TEXTURE_INL_HPP

#include "checkerboard_texture.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
template <uint> class WavelengthSamples;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> CheckerboardTexture::emissiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return sample(*emissive_value_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::getCheckerboardEmissiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto texture = zisc::cast<const CheckerboardTexture*>(this);
  return texture->emissiveValue(coordinate, wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> CheckerboardTexture::reflectiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return sample(*reflective_value_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::getCheckerboardReflectiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto texture = zisc::cast<const CheckerboardTexture*>(this);
  return texture->reflectiveValue(coordinate, wavelengths);
}

} // namespace nanairo

#endif // NANAIRO_CHECKERBOARD_TEXTURE_INL_HPP
