/*!
  \file color_space.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "color_space.hpp"
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/matrix.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Matrix3x3 getXyzToRgbMatrix(const uint32 color_space) noexcept
{
  using zisc::toHash32;

  switch (color_space) {
   case toHash32(keyword::sRgbD65):
    return Matrix3x3{3.2404542, -1.5371385, -0.4985314,
                     -0.9692660, 1.8760108, 0.0415560,
                     0.0556434, -0.2040259, 1.0572252};
   case toHash32(keyword::sRgbD50):
    return Matrix3x3{3.1338561, -1.6168667, -0.4906146,
                     -0.9787684, 1.9161415, 0.0334540,
                     0.0719453, -0.2289914, 1.4052427};
   case toHash32(keyword::adobeRgbD65):
    return Matrix3x3{2.0413690, -0.5649464, -0.3446944,
                     -0.9692660, 1.8760108, 0.0415560,
                     0.0134474, -0.1183897, 1.0154096};
   case toHash32(keyword::adobeRgbD50):
    return Matrix3x3{1.9624274, -0.6105343, -0.3413404,
                     -0.9787684, 1.9161415, 0.0334540,
                     0.0286869, -0.1406752,  1.3487655};
   default:
    zisc::raiseError("ColorSpaceError: Unsupported color space is specified.");
    break;
  }
  return Matrix3x3{};
}

/*!
  \details
  No detailed.
  */
Matrix3x3 getRgbToXyzMatrix(const uint32 color_space) noexcept
{
  using zisc::toHash32;

  switch (color_space) {
   case toHash32(keyword::sRgbD65):
    return Matrix3x3{0.4124564, 0.3575761, 0.1804375,
                     0.2126729, 0.7151522, 0.0721750,
                     0.0193339, 0.1191920, 0.9503041};
   case toHash32(keyword::sRgbD50):
    return Matrix3x3{0.4360747, 0.3850649, 0.1430804,
                     0.2225045, 0.7168786, 0.0606169,
                     0.0139322, 0.0971045, 0.7141733};
   case toHash32(keyword::adobeRgbD65):
    return Matrix3x3{0.5767309, 0.1855540, 0.1881852,
                     0.2973769, 0.6273491, 0.0752741,
                     0.0270343, 0.0706872, 0.9911085};
   case toHash32(keyword::adobeRgbD50):
    return Matrix3x3{0.6097559, 0.2052401, 0.1492240,
                     0.3111242, 0.6256560, 0.0632197,
                     0.0194811, 0.0608902, 0.7448387};
   default:
    zisc::raiseError("ColorSpaceError: Unsupported color space is specified.");
    break;
  }
  return Matrix3x3{};
}

} // namespace nanairo
