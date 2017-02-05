/*!
  \file rgb_color.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RGB_COLOR_HPP
#define NANAIRO_RGB_COLOR_HPP

// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class RgbColor : public Color<3>
{
 public:
  //! Create RGB
  RgbColor() noexcept;

  //! Create RGB
  RgbColor(const Float red, const Float green, const Float blur) noexcept;


  //! Apply gamma correction
  void correctGamma(const Float gamma) noexcept;

  //! Return the blue element reference.
  Float& blue() noexcept;

  //! Return the blue element reference.
  const Float& blue() const noexcept;

  //! Return the green element reference.
  Float& green() noexcept;

  //! Return the green element reference.
  const Float& green() const noexcept;

  //! Return the red element reference.
  Float& red() noexcept;

  //! Return the red element reference.
  const Float& red() const noexcept;
};

//! \} Core

} // namespace nanairo

#include "rgb_color-inl.hpp"

#endif // NANAIRO_RGB_COLOR_HPP
