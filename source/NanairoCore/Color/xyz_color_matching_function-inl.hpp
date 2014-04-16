/*!
  \file xyz_color_matching_function-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP_
#define _NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP_

#include "xyz_color_matching_function.hpp"
// Zisc
#include "zisc/arithmetic_array.hpp"
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
XyzColor XyzColorMatchingFunction::toXyzInEmissiveCase(
    const SpectralDistribution& spectra) const
{
  auto x = x_bar_ * spectra;
  auto y = y_bar_ * spectra;
  auto z = z_bar_ * spectra;
  return XyzColor{x.sum(), y.sum(), z.sum()};
}

/*!
  \details
  No detailed.
  */
inline
XyzColor XyzColorMatchingFunction::toXyzInReflectiveCase(
    const SpectralDistribution& spectra) const
{
  auto x = x_bar_ * spectra * standard_illuminant_;
  auto y = y_bar_ * spectra * standard_illuminant_;
  auto z = z_bar_ * spectra * standard_illuminant_;
  return XyzColor{k_ * x.sum(), k_ * y.sum(), k_ * z.sum()};
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::xBar() const
{
  return x_bar_;
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::yBar() const
{
  return y_bar_;
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::zBar() const
{
  return z_bar_;
}

} // namespace nanairo

#endif // _NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP_
