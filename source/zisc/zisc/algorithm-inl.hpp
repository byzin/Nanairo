/*!
  \file algorithm-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_ALGORITHM_INL_HPP_
#define _ZISC_ALGORITHM_INL_HPP_

#include "algorithm.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
// Zisc
#include "error.hpp"
#include "type_traits.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

namespace zisc_algorithm {

/*!
  \details
  No detailed.
  */
template <typename RandomAccessIterator, typename OutputIterator> inline
void toBinaryTree(RandomAccessIterator begin, 
                  RandomAccessIterator end,
                  const uint index,
                  OutputIterator first)
{
  const uint n = cast<uint>(std::distance(begin, end));
  if (n == 1) {
    auto leaf = begin;
    *(first + index) = std::move(*leaf);
  }
  else if (1 < n) {
    auto size = n;
    uint s = 1;
    while (1 != size) {
      s = s << 1;
      size = size >> 1;
    }
    s = s >> 1;
    // Find the center node
    const auto c = ((3 * s - 1) < n) ? (2 * s - 1) : (n - s);
    auto center = begin + c;
    *(first + index) = std::move(*center);
    // Left child
    const uint left_index = (index << 1) + 1;
    toBinaryTree(begin, center, left_index, first);
    // Right child
    const auto right_index = left_index + 1;
    toBinaryTree(center + 1, end, right_index, first);
  }
}

} // namespace zisc_algorithm

/*!
  \details
  No detailed.
  */
template <typename RandomAccessIterator, typename Type>
RandomAccessIterator searchBinaryTree(const RandomAccessIterator begin,
                                      const RandomAccessIterator end,
                                      const Type& value)
{
  static_assert(kIsRandomAccessIterator<RandomAccessIterator>,
                "Iterator must be random access iterator.");

  const uint n = cast<uint>(std::distance(begin, end));
  uint index = 0;
  while (index < n) {
    const auto node = begin + index;
    index = (index << 1) + ((value < *node) ? 1 : 2);
  }
  while ((index & 1) == 1)
    index = index >> 1;
  ZISC_ASSERT(index != 0, "The value is invalid.");
  index = (index >> 1) - 1;
  return begin + index;
}

/*!
  \details
  No detailed.
  */
template <typename RandomAccessIterator> inline
void toBinaryTree(RandomAccessIterator begin, RandomAccessIterator end)
{
  static_assert(kIsRandomAccessIterator<RandomAccessIterator>,
                "Iterator must be random access iterator.");

  using Type = typename std::iterator_traits<RandomAccessIterator>::value_type;
  ZISC_ASSERT(std::is_sorted(begin, end), "The array must be sorted.");
  ZISC_ASSERT(end == std::unique(begin, end), "The array must be unique.");

  const auto size = std::distance(begin, end);
  if (size != 0) {
    // Create a temp array
    std::vector<Type> array;
    array.reserve(size);
    for (auto iterator = begin; iterator != end; ++iterator)
      array.emplace_back(std::move(*iterator));
    // Transform an array to a binary tree
    zisc_algorithm::toBinaryTree(array.begin(), array.end(), 0, begin);
  }
}

namespace zisc_algorithm {

/*!
  \brief Calculate the hash of string using FNV-1a
  \details
  The details of this algorithm are explained in the following URL
  http://www.isthe.com/chongo/tech/comp/fnv/
  */
template <typename ValueType, ValueType kPrime> inline
constexpr ValueType toHash(const char* string, const ValueType hash)
{
  if (*string != '\0') {
    const ValueType updated_hash = (hash ^ cast<ValueType>(*string)) * kPrime;
    return toHash<ValueType, kPrime>(string + 1, updated_hash);
  }
  else {
    return hash;
  }
}

} // namespace zisc_algorithm  

/*!
  \details
  No detailed.
  */
constexpr uint32 toHash32(const char* string)
{
  constexpr uint32 prime = 16777619;
  constexpr uint32 offset = 2166136261;
  return zisc_algorithm::toHash<uint32, prime>(string, offset);
}

/*!
  \details
  No detailed.
  */
constexpr uint64 toHash64(const char* string)
{
  constexpr uint64 prime = 1099511628211;
  constexpr uint64 offset = 14695981039346656037ull;
  return zisc_algorithm::toHash<uint64, prime>(string, offset);
}

} // namespace zisc

#endif // _ZISC_ALGORITHM_INL_HPP_
