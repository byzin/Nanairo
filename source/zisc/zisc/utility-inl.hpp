/*!
  \file utility-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_UTILITY_INL_HPP
#define ZISC_UTILITY_INL_HPP

#include "utility.hpp"
// Standard C++ library
#include <cstdint>
#include <type_traits>

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Type, typename T> inline
constexpr Type cast(const T value) noexcept
{
  return static_cast<Type>(value);
}

/*!
 \details
 No detailed.
*/
template <typename Type> inline
constexpr Type clamp(const Type x, 
                     const Type min_value, 
                     const Type max_value) noexcept
{
  return min(max(x, min_value), max_value);
}

/*!
  */
template <bool kIsLeftClosed, bool kIsRightClosed, typename Type> inline
constexpr bool isInBounds(const Type& value, 
                          const Type& lower, 
                          const Type& upper) noexcept
{
  return ((kIsLeftClosed) ? !(value < lower) : (lower < value)) &&
         ((kIsRightClosed) ? !(upper < value) : (value < upper));
}

/*!
  */
template <typename Type> inline
constexpr bool isInOpenBounds(const Type& value, 
                              const Type& lower, 
                              const Type& upper) noexcept
{
  return isInBounds<false, false>(value, lower, upper);
}

/*!
  */
template <typename Type> inline
constexpr bool isInLClosedROpenBounds(const Type& value, 
                                      const Type& lower, 
                                      const Type& upper) noexcept
{
  return isInBounds<true, false>(value, lower, upper);
}

/*!
  */
template <typename Type> inline
constexpr bool isInLOpenRClosedBounds(const Type& value, 
                                      const Type& lower, 
                                      const Type& upper) noexcept
{
  return isInBounds<false, true>(value, lower, upper);
}

/*!
  */
template <typename Type> inline
constexpr bool isInClosedBounds(const Type& value, 
                                const Type& lower, 
                                const Type& upper) noexcept
{
  return isInBounds<true, true>(value, lower, upper);
}

/*!
 \details
 No detailed.
 */
template <typename Signed> inline
constexpr bool isNegative(const Signed n) noexcept
{
  static_assert(std::is_signed<Signed>::value, "Signed isn't signed type.");
  constexpr auto zero = static_cast<Signed>(0);
  return (n < zero);
}

/*!
  */
template <typename Integer> inline
constexpr bool isOdd(const Integer n) noexcept
{
  static_assert(std::is_integral<Integer>::value, "Integer isn't integer type.");
  constexpr auto one = static_cast<Integer>(1);
  return ((n & one) == one);
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
constexpr const Type& max(const Type& a, const Type& b) noexcept
{
  return (b < a) ? a : b;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
constexpr const Type& min(const Type& a, const Type& b) noexcept
{
  return (b < a) ? b : a;
}

/*!
  \details
  No detailed.
  */
template <typename Type, typename T> inline
constexpr Type treatAs(const T* object) noexcept
{
  return static_cast<Type>(static_cast<const void*>(object));
}

/*!
  \details
  No detailed.
  */
template <typename Type, typename T> inline
constexpr Type treatAs(T* object) noexcept
{
  return static_cast<Type>(static_cast<void*>(object));
}

} // namespace zisc

#endif // ZISC_UTILITY_INL_HPP
