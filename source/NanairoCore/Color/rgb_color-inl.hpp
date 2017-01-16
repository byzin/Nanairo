/*!
  \file rgb_color-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RGB_COLOR_INL_HPP
#define NANAIRO_RGB_COLOR_INL_HPP

#include "rgb_color.hpp"
// Qt
#include <QColor>
// Zisc
#include "zisc/math.hpp"
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
RgbColor::RgbColor() noexcept
{
}

/*!
  \details
  No detailed.
  */
inline
RgbColor::RgbColor(const Float red, const Float green, const Float blue) noexcept :
    Color(red, green, blue)
{
}

/*!
  \details
  No detailed.
  */
inline
RgbColor::RgbColor(const QColor& color) noexcept :
    Color(zisc::cast<Float>(color.red()) * (1.0 / 255.0),
          zisc::cast<Float>(color.green()) * (1.0 / 255.0),
          zisc::cast<Float>(color.blue()) * (1.0 / 255.0))
{
}

/*!
  \details
  No detailed.
  */
inline
RgbColor::RgbColor(const zisc::ArithmeticArray<Float, 3>& color) noexcept :
    Color(color)
{
}

/*!
  \details
  No detailed.
  */
inline
void RgbColor::correctGamma(const Float gamma) noexcept
{
  color_.setElements(zisc::pow(red(), gamma),
                     zisc::pow(green(), gamma),
                     zisc::pow(blue(), gamma));
}

/*!
  \details
  No detailed.
  */
inline
Float& RgbColor::blue() noexcept
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
const Float& RgbColor::blue() const noexcept
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
Float& RgbColor::green() noexcept
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
const Float& RgbColor::green() const noexcept
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
Float& RgbColor::red() noexcept
{
  return color_[0];
}

/*!
  \details
  No detailed.
  */
inline
const Float& RgbColor::red() const noexcept
{
  return color_[0];
}

/*!
  \details
  No detailed.
  */
inline
QRgb RgbColor::toQRgb() const noexcept
{
  using zisc::cast;

  constexpr Float scale = 255.0;
  const auto qt_color = scale * color_;
  return qRgb(cast<int>(qt_color[0]), cast<int>(qt_color[1]), cast<int>(qt_color[2]));
}

} // namespace nanairo

#endif // NANAIRO_RGB_COLOR_INL_HPP
