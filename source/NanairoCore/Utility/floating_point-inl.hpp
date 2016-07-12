/*!
  \file floating_point-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FLOATING_PONIT_INL_HPP
#define NANAIRO_FLOATING_PONIT_INL_HPP

#include "floating_point.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool isBetweenZeroAndOneFloat(const SampledSpectra<kSampleSize>& sample) noexcept
{
  for (uint index = 0; index < sample.size(); ++index) {
    const Float intensity = sample.intensity(index);
    if (!isBetweenZeroAndOneFloat(intensity))
      return false;
  }
  return true;
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
bool isBetweenZeroAndOneFloat(const Color<kN>& color) noexcept
{
  for (uint index = 0; index < kN; ++index) {
    if (!isBetweenZeroAndOneFloat(color[index]))
      return false;
  }
  return true;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool hasInfFloat(const SampledSpectra<kSampleSize>& sample) noexcept
{
  for (uint index = 0; index < sample.size(); ++index) {
    const Float intensity = sample.intensity(index);
    if (isInfFloat(intensity))
      return true;
  }
  return false;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool hasNanFloat(const SampledSpectra<kSampleSize>& sample) noexcept
{
  for (uint index = 0; index < sample.size(); ++index) {
    const Float intensity = sample.intensity(index);
    if (isNanFloat(intensity))
      return true;
  }
  return false;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool hasNegativeFloat(const SampledSpectra<kSampleSize>& sample) noexcept
{
  for (uint index = 0; index < sample.size(); ++index) {
    const Float intensity = sample.intensity(index);
    if (isNegativeFloat(intensity))
      return true;
  }
  return false;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool hasZeroFloat(const SampledSpectra<kSampleSize>& sample) noexcept
{
  for (uint index = 0; index < sample.size(); ++index) {
    const Float intensity = sample.intensity(index);
    if (isZeroFloat(intensity))
      return true;
  }
  return false;
}

} // namespace nanairo

#endif // NANAIRO_FLOATING_PONIT_INL_HPP
