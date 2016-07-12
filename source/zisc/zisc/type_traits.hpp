/*!
  \file type_traits.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_TYPE_TRAITS_HPP
#define ZISC_TYPE_TRAITS_HPP

// Standard C++ library
#include <string>
#include <type_traits>

#include "type_traits-inl.hpp"

namespace zisc {

// Type properties

//! Check if Type is boolean type
template <typename Type>
constexpr bool kIsBoolean = zisc_type_traits::IsBoolean<Type>::value;

//! Check if Type is bloating point type
template <typename Type>
constexpr bool kIsFloat = zisc_type_traits::IsFloat<Type>::value;

//! Check if Type is integer type
template <typename Type>
constexpr bool kIsInteger = zisc_type_traits::IsInteger<Type>::value;

//! Check if Type is unsigned integer type
template <typename Type>
constexpr bool kIsUnsignedInteger = zisc_type_traits::IsUnsignedInteger<Type>::value;

//! Check if Type is signed integer type
template <typename Type>
constexpr bool kIsSignedInteger = zisc_type_traits::IsSignedInteger<Type>::value;

template <typename Type>
constexpr bool kIsIterator = zisc_type_traits::IsIterator<Type>::value;

template <typename Type>
constexpr bool kIsRandomAccessIterator = 
    zisc_type_traits::IsRandomAccessIterator<Type>::value;

// SFINAE

constexpr void* kEnabler = nullptr;

template <typename T, typename Type = void* const>
using EnableIfBoolean = typename std::enable_if<kIsBoolean<T>, Type>::type;

template <typename T, typename Type = void* const>
using EnableIfFloat = typename std::enable_if<kIsFloat<T>, Type>::type;

template <typename T, typename Type = void* const>
using EnableIfInteger = typename std::enable_if<kIsInteger<T>, Type>::type;

template <typename T, typename Type = void* const>
using EnableIfUnsignedInteger = 
    typename std::enable_if<kIsUnsignedInteger<T>, Type>::type;

template <typename T, typename Type = void* const>
using EnableIfSignedInteger = 
    typename std::enable_if<kIsSignedInteger<T>, Type>::type;

template <typename T, typename U, typename Type = void* const>
using EnableIfSame = 
    typename std::enable_if<std::is_same<T, U>::value, Type>::type;

template <typename B, typename D, typename Type = void* const>
using EnableIfBaseOf = 
    typename std::enable_if<std::is_base_of<B, D>::value, Type>::type;

template <typename T, typename Type = void* const>
using EnableIfIterator = typename std::enable_if<kIsIterator<T>, Type>::type;

template <typename T, typename Type = void* const>
using EnableIfRandomAccessIterator =
    typename std::enable_if<kIsRandomAccessIterator<T>, Type>::type;

} // namespace zisc

#endif // ZISC_TYPE_TRAITS_HPP
