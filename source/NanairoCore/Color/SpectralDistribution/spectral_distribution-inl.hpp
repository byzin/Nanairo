/*!
  \file spectral_distribution-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRAL_DISTRIBUTION_INL_HPP
#define NANAIRO_SPECTRAL_DISTRIBUTION_INL_HPP

#include "spectral_distribution.hpp"
// Zisc
#include "zisc/arith_array.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  */
inline
Float SpectralDistribution::operator[](const uint index) const noexcept
{
  return get(index);
}

/*!
  \details
  No detailed.
  */
inline
Float SpectralDistribution::getByWavelength(const uint16 wavelength) const noexcept
{
  return get(getIndex(wavelength));
}

/*!
  */
inline
void SpectralDistribution::setByWavelength(const uint16 wavelength,
                                           const Float intensity) noexcept
{
  return set(getIndex(wavelength), intensity);
}

} // namespace nanairo

#endif // NANAIRO_SPECTRAL_DISTRIBUTION_INL_HPP
