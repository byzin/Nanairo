/*!
  \file color_conversion-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_COLOR_CONVERSION_INL_HPP
#define NANAIRO_COLOR_CONVERSION_INL_HPP

#include "color_conversion.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/matrix.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "rgb_color.hpp"
#include "rgba_32.hpp"
#include "xyz_color.hpp"
#include "yxy_color.hpp"
#include "xyz_color_matching_function.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
RgbColor ColorConversion::toFloatRgb(const Rgba32& rgb) noexcept
{
  return RgbColor{rgb.redFloat(), rgb.greenFloat(), rgb.blueFloat()};
}

/*!
  */
inline
Rgba32 ColorConversion::toIntRgb(const RgbColor& rgb) noexcept
{
  return Rgba32{rgb.red(), rgb.green(), rgb.blue()};
}

/*!
  */
inline
RgbColor ColorConversion::toRgb(const XyzColor& xyz,
                                const Matrix3x3& to_rgb) noexcept
{
  const auto rgb = to_rgb * xyz.data();
  return RgbColor{rgb[0], rgb[1], rgb[2]};
}

/*!
  */
inline
XyzColor ColorConversion::toXyz(const RgbColor& rgb,
                                const Matrix3x3& to_xyz) noexcept
{
  const auto xyz = to_xyz * rgb.data();
  return XyzColor{xyz[0], xyz[1], xyz[2]};
}

/*!
  */
inline
constexpr XyzColor ColorConversion::toXyz(const YxyColor& yxy) noexcept
{
  const Float k = yxy.Y() / yxy.y();
  return XyzColor{k * yxy.x(), yxy.Y(), k * (1.0 - (yxy.x() + yxy.y()))};
}

/*!
  */
inline
constexpr YxyColor ColorConversion::toYxy(const XyzColor& xyz) noexcept
{
  const Float k = zisc::invert(xyz.x() + xyz.y() + xyz.z());
  return YxyColor{xyz.y(), k * xyz.x(), k * xyz.y()};
}

} // namespace nanairo

#endif // NANAIRO_COLOR_CONVERSION_INL_HPP
