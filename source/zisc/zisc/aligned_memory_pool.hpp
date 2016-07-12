/*!
  \file aligned_memory_pool.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_ALIGNED_MEMORY_POOL_HPP
#define ZISC_ALIGNED_MEMORY_POOL_HPP

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
  AlignedMemoryPool() noexcept;

  //! Move memory data
  AlignedMemoryPool(AlignedMemoryPool&& memory_pool);


  //! Allocate memory
  template <typename Type, typename ...Arguments>
  Type* allocate(Arguments&& ...arguments) noexcept;

  //! Reset memory pool
  void reset() noexcept;

  //! Return the size of memory pool
  static constexpr uint size() noexcept;

  //! Return the used memory
  uint usedMemory() const noexcept;

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

#endif // ZISC_ALIGNED_MEMORY_POOL_HPP
