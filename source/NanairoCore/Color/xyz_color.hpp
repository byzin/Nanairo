/*!
  \file xyz_color.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_XYZ_COLOR_HPP_
#define _NANAIRO_XYZ_COLOR_HPP_

// Zisc
#include "zisc/arithmetic_array.hpp"
// Qt
#include <QColor>
// Nanairo
#include "color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward decralation
class RgbColor;
class YxyColor;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class XyzColor : public Color<3>
{
 public:
  //! Initialize XYZ color with 0
  XyzColor();

  //! Create XYZ color
  XyzColor(const Float x, const Float y, const Float z);

  //! Create XYZ color
  XyzColor(const zisc::ArithmeticArray<Float, 3>& color);


  //! Convert XYZ to RGB.
  RgbColor toRgb(const Matrix3x3& to_rgb_matrix) const;

  //! Convert XYZ to Yxy.
  YxyColor toYxy() const;

  //! Return the x element reference
  Float& x();

  //! Return the x element reference
  const Float& x() const;

  //! Return the y element reference
  Float& y();

  //! Return the y element reference
  const Float& y() const;

  //! Return the z element reference
  Float& z();

  //! Return the z element reference
  const Float& z() const;
};

//! \} Color

} // namespace nanairo

#include "xyz_color-inl.hpp"

#endif // _NANAIRO_XYZ_COLOR_HPP_
