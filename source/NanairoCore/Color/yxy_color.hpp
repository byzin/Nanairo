/*!
  \file yxy_color.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_YXY_COLOR_HPP_
#define _NANAIRO_YXY_COLOR_HPP_

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
  YxyColor();

  //! Create Yxy
  YxyColor(const Float Y, const Float x, const Float y);


  //! Return the x element reference
  Float& x();

  //! Return the x element reference
  const Float& x() const;

  //! Return the y element reference
  Float& y();

  //! Return the y element reference
  const Float& y() const;

  //! Return the Y element reference
  Float& Y();

  //! Return the Y element reference
  const Float& Y() const;

  //! Convert to XYZ
  XyzColor toXyz() const;
};

//! \} Core

} // namespace nanairo

#include "yxy_color-inl.hpp"

#endif // _NANAIRO_YXY_COLOR_HPP_
