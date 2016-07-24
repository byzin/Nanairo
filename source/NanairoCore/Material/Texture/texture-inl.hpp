/*!
  \file texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TEXTURE_INL_HPP
#define NANAIRO_TEXTURE_INL_HPP

#include "texture.hpp"
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "checkerboard_texture.hpp"
#include "image_texture.hpp"
#include "unicolor_texture.hpp"
#include "value_texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> Texture::spectraValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  SampledSpectra<kSampleSize> spectra{wavelengths};

  const auto texture_type = type();
  switch (texture_type) {
   case TextureType::Value:
    spectra = getSpectraValue(this, coordinate, wavelengths);
    break;
   case TextureType::Unicolor:
    spectra = getUnicolorSpectraValue(this, coordinate, wavelengths);
    break;
   case TextureType::Checkerboard:
    spectra = getCheckerboardSpectraValue(this, coordinate, wavelengths);
    break;
   case TextureType::Image:
    spectra = getImageSpectraValue(this, coordinate, wavelengths);
    break;
   default:
    zisc::raiseError("TextureError: spectraValue method is not implemented.");
    break;
  }
  return spectra;
}

} // namespace nanairo

#endif // NANAIRO_TEXTURE_INL_HPP
