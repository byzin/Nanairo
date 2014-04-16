/*!
  \file yxy_color-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_YXY_COLOR_INL_HPP_
#define _NANAIRO_YXY_COLOR_INL_HPP_

// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
YxyColor::YxyColor() :
    Color(0.0, 0.0, 0.0)
{
}

/*!
  \details
  No detailed.
  */
inline
YxyColor::YxyColor(const Float Y, const Float x, const Float y) :
    Color(Y, x, y)
{
}

/*!
  \details
  No detailed.
  */
inline
Float& YxyColor::x()
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
const Float& YxyColor::x() const
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
Float& YxyColor::y()
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
const Float& YxyColor::y() const
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
Float& YxyColor::Y()
{
  return color_[0];
}

/*!
  \details
  No detailed.
  */
inline
const Float& YxyColor::Y() const
{
  return color_[0];
}

} // namespace nanairo

#endif // _NANAIRO_YXY_COLOR_INL_HPP_
