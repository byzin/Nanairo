/*!
  \file color_space.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_COLOR_SPACE_HPP
#define NANAIRO_COLOR_SPACE_HPP

// Zisc
#include "zisc/algorithm.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

enum class ColorSpaceType : uint32
{
  kSRgbD65                     = zisc::toHash32("sRGBD65"),
  kSRgbD50                     = zisc::toHash32("sRGBD50"),
  kAdobeRgbD65                 = zisc::toHash32("AdobeRGBD65"),
  kAdobeRgbD50                 = zisc::toHash32("AdobeRGBD50")
};

//! Get XYZ to RGB matrix.
Matrix3x3 getXyzToRgbMatrix(const ColorSpaceType color_space) noexcept;

//! Get RGB to XYZ matrix.
Matrix3x3 getRgbToXyzMatrix(const ColorSpaceType color_space) noexcept;

//! \} Core

} // namespace nanairo

#endif // _NANAIRO_COLOR_SPACE_HPP_
