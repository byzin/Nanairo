/*!
  \file xyz_color-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _REFLECT_XYZ_COLOR_INL_HPP_
#define _REFLECT_XYZ_COLOR_INL_HPP_

#include "xyz_color.hpp"
// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "rgb_color.hpp"
#include "yxy_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
XyzColor::XyzColor()
{
}

/*!
  \details
  No detailed.
  */
inline
XyzColor::XyzColor(const Float x, const Float y, const Float z) :
    Color(x, y, z)
{
}

/*!
  \details
  No detailed.
  */
inline
XyzColor::XyzColor(const zisc::ArithmeticArray<Float, 3>& color) :
    Color(color)
{
}

/*!
  \details
  No detailed.
  */
inline
Float& XyzColor::x()
{
  return color_[0];
}

/*!
  \details
  No detailed.
  */
inline
const Float& XyzColor::x() const
{
  return color_[0];
}

/*!
  \details
  No detailed.
  */
inline
Float& XyzColor::y()
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
const Float& XyzColor::y() const
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
Float& XyzColor::z()
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
const Float& XyzColor::z() const
{
  return color_[2];
}

} // namespace nanairo

#endif // _REFLECT_XYZ_COLOR_INL_HPP_
