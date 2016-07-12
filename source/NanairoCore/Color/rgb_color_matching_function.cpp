/*!
  \file rgb_color_matching_function.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rgb_color_matching_function.hpp"
// Qt
#include <QString>
// Nanairo
#include "spectral_distribution.hpp"
#include "NanairoCommon/keyword.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RgbColorMatchingFunction::RgbColorMatchingFunction() noexcept
{
  initialize();
}

/*!
  \details
  No detailed.
  */
const SpectralDistribution& RgbColorMatchingFunction::blueBar() const noexcept
{
  return blue_bar_;
}

/*!
  \details
  No detailed.
  */
const SpectralDistribution& RgbColorMatchingFunction::greenBar() const noexcept
{
  return green_bar_;
}

/*!
  \details
  No detailed.
  */
const SpectralDistribution& RgbColorMatchingFunction::redBar() const noexcept
{
  return red_bar_;
}

/*!
  \details
  No detailed.
  */
void RgbColorMatchingFunction::initialize() noexcept
{
  red_bar_ = makeSpectra(":/spectrum/cie_1931_red.csv");
  green_bar_ = makeSpectra(":/spectrum/cie_1931_green.csv");
  blue_bar_ = makeSpectra(":/spectrum/cie_1931_blue.csv");

//  constexpr Float k = 3.782;
//  red_bar_ = red_bar_.normalized() * k;
//  green_bar_ = green_bar_.normalized() * k;
//  blue_bar_ = blue_bar_.normalized() * k;
}

} // namespace nanairo
