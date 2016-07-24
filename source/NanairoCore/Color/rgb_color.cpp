/*!
  \file rgb_color.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rgb_color.hpp"
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/matrix.hpp"
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
XyzColor RgbColor::toXyz(const Matrix3x3& to_xyz_matrix) const noexcept
{
  return XyzColor{to_xyz_matrix * color_};
}

} // namespace nanairo
