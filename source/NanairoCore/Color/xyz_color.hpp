/*!
  \file xyz_color.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_XYZ_COLOR_HPP
#define NANAIRO_XYZ_COLOR_HPP

// Zisc
#include "zisc/arithmetic_array.hpp"
// Qt
#include <QColor>
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
class XyzColor : public Color<3>
{
 public:
  //! Initialize XYZ color with 0
  constexpr XyzColor() noexcept;

  //! Create XYZ color
  constexpr XyzColor(const Float x, const Float y, const Float z) noexcept;


  //! Return the x element reference
  constexpr Float& x() noexcept;

  //! Return the x element reference
  constexpr const Float& x() const noexcept;

  //! Return the y element reference
  constexpr Float& y() noexcept;

  //! Return the y element reference
  constexpr const Float& y() const noexcept;

  //! Return the z element reference
  constexpr Float& z() noexcept;

  //! Return the z element reference
  constexpr const Float& z() const noexcept;
};

//! \} Color

} // namespace nanairo

#include "xyz_color-inl.hpp"

#endif // NANAIRO_XYZ_COLOR_HPP
