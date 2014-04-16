/*!
  \file color_space.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_COLOR_SPACE_HPP_
#define _NANAIRO_COLOR_SPACE_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! Get XYZ to RGB matrix.
Matrix3x3 getXyzToRgbMatrix(const uint32 color_space);

//! Get RGB to XYZ matrix.
Matrix3x3 getRgbToXyzMatrix(const uint32 color_space);

//! \} Core

} // namespace nanairo

#endif // _NANAIRO_COLOR_SPACE_HPP_
