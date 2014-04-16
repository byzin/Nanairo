/*!
  \file value_texture-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_VALUE_TEXTURE_INL_HPP_
#define _NANAIRO_VALUE_TEXTURE_INL_HPP_

#include "value_texture.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
template <uint> class WavelengthSamples;

/*!
  \details
  No detailed.
  */
inline
Float ValueTexture::uniqueValue() const
{
  return value_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> getSpectraValue(
    const Texture* texture,
    const Point2& /* coordinate */,
    const WavelengthSamples<kSampleSize>& wavelengths)
{
  using zisc::cast;

  auto t = cast<const ValueTexture*>(texture);
  return SampledSpectra<kSampleSize>{wavelengths, t->uniqueValue()};
}

} // namespace nanairo

#endif // _NANAIRO_VALUE_TEXTURE_INL_HPP_
