/*!
  \file wavelength_samples-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_WAVELENGTH_SAMPLE_INL_HPP
#define NANAIRO_WAVELENGTH_SAMPLE_INL_HPP

#include "wavelength_samples.hpp"
// Standard C++ library
#include <array>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
WavelengthSamples::WavelengthSamples() noexcept :
    primary_index_{0}
{
}

/*!
  \details
  No detailed.
  */
inline
constexpr uint WavelengthSamples::size() noexcept
{
  return CoreConfig::wavelengthSampleSize();
}

/*!
  \details
  No detailed.
  */
inline
uint16& WavelengthSamples::operator[](const uint index) noexcept
{
  ZISC_ASSERT(zisc::isInBounds(index, 0u, size()), "The index is out of range.");
  return wavelengths_[index];
}

/*!
  \details
  No detailed.
  */
inline
const uint16& WavelengthSamples::operator[](const uint index) const noexcept
{
  ZISC_ASSERT(zisc::isInBounds(index, 0u, size()), "The index is out of range.");
  return wavelengths_[index];
}

/*!
  \details
  No detailed.
  */
inline
constexpr Float WavelengthSamples::primaryInverseProbability() noexcept
{
  return zisc::cast<Float>(size());
}

/*!
  \details
  No detailed.
  */
inline
constexpr Float WavelengthSamples::primaryProbability() noexcept
{
  constexpr Float probability = zisc::invert(primaryInverseProbability());
  return probability;
}

/*!
  \details
  No detailed.
  */
inline
uint WavelengthSamples::primaryWavelengthIndex() const noexcept
{
  return zisc::cast<uint>(primary_index_);
}

/*!
  \details
  No detailed.
  */
inline
void WavelengthSamples::setPrimaryWavelength(const uint index) noexcept
{
  primary_index_ = zisc::cast<uint16>(index);
}

} // namespace nanairo

#endif // NANAIRO_WAVELENGTH_SAMPLE_INL_HPP
