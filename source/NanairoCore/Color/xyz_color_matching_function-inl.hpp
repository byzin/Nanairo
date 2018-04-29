/*!
  \file xyz_color_matching_function-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP
#define NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP

#include "xyz_color_matching_function.hpp"
// Standard C++ library
#include <memory>
// Nanairo
#include "spectral_distribution.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& XyzColorMatchingFunction::xBar() noexcept
{
  return bar_[0];
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::xBar() const noexcept
{
  return bar_[0];
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& XyzColorMatchingFunction::yBar() noexcept
{
  return bar_[1];
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::yBar() const noexcept
{
  return bar_[1];
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& XyzColorMatchingFunction::zBar() noexcept
{
  return bar_[2];
}

/*!
  \details
  No detailed.
  */
inline
const SpectralDistribution& XyzColorMatchingFunction::zBar() const noexcept
{
  return bar_[2];
}

} // namespace nanairo

#endif // NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_INL_HPP
