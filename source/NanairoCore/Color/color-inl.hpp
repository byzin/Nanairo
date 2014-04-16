/*!
  \file color-inl.hpp
  \author zin
  */

#ifndef _NANAIRO_COLOR_INL_HPP_
#define _NANAIRO_COLOR_INL_HPP_

#include "color.hpp"
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/type_traits.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kN> inline
Color<kN>::Color()
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> template <typename ...Types> inline
Color<kN>::Color(const Types ...elements) :
    color_{elements...}
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
Color<kN>::Color(const zisc::ArithmeticArray<Float, kN>& color) :
    color_{color}
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
Float& Color<kN>::operator[](const uint index)
{
  return color_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
const Float& Color<kN>::operator[](const uint index) const
{
  return color_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
void Color<kN>::clamp(const Float min, const Float max)
{
  for (uint index = 0; index < kN; ++index)
    color_[index] = zisc::clamp(color_[index], min, max);
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
const zisc::ArithmeticArray<Float, kN>& Color<kN>::data() const
{
  return color_;
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
bool Color<kN>::isZero() const
{
  return color_.isZero();
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
Float Color<kN>::max() const
{
  return color_.max();
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
void Color<kN>::scale()
{
  const Float scale_factor = 1.0 / max();
  color_ = color_ * scale_factor;
}

/*!
  \details
  No detailed.
  */
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type>> inline
Type operator*(const Type& color, const Float scalar)
{
  return Type{color.data() * scalar};
}

/*!
  \details
  No detailed.
  */
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type>> inline
Type operator*(const Float scalar, const Type& color)
{
  return color * scalar;
}

} // namespace nanairo

#endif // _NANAIRO_COLOR_INL_HPP_
