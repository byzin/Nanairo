/*!
  \file xyz_color.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_XYZ_COLOR_HPP
#define NANAIRO_XYZ_COLOR_HPP

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
  XyzColor() noexcept;

  //! Create XYZ color
  XyzColor(const Float x, const Float y, const Float z) noexcept;

  //! Create XYZ color
  XyzColor(const zisc::ArithmeticArray<Float, 3>& color) noexcept;


  //! Convert XYZ to RGB.
  RgbColor toRgb(const Matrix3x3& to_rgb_matrix) const noexcept;

  //! Convert XYZ to Yxy.
  YxyColor toYxy() const noexcept;

  //! Return the x element reference
  Float& x() noexcept;

  //! Return the x element reference
  const Float& x() const noexcept;

  //! Return the y element reference
  Float& y() noexcept;

  //! Return the y element reference
  const Float& y() const noexcept;

  //! Return the z element reference
  Float& z() noexcept;

  //! Return the z element reference
  const Float& z() const noexcept;
};

//! \} Color

} // namespace nanairo

#include "xyz_color-inl.hpp"

#endif // NANAIRO_XYZ_COLOR_HPP
