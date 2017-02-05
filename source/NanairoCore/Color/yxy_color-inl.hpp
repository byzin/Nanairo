/*!
  \file yxy_color-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_YXY_COLOR_INL_HPP
#define NANAIRO_YXY_COLOR_INL_HPP

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
constexpr YxyColor::YxyColor() noexcept :
    Color()
{
}

/*!
  \details
  No detailed.
  */
inline
constexpr YxyColor::YxyColor(const Float Y, const Float x, const Float y) noexcept :
    Color(Y, x, y)
{
}

/*!
  \details
  No detailed.
  */
inline
constexpr Float& YxyColor::x() noexcept
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
constexpr const Float& YxyColor::x() const noexcept
{
  return color_[1];
}

/*!
  \details
  No detailed.
  */
inline
constexpr Float& YxyColor::y() noexcept
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
constexpr const Float& YxyColor::y() const noexcept
{
  return color_[2];
}

/*!
  \details
  No detailed.
  */
inline
constexpr Float& YxyColor::Y() noexcept
{
  return color_[0];
}

/*!
  \details
  No detailed.
  */
inline
constexpr const Float& YxyColor::Y() const noexcept
{
  return color_[0];
}

} // namespace nanairo

#endif // NANAIRO_YXY_COLOR_INL_HPP
