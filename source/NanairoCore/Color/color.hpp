/*!
  \file color.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_COLOR_HPP_
#define _NANAIRO_COLOR_HPP_

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
  Color();

  //! Create a color
  template <typename ...Types>
  Color(const Types ...elements);

  //! Create a color
  Color(const zisc::ArithmeticArray<Float, kN>& color);


  //! Return the element of the color by the index
  Float& operator[](const uint index);

  //! Return the element of the color by the index
  const Float& operator[](const uint index) const;


  //! Clamp the elements of the color to a range [0, 1]
  void clamp(const Float min, const Float max);

  //! Return the raw array
  const zisc::ArithmeticArray<Float, kN>& data() const;

  //! Check whether all elements is zero
  bool isZero() const;

  //! Return the max value
  Float max() const;

  //! All elements are scaled by using the inverse max value
  void scale();

 protected:
  zisc::ArithmeticArray<Float, kN> color_;
};

//! Multiplay color and scalar
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type> = zisc::kEnabler>
Type operator*(const Type& color, const Float scalar);

//! Multiplay color and scalar
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type> = zisc::kEnabler>
Type operator*(const Float scalar, const Type& color);

//! \} Core

} // namespace nanairo

#include "color-inl.hpp"

#endif // _NANAIRO_COLOR_HPP_
