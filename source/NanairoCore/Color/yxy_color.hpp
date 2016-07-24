/*!
  \file yxy_color.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_YXY_COLOR_HPP
#define NANAIRO_YXY_COLOR_HPP

// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward decralation
class XyzColor;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class YxyColor : public Color<3>
{
 public:
  //! Create Yxy
  YxyColor() noexcept;

  //! Create Yxy
  YxyColor(const Float Y, const Float x, const Float y) noexcept;


  //! Return the x element reference
  Float& x() noexcept;

  //! Return the x element reference
  const Float& x() const noexcept;

  //! Return the y element reference
  Float& y() noexcept;

  //! Return the y element reference
  const Float& y() const noexcept;

  //! Return the Y element reference
  Float& Y() noexcept;

  //! Return the Y element reference
  const Float& Y() const noexcept;

  //! Convert to XYZ
  XyzColor toXyz() const noexcept;
};

//! \} Core

} // namespace nanairo

#include "yxy_color-inl.hpp"

#endif // NANAIRO_YXY_COLOR_HPP
