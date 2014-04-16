/*!
  \file type_traits-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_TYPE_TRAITS_INL_HPP_
#define _ZISC_TYPE_TRAITS_INL_HPP_

#include "type_traits.hpp"
// Standard C++ library
#include <type_traits>
#include <iterator>

namespace zisc {

namespace zisc_type_traits {

//! Check if Type is boolean type
template <typename Type>
using IsBoolean = std::is_same<bool, typename std::remove_cv<Type>::type>;

//! Check if Type is bloating point type
template <typename Type>
using IsFloat = std::is_floating_point<Type>;

//! Check if Type is integer type
template <typename Type>
struct IsInteger
{
  static constexpr bool value = std::is_integral<Type>::value && 
                                !IsBoolean<Type>::value;
};

//! Check if Type is unsigned integer type
template <typename Type>
struct IsUnsignedInteger
{
  static constexpr bool value = IsInteger<Type>::value && 
                                std::is_unsigned<Type>::value;
};

//! Check if Type is signed integer type
template <typename Type>
struct IsSignedInteger
{
  static constexpr bool value = IsInteger<Type>::value && 
                                !IsUnsignedInteger<Type>::value;
};

//! Check if Iterator and IteratorTag are same iterator condition
template <typename Iterator, typename IteratorTag>
using IsSameIteratorCondition = 
    std::is_same<IteratorTag, 
                 typename std::iterator_traits<Iterator>::iterator_category>;

//! Check if Type is random access iterator
template <typename Type>
using IsRandomAccessIterator =
    IsSameIteratorCondition<Type, std::random_access_iterator_tag>;

} // namespace zisc_type_traits

} // namespace zisc

#endif // _ZISC_TYPE_TRAITS_INL_HPP_
