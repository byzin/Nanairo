/*!
  \file checkerboard_texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_CHECKERBOARD_TEXTURE_INL_HPP_
#define _NANAIRO_CHECKERBOARD_TEXTURE_INL_HPP_

#include "checkerboard_texture.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture.hpp"
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
SampledSpectra<kSampleSize> CheckerboardTexture::checkerboardSpectraValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const
{
  using zisc::cast;

  const auto x = cast<uint>(coordinate[0] * width_);
  const auto y = cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return sample(spectra_value_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> getCheckerboardSpectraValue(
    const Texture* texture,
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths)
{
  using zisc::cast;

  auto t = cast<const CheckerboardTexture*>(texture);
  return t->checkerboardSpectraValue(coordinate, wavelengths);
}

} // namespace nanairo

#endif // _NANAIRO_CHECKERBOARD_TEXTURE_INL_HPP_
