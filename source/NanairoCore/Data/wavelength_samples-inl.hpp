/*!
  \file wavelength_samples-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_WAVELENGTH_SAMPLE_INL_HPP_
#define _NANAIRO_WAVELENGTH_SAMPLE_INL_HPP_

#include "wavelength_samples.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
WavelengthSamples<kSampleSize>::WavelengthSamples() :
    primary_index_{0}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
uint16& WavelengthSamples<kSampleSize>::operator[](const uint index)
{
  return wavelengths_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
uint16 WavelengthSamples<kSampleSize>::operator[](const uint index) const
{
  return wavelengths_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
constexpr Float WavelengthSamples<kSampleSize>::primaryInverseProbability()
{
  return zisc::cast<Float>(size());
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
uint WavelengthSamples<kSampleSize>::primaryWavelengthIndex() const
{
  return zisc::cast<uint>(primary_index_);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void WavelengthSamples<kSampleSize>::setPrimaryWavelength(const uint index)
{
  primary_index_ = zisc::cast<uint16>(index);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
constexpr uint WavelengthSamples<kSampleSize>::size()
{
  return kSampleSize;
}

} // namespace nanairo

#endif // _NANAIRO_WAVELENGTH_SAMPLE_INL_HPP_
