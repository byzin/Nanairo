/*!
  \file texture_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TEXTURE_MODEL_INL_HPP
#define NANAIRO_TEXTURE_MODEL_INL_HPP

#include "texture_model.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
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
SampledSpectra<kSampleSize> TextureModel::emissiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  using zisc::cast;

  SampledSpectra<kSampleSize> spectra{wavelengths};

  const auto texture_type = type();
  switch (texture_type) {
   case TextureType::Value: {
    spectra = getValueEmissiveValue(coordinate, wavelengths);
    break;
   }
   case TextureType::Unicolor: {
    spectra = getUnicolorEmissiveValue(coordinate, wavelengths);
    break;
   }
   case TextureType::Checkerboard: {
    spectra = getCheckerboardEmissiveValue(coordinate, wavelengths);
    break;
   }
   case TextureType::Image: {
    spectra = getImageEmissiveValue(coordinate, wavelengths);
    break;
   }
   default: {
    zisc::raiseError("TextureError: emissiveValue method is not implemented.");
    break;
   }
  }
  return spectra;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::reflectiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  using zisc::cast;

  SampledSpectra<kSampleSize> spectra{wavelengths};

  const auto texture_type = type();
  switch (texture_type) {
   case TextureType::Value: {
    spectra = getValueReflectiveValue(coordinate, wavelengths);
    break;
   }
   case TextureType::Unicolor: {
    spectra = getUnicolorReflectiveValue(coordinate, wavelengths);
    break;
   }
   case TextureType::Checkerboard: {
    spectra = getCheckerboardReflectiveValue(coordinate, wavelengths);
    break;
   }
   case TextureType::Image: {
    spectra = getImageReflectiveValue(coordinate, wavelengths);
    break;
   }
   default: {
    zisc::raiseError("TextureError: reflectiveValue method is not implemented.");
    break;
   }
  }
  return spectra;
}

} // namespace nanairo

#endif // NANAIRO_TEXTURE_MODEL_INL_HPP
