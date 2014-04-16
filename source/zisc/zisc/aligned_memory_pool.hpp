/*!
  \file aligned_memory_pool.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_ALIGNED_MEMORY_POOL_HPP_
#define _ZISC_ALIGNED_MEMORY_POOL_HPP_

// Standard C++ library
#include <type_traits>
// Zisc
#include "non_copyable.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <uint kSize, uint kAlignment>
class AlignedMemoryPool : public NonCopyable
{
 public:
  //! Create a memory pool
  AlignedMemoryPool();

  //! Move memory data
  AlignedMemoryPool(AlignedMemoryPool&& memory_pool);


  //! Allocate memory
  template <typename Type, typename ...Arguments>
  Type* allocate(Arguments&& ...arguments);

  //! Reset memory pool
  void reset();

  //! Return the size of memory pool
  static constexpr uint size();

  //! Return the used memory
  uint usedMemory() const;

 private:
  using AlignedBlock = typename std::aligned_storage<kAlignment, kAlignment>::type;


  static constexpr uint kLength = ((kSize % kAlignment) == 0)
      ? kSize / kAlignment
      : kSize / kAlignment + 1;


  AlignedBlock memory_[kLength];
  uint index_;
};

} // namespace zisc

#include "aligned_memory_pool-inl.hpp"

#endif // _ZISC_ALIGNED_MEMORY_POOL_HPP_
