/*!
  \file unicolor_texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_UNICOLOR_TEXTURE_INL_HPP_
#define _NANAIRO_UNICOLOR_TEXTURE_INL_HPP_

#include "unicolor_texture.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
template <uint> class WavelengthSamples;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> UnicolorTexture::unicolorSpectraValue(
    const Point2& /* coordinate */,
    const WavelengthSamples<kSampleSize>& wavelengths) const
{
  return sample(spectra_value_, wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> getUnicolorSpectraValue(
    const Texture* texture,
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths)
{
  using zisc::cast;

  auto t = cast<const UnicolorTexture*>(texture);
  return t->unicolorSpectraValue(coordinate, wavelengths);
}

} // namespace nanairo

#endif // _NANAIRO_UNICOLOR_TEXTURE_INL_HPP_
