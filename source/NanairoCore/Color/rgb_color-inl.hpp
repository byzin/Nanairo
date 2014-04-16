/*!
  \file rgb_color-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RGB_COLOR_INL_HPP_
#define _NANAIRO_RGB_COLOR_INL_HPP_

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
RgbColor::RgbColor()
{
}

/*!
  \details
  No detailed.
  */
inline
RgbColor::RgbColor(const Float red, const Float green, const Float blue) :
    Color(red, green, blue)
{
}

/*!
  \details
  No detailed.
  */
inline
RgbColor::RgbColor(const QColor& color) :
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
RgbColor::RgbColor(const zisc::ArithmeticArray<Float, 3>& color) :
    Color(color)
{
}

/*!
  \details
  No detailed.
  */
inline
void RgbColor::correctGamma(const Float gamma)
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
Float& RgbColor::blue()
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
const Float& RgbColor::blue() const
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
Float& RgbColor::green()
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
const Float& RgbColor::green() const
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
Float& RgbColor::red()
{
  return color_[0];
}

/*!
  \details
  No detailed.
  */
inline
const Float& RgbColor::red() const
{
  return color_[0];
}

/*!
  \details
  No detailed.
  */
inline
QRgb RgbColor::toQRgb() const
{
  using zisc::cast;

  const auto qt_color = color_ * 255.0;
  return qRgb(cast<int>(qt_color[0]), cast<int>(qt_color[1]), cast<int>(qt_color[2]));
}

} // namespace nanairo

#endif // _NANAIRO_RGB_COLOR_INL_HPP_
