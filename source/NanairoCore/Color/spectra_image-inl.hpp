/*!
  \file spectra_image-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRA_IMAGE_INL_HPP
#define NANAIRO_SPECTRA_IMAGE_INL_HPP

#include "spectra_image.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void SpectraImage::addSpectraContribution(
    const uint x,
    const uint y,
    const SampledSpectra<kSampleSize>& contribution) noexcept
{
  volatile Float c = 0.0;
  volatile Float tmp1 = 0.0;
  volatile Float tmp2 = 0.0;

  const uint pixel_index = widthResolution() * y + x;
  auto& pixel = buffer_[pixel_index];
  auto& compensation = compensation_[pixel_index];
  for (uint i = 0; i < kSampleSize; ++i) {
    const uint index = getIndex(contribution.wavelength(i));

    c = compensation[index];
    tmp1 = contribution.intensity(i) - c;
    tmp2 = pixel[index] + tmp1;

    compensation[index] = (tmp2 - pixel[index]) - tmp1;
    pixel[index] = tmp2;
  }
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void addSpectraContribution(
    SpectraImageInterface* image,
    const uint x,
    const uint y,
    const SampledSpectra<kSampleSize>& contribution) noexcept
{
  using zisc::cast; 

  auto spectra_image = cast<SpectraImage*>(image);
  spectra_image->addSpectraContribution(x, y, contribution);
}

} // namespace nanairo

#endif // NANAIRO_SPECTRA_IMAGE_INL_HPP
