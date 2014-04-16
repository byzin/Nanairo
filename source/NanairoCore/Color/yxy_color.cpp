/*!
  \file yxy_color.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "yxy_color.hpp"
// Nanairo
#include "color.hpp"
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
XyzColor YxyColor::toXyz() const
{
  const Float coefficient = Y() / y();
  return XyzColor{coefficient * x(), Y(), coefficient * (1.0 - x() - y())};
}

} // namespace nanairo
