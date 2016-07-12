/*!
  \file utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_UTILITY_HPP
#define ZISC_UTILITY_HPP

// Standard C++ library
#include <type_traits>

namespace zisc {

//! Convert type from T to Type
template <typename Type, typename T>
constexpr Type cast(const T value) noexcept;

//! Clamp the value x to be between min and max
template <typename Type>
constexpr Type clamp(const Type x, 
                     const Type min_value, 
                     const Type max_value) noexcept;

//! Test whether a value is in a range
template <bool kIsLeftClosed = true, bool kIsRightCloed = false, typename Type>
constexpr bool isInBounds(const Type& value, 
                          const Type& lower, 
                          const Type& upper) noexcept;

//! Test whether a value is in a range: (lower, upper)
template <typename Type>
constexpr bool isInOpenBounds(const Type& value, 
                              const Type& lower, 
                              const Type& upper) noexcept;

//! Test whether a value is in a range: [lower, upper)
template <typename Type>
constexpr bool isInLClosedROpenBounds(const Type& value, 
                                      const Type& lower, 
                                      const Type& upper) noexcept;

//! Test whether a value is in a range: [lower, upper)
template <typename Type>
constexpr bool isInLClosedROpenBounds(const Type& value, 
                                      const Type& lower, 
                                      const Type& upper) noexcept;

//! Test whether a value is in a range: (lower, upper]
template <typename Type>
constexpr bool isInLOpenRClosedBounds(const Type& value, 
                                      const Type& lower, 
                                      const Type& upper) noexcept;

//! Test whether a value is in a range: [lower, upper]
template <typename Type>
constexpr bool isInClosedBounds(const Type& value, 
                                const Type& lower, 
                                const Type& upper) noexcept;

//! Test whether the n is negative
template <typename Signed>
constexpr bool isNegative(const Signed n) noexcept;

//! Test whether the n is odd
template <typename Integer>
constexpr bool isOdd(const Integer n) noexcept;

//! Return the large of two elements
template <typename Type>
constexpr const Type& max(const Type& a, const Type& b) noexcept;

//! Return the small of two elements
template <typename Type>
constexpr const Type& min(const Type& a, const Type& b) noexcept;

//! Treat T* as Type*
template <typename Type, typename T>
constexpr Type treatAs(const T* object) noexcept;

//! Treat T* as Type*
template <typename Type, typename T>
constexpr Type treatAs(T* object) noexcept;

} // namespace zisc

#include "utility-inl.hpp"

#endif // ZISC_UTILITY_HPP
