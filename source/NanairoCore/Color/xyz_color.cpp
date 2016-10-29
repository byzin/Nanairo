/*!
  \file xyz_color.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "xyz_color.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/matrix.hpp"
// Nanairo
#include "rgb_color.hpp"
#include "yxy_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RgbColor XyzColor::toRgb(const Matrix3x3& to_rgb_matrix) const noexcept
{
  return RgbColor{to_rgb_matrix * color_};
}

/*!
  \details
  No detailed.
  */
YxyColor XyzColor::toYxy() const noexcept
{
  ZISC_ASSERT(0.0 < y(), "The y is not positive.");
  const Float normalization = 1.0 / color_.sum();
  return YxyColor{y(), x() * normalization, y() * normalization};
}

} // namespace nanairo

