/*!
  \file color.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_COLOR_HPP
#define NANAIRO_COLOR_HPP

// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/type_traits.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kN>
class Color
{
 public:
  //! Creaet a color
  constexpr Color() noexcept;

  //! Create a color
  constexpr Color(const Float c1, const Float c2) noexcept;

  //! Create a color
  constexpr Color(const Float c1, const Float c2, const Float c3) noexcept;

  //! Create a color
  constexpr Color(const Float c1, const Float c2, const Float c3, const Float c4) noexcept;


  //! Return the element of the color by the index
  constexpr Float& operator[](const uint index) noexcept;

  //! Return the element of the color by the index
  constexpr const Float& operator[](const uint index) const noexcept;


  //! Clamp the elements of the color to a range [0, 1]
  constexpr void clampAll(const Float min, const Float max) noexcept;

  //! Return the raw array
  constexpr const zisc::ArithmeticArray<Float, kN>& data() const noexcept;

  //! Check whether all elements are between \p lower and \p upper
  constexpr bool isAllInBounds(const Float lower, const Float upper) const noexcept;

  //! Check whether all elements are zero
  constexpr bool isAllZero() const noexcept;

  //! Return the max value
  constexpr Float max() const noexcept;

  //! Return the min value
  constexpr Float min() const noexcept;

  //! All elements are scaled by using the inverse max value
  constexpr void scale() noexcept;

 protected:
  zisc::ArithmeticArray<Float, kN> color_;
};

//! Multiplay color and scalar
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type> = zisc::kEnabler>
constexpr Type operator*(const Type& color, const Float scalar) noexcept;

//! Multiplay color and scalar
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type> = zisc::kEnabler>
constexpr Type operator*(const Float scalar, const Type& color) noexcept;

//! \} Core

} // namespace nanairo

#include "color-inl.hpp"

#endif // NANAIRO_COLOR_HPP
