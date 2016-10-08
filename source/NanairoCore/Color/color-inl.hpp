/*!
  \file color-inl.hpp
  \author zin
  */

#ifndef NANAIRO_COLOR_INL_HPP
#define NANAIRO_COLOR_INL_HPP

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
Color<kN>::Color() noexcept
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> template <typename ...Types> inline
Color<kN>::Color(const Types ...elements) noexcept :
    color_{elements...}
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
Color<kN>::Color(const zisc::ArithmeticArray<Float, kN>& color) noexcept :
    color_{color}
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
Float& Color<kN>::operator[](const uint index) noexcept
{
  return color_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
const Float& Color<kN>::operator[](const uint index) const noexcept
{
  return color_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
void Color<kN>::clamp(const Float minimum, const Float maximum) noexcept
{
  for (uint index = 0; index < kN; ++index)
    color_[index] = zisc::clamp(color_[index], minimum, maximum);
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
const zisc::ArithmeticArray<Float, kN>& Color<kN>::data() const noexcept
{
  return color_;
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
bool Color<kN>::isZero() const noexcept
{
  return color_.isZero();
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
Float Color<kN>::max() const noexcept
{
  return color_.max();
}

/*!
  */
template <uint kN> inline
Float Color<kN>::min() const noexcept
{
  return color_.min();
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
void Color<kN>::scale() noexcept
{
  const Float scale_factor = 1.0 / max();
  color_ = color_ * scale_factor;
}

/*!
  \details
  No detailed.
  */
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type>> inline
Type operator*(const Type& color, const Float scalar) noexcept
{
  return Type{color.data() * scalar};
}

/*!
  \details
  No detailed.
  */
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type>> inline
Type operator*(const Float scalar, const Type& color) noexcept
{
  return color * scalar;
}

} // namespace nanairo

#endif // NANAIRO_COLOR_INL_HPP
