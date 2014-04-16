/*!
  \file image_texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_IMAGE_TEXTURE_INL_HPP_
#define _NANAIRO_IMAGE_TEXTURE_INL_HPP_

#include "image_texture.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture.hpp"
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
SampledSpectra<kSampleSize> ImageTexture::imageSpectraValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const
{
  using zisc::cast;

  const auto x = cast<uint>(coordinate[0] * width_);
  const auto y = cast<uint>(coordinate[1] * height_);
  const uint index = width_resolution_ * y + x;
  return sample(spectra_table_[color_index_[index]], wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> getImageSpectraValue(
    const Texture* texture,
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths)
{
  using zisc::cast;

  auto t = cast<const ImageTexture*>(texture);
  return t->imageSpectraValue(coordinate, wavelengths);
}

} // namespace nanairo

#endif // _NANAIRO_IMAGE_TEXTURE_INL_HPP_
