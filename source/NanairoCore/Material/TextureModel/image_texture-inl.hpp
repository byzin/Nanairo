/*!
  \file image_texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_IMAGE_TEXTURE_INL_HPP
#define NANAIRO_IMAGE_TEXTURE_INL_HPP

#include "image_texture.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
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
SampledSpectra<kSampleSize> ImageTexture::emissiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = width_resolution_ * y + x;
  return sample(emissive_value_table_[color_index_[index]], wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::getImageEmissiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto texture = zisc::cast<const ImageTexture*>(this);
  return texture->emissiveValue(coordinate, wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> ImageTexture::reflectiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = width_resolution_ * y + x;
  return sample(reflective_value_table_[color_index_[index]], wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::getImageReflectiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto texture = zisc::cast<const ImageTexture*>(this);
  return texture->reflectiveValue(coordinate, wavelengths);
}

} // namespace nanairo

#endif // NANAIRO_IMAGE_TEXTURE_INL_HPP
