/*!
  \file algorithm.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_ALGORITHM_HPP_
#define _ZISC_ALGORITHM_HPP_

// Standard C++ library
#include <vector>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

//! Return the iterator pointing that the element is greatest <= value
template <typename RandomAccessIterator, typename Type>
RandomAccessIterator searchBinaryTree(const RandomAccessIterator begin, 
                                      const RandomAccessIterator end,
                                      const Type& value);

//! Transform a sorted unique array to a binary tree
template <typename RandomAccessIterator>
void toBinaryTree(RandomAccessIterator begin, RandomAccessIterator end);

//! Calculate the 32bit hash of string using FNV-1a
constexpr uint32 toHash32(const char* string);

//! Calculate the 64bit hash of string using FNV-1a
constexpr uint64 toHash64(const char* string);

} // namespace zisc

#include "algorithm-inl.hpp"

#endif // _ZISC_ALGORITHM_HPP_
