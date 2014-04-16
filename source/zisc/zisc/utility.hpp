/*!
  \file utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_UTILITY_HPP_
#define _ZISC_UTILITY_HPP_

// Standard C++ library
#include <type_traits>

namespace zisc {

//! Return the large of two elements
template <typename Type>
constexpr const Type& max(const Type& a, const Type& b);

//! Return the small of two elements
template <typename Type>
constexpr const Type& min(const Type& a, const Type& b);

//! Convert type from T to Type
template <typename Type, typename T>
constexpr Type cast(const T value);

//! Clamp the value x to be between min and max
template <typename Arithmetic>
constexpr Arithmetic clamp(const Arithmetic x, 
                           const Arithmetic min_value, 
                           const Arithmetic max_value);

//! Get the sign bit (MSB)
template <typename SignedInteger>
constexpr SignedInteger getSignedBit();

//! Treat T* as Type*
template <typename Type, typename T>
constexpr Type treatAs(const T* object);

//! Treat T* as Type*
template <typename Type, typename T>
constexpr Type treatAs(T* object);

} // namespace zisc

#include "utility-inl.hpp"

#endif // _ZISC_UTILITY_HPP_
