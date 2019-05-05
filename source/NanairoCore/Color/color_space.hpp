/*!
  \file color_space.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_COLOR_SPACE_HPP
#define NANAIRO_COLOR_SPACE_HPP

// Zisc
#include "zisc/fnv_1a_hash_engine.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

enum class ColorSpaceType : uint32
{
  kSRgbD65                     = zisc::Fnv1aHash32::hash("sRGBD65"),
  kSRgbD50                     = zisc::Fnv1aHash32::hash("sRGBD50"),
  kAdobeRgbD65                 = zisc::Fnv1aHash32::hash("AdobeRGBD65"),
  kAdobeRgbD50                 = zisc::Fnv1aHash32::hash("AdobeRGBD50")
};

//! Get XYZ to RGB matrix.
Matrix3x3 getXyzToRgbMatrix(const ColorSpaceType color_space) noexcept;

//! Get RGB to XYZ matrix.
Matrix3x3 getRgbToXyzMatrix(const ColorSpaceType color_space) noexcept;

//! \} Core

} // namespace nanairo

#endif // _NANAIRO_COLOR_SPACE_HPP_
