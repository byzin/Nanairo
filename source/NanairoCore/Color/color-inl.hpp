/*!
  \file color-inl.hpp
  \author zin
  */

#ifndef NANAIRO_COLOR_INL_HPP
#define NANAIRO_COLOR_INL_HPP

#include "color.hpp"
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/math.hpp"
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
constexpr Color<kN>::Color() noexcept :
    color_{}
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr Color<kN>::Color(const Float c1,
                           const Float c2) noexcept :
    color_{c1, c2}
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr Color<kN>::Color(const Float c1,
                           const Float c2,
                           const Float c3) noexcept :
    color_{c1, c2, c3}
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr Color<kN>::Color(const Float c1,
                           const Float c2,
                           const Float c3,
                           const Float c4) noexcept :
    color_{c1, c2, c3, c4}
{
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr Float& Color<kN>::operator[](const uint index) noexcept
{
  return color_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr const Float& Color<kN>::operator[](const uint index) const noexcept
{
  return color_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr void Color<kN>::clampAll(const Float minimum, const Float maximum) noexcept
{
  color_.clampAll(minimum, maximum);
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr const zisc::ArithmeticArray<Float, kN>& Color<kN>::data() const noexcept
{
  return color_;
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr bool Color<kN>::isAllInBounds(const Float lower, const Float upper) const noexcept
{
  return color_.isAllInBounds(lower, upper);
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr bool Color<kN>::isAllZero() const noexcept
{
  return color_.isAllZero();
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr Float Color<kN>::max() const noexcept
{
  return color_.max();
}

/*!
  */
template <uint kN> inline
constexpr Float Color<kN>::min() const noexcept
{
  return color_.min();
}

/*!
  \details
  No detailed.
  */
template <uint kN> inline
constexpr void Color<kN>::scale() noexcept
{
  const Float s = max();
  const Float scale_factor = (s != 0.0) ? zisc::invert(s) : 0.0;
  color_ = color_ * scale_factor;
}

/*!
  \details
  No detailed.
  */
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type>> inline
constexpr Type operator*(const Type& color, const Float scalar) noexcept
{
  const auto c = color.data() * scalar;
  return Type{c[0], c[1], c[2]};
}

/*!
  \details
  No detailed.
  */
template <typename Type, zisc::EnableIfBaseOf<Color<3>, Type>> inline
constexpr Type operator*(const Float scalar, const Type& color) noexcept
{
  return color * scalar;
}

} // namespace nanairo

#endif // NANAIRO_COLOR_INL_HPP
