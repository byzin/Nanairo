/*!
  \file xyz_color_matching_function-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP
#define NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP

#include "xyz_color_matching_function.hpp"
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/const_math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "spectral_distribution.hpp"
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::illuminant() const noexcept
{
  return *illuminant_;
}

/*!
  \details
  No detailed.
  */
inline
XyzColor XyzColorMatchingFunction::toXyzForEmitter(
    const SpectralDistribution& spectra) const noexcept
{
  XyzColor xyz;
  for (uint color = 0; color < 3; ++color) {
    const auto& bar = *bar_[color];
    zisc::CompensatedSummation<Float> s{0.0};
    for (uint i = 0; i < CoreConfig::spectraSize(); ++i)
      s.add(bar[i] * spectra[i]);
    xyz[color] = s.get();
  }
  return xyz;
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::xBar() const noexcept
{
  return *bar_[0];
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::yBar() const noexcept
{
  return *bar_[1];
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::zBar() const noexcept
{
  return *bar_[2];
}

} // namespace nanairo

#endif // NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP
