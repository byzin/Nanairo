/*!
  \file color_conversion.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_COLOR_CONVERSION_HPP
#define NANAIRO_COLOR_CONVERSION_HPP

// Standard C++ library
#include <tuple>
// Nanairo
#include "rgba_32.hpp"
#include "rgb_color.hpp"
#include "xyz_color.hpp"
#include "yxy_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Color
//! \{

/*!
  \brief Convert XYZ to spectra
  */
class ColorConversion
{
 public:
  //! Convert integer RGB to float RGB
  static RgbColor toFloatRgb(const Rgba32& rgb) noexcept;

  //! Convert float RGB to integer RGB
  static Rgba32 toIntRgb(const RgbColor& rgb) noexcept;

  //! Convert XYZ to RGB
  static RgbColor toRgb(const XyzColor& xyz,
                        const Matrix3x3& to_rgb) noexcept;

  //! Convert RGB to XYZ
  static XyzColor toXyz(const RgbColor& rgb,
                        const Matrix3x3& to_xyz) noexcept;

  //! Convert Yxy to XYZ
  static constexpr XyzColor toXyz(const YxyColor& yxy) noexcept;

  //! Convert XYZ to Yxy
  static constexpr YxyColor toYxy(const XyzColor& xyz) noexcept;
};

//! \} Color

} // namespace nanairo

#include "color_conversion-inl.hpp"

#endif // NANAIRO_COLOR_CONVERSION_HPP
