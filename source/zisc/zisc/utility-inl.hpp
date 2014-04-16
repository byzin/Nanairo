/*!
  \file utility-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_UTILITY_INL_HPP_
#define _ZISC_UTILITY_INL_HPP_

#include "utility.hpp"
// Standard C++ library
#include <cstdint>
#include <type_traits>

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Type> inline
constexpr const Type& max(const Type& a, const Type& b)
{
  return (a > b) ? a : b;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
constexpr const Type& min(const Type& a, const Type& b)
{
  return (a < b) ? a : b;
}

/*!
  \details
  No detailed.
  */
template <typename Type, typename T> inline
constexpr Type cast(const T value)
{
  return static_cast<Type>(value);
}

/*!
 \details
 No detailed.
*/
template <typename Arithmetic> inline
constexpr Arithmetic clamp(const Arithmetic x, 
                           const Arithmetic min_value, 
                           const Arithmetic max_value)
{
  static_assert(std::is_arithmetic<Arithmetic>::value, "## Must be arithmetic type.");
  return min(max(x, min_value), max_value);
}

/*!
  \details
  No detailed.

  \return 符号ビット
  */
template <> inline
constexpr std::int8_t getSignedBit<std::int8_t>()
{
  return static_cast<std::int8_t>(0x80);
}

/*!
  \details
  No detailed.

  \return 符号ビット
  */
template <> inline
constexpr std::int16_t getSignedBit<std::int16_t>()
{
  return static_cast<std::int16_t>(0x8000);
}

/*!
  \details
  No detailed.

  \return 符号ビット
  */
template <> inline
constexpr std::int32_t getSignedBit<std::int32_t>()
{
  return static_cast<std::int32_t>(0x80000000);
}

/*!
  \details
  No detailed.

  \return 符号ビット
  */
template <> inline
constexpr std::int64_t getSignedBit<std::int64_t>()
{
  return static_cast<std::int64_t>(0x8000000000000000);
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
constexpr Type getSignedBit()
{
  static_assert(std::is_integral<Type>::value && std::is_signed<Type>::value,
                "## Template must be signed integer type.");
  return static_cast<Type>(0);
}

/*!
  \details
  No detailed.
  */
template <typename Type, typename T> inline
constexpr Type treatAs(const T* object)
{
  return static_cast<Type>(static_cast<const void*>(object));
}

/*!
  \details
  No detailed.
  */
template <typename Type, typename T> inline
constexpr Type treatAs(T* object)
{
  return static_cast<Type>(static_cast<void*>(object));
}

} // namespace zisc

#endif // _ZISC_UTILITY_INL_HPP_
