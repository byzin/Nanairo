/*!
  \file unicolor_texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNICOLOR_TEXTURE_INL_HPP
#define NANAIRO_UNICOLOR_TEXTURE_INL_HPP

#include "unicolor_texture.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
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
SampledSpectra<kSampleSize> UnicolorTexture::emissiveValue(
    const Point2& /* coordinate */,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  return sample(*emissive_value_, wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::getUnicolorEmissiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto texture = zisc::cast<const UnicolorTexture*>(this);
  return texture->emissiveValue(coordinate, wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> UnicolorTexture::reflectiveValue(
    const Point2& /* coordinate */,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  return sample(*reflective_value_, wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::getUnicolorReflectiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto texture = zisc::cast<const UnicolorTexture*>(this);
  return texture->reflectiveValue(coordinate, wavelengths);
}

} // namespace nanairo

#endif // NANAIRO_UNICOLOR_TEXTURE_INL_HPP
