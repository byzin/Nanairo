/*!
  \file aligned_memory_pool-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_ALIGNED_MEMORY_POOL_INL_HPP_
#define _ZISC_ALIGNED_MEMORY_POOL_INL_HPP_

#include "aligned_memory_pool.hpp"
// Standard C++ library
#include <type_traits>
#include <utility>
// Zisc
#include "error.hpp"
#include "non_copyable.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <uint kSize, uint kAlignment> inline
AlignedMemoryPool<kSize, kAlignment>::AlignedMemoryPool() :
    index_{0}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSize, uint kAlignment> inline
AlignedMemoryPool<kSize, kAlignment>::AlignedMemoryPool(
    AlignedMemoryPool&& memory_pool) :
        index_{memory_pool.index_}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSize, uint kAlignment> 
template <typename Type, typename ...Arguments> inline
Type* AlignedMemoryPool<kSize, kAlignment>::allocate(Arguments&& ...arguments)
{
//  static_assert(kAlignment == std::alignment_of<Type>::value,
//                "## Type alignment doesn't match memory alignment.");

  void* position = static_cast<void*>(&memory_[index_]);

  constexpr uint length = (sizeof(Type) % kAlignment) == 0
      ? sizeof(Type) / kAlignment
      : sizeof(Type) / kAlignment + 1;
  index_ += length;
  return new(position) Type{std::forward<Arguments>(arguments)...};
}

/*!
  \details
  No detailed.
  */
template <uint kSize, uint kAlignment> inline
void AlignedMemoryPool<kSize, kAlignment>::reset()
{
  index_ = 0;
}

/*!
  \details
  No detailed.
  */
template <uint kSize, uint kAlignment> inline
constexpr uint AlignedMemoryPool<kSize, kAlignment>::size()
{
  return kLength * kAlignment;
}

/*!
  \details
  No detailed.
  */
template <uint kSize, uint kAlignment> inline
uint AlignedMemoryPool<kSize, kAlignment>::usedMemory() const
{
  return index_ * kAlignment;
}

} // namespace zisc

#endif // _ZISC_ALIGNED_MEMORY_POOL_INL_HPP_
