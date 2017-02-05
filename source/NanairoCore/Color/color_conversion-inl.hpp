/*!
  \file color_conversion-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_COLOR_CONVERSION_INL_HPP
#define NANAIRO_COLOR_CONVERSION_INL_HPP

#include "color_conversion.hpp"
// Standard C++ library
#include <tuple>
// Qt
#include <QColor>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/math.hpp"
#include "zisc/matrix.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "rgb_color.hpp"
#include "xyz_color.hpp"
#include "yxy_color.hpp"
#include "xyz_color_matching_function.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
RgbColor ColorConversion::toRgb(const QColor& rgb) noexcept
{
  using zisc::cast;
  constexpr Float k = zisc::invert(255.0);
  return RgbColor{k * cast<Float>(rgb.red()),
                  k * cast<Float>(rgb.green()),
                  k * cast<Float>(rgb.blue())};
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
QRgb ColorConversion::toQRgb(const RgbColor& rgb) noexcept
{
  using zisc::cast;
  constexpr Float scale = 255.0;
  const auto qt_color = scale * rgb.data();
  return qRgb(cast<int>(qt_color[0]), cast<int>(qt_color[1]), cast<int>(qt_color[2]));
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
