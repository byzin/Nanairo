/*!
  \file value_texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_VALUE_TEXTURE_INL_HPP
#define NANAIRO_VALUE_TEXTURE_INL_HPP

#include "value_texture.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
template <uint> class WavelengthSamples;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> ValueTexture::emissiveValue(
    const Point2& /* coordinate */,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  return SampledSpectra<kSampleSize>{wavelengths, emissive_value_};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::getValueEmissiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto texture = zisc::cast<const ValueTexture*>(this);
  return texture->emissiveValue(coordinate, wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> ValueTexture::reflectiveValue(
    const Point2& /* coordinate */,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  return SampledSpectra<kSampleSize>{wavelengths, reflective_value_};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> TextureModel::getValueReflectiveValue(
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) const noexcept
{
  const auto texture = zisc::cast<const ValueTexture*>(this);
  return texture->reflectiveValue(coordinate, wavelengths);
}

} // namespace nanairo

#endif // NANAIRO_VALUE_TEXTURE_INL_HPP
