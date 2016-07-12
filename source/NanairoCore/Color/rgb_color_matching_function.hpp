/*!
  \file rgb_color_matching_function.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RGB_COLOR_MATCHING_FUNCTION_HPP
#define NANAIRO_RGB_COLOR_MATCHING_FUNCTION_HPP

// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "spectral_distribution.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class RgbColorMatchingFunction
{
 public:
  //! Create a rgb color matching function
  RgbColorMatchingFunction() noexcept;


  //! Return the blue bar
  const SpectralDistribution& blueBar() const noexcept;

  //! Return the green bar
  const SpectralDistribution& greenBar() const noexcept;

  //! Return the red bar
  const SpectralDistribution& redBar() const noexcept;

 private:
  //! Initialize
  void initialize() noexcept;


  SpectralDistribution red_bar_,
                       green_bar_,
                       blue_bar_;
};

//! \} Core 

} // namespace nanairo

#endif // NANAIRO_RGB_COLOR_MATCHING_FUNCTION_HPP
