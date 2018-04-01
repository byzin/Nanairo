/*!
  \file unique_pointer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNIQUE_POINTER_HPP
#define NANAIRO_UNIQUE_POINTER_HPP

// Zisc
#include "zisc/type_traits.hpp"

namespace zisc {

// Forward declaration
class MemoryChunk;

} // namespace zisc

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <typename Type>
class UniquePointer
{
 public:
  //! Create a empty pointer
  UniquePointer() noexcept;

  //! Create a unique pointer
  UniquePointer(zisc::MemoryChunk* chunk) noexcept;

  //! Swap data for other data
  UniquePointer(UniquePointer&& other) noexcept;

  //! Move a data from other
  template <typename Super>
  UniquePointer(UniquePointer<Super>&& other,
                zisc::EnableIfBaseOf<Type, Super> = zisc::kEnabler) noexcept;

  //! Delete the managed object
  ~UniquePointer() noexcept;


  //! Access to the object
  Type& operator*() noexcept;

  //! Access to the object
  const Type& operator*() const noexcept;

  //! Access to the object pointer
  Type* operator->() noexcept;

  //! Access to the object pointer
  const Type* operator->() const noexcept;

  //! It is same as calling reset(pointer)
  UniquePointer& operator=(zisc::MemoryChunk* chunk) noexcept;

  //! Move the pointer
  UniquePointer& operator=(UniquePointer&& other) noexcept;


  //! Return a pointer to the managed object 
  Type* get() noexcept;

  //! Return a pointer to the managed object 
  const Type* get() const noexcept;

  //! Return the memory chunk
  zisc::MemoryChunk* getChunk() noexcept;

  //! Return the memory chunk
  const zisc::MemoryChunk* getChunk() const noexcept;

  //! Check if there is not associated managed object
  bool isNull() const noexcept;

  //! Release the data owner
  zisc::MemoryChunk* release() noexcept;

  //! Replace the managed object
  void reset(zisc::MemoryChunk* chunk = nullptr) noexcept;

  //! Swap the managed objects with another
  void swap(UniquePointer& other) noexcept;

 private:
  Type* pointer_;
  zisc::MemoryChunk* chunk_;
};

//! Compare the pointer address
template <typename Type>
bool operator<(const UniquePointer<Type>& lhs,
               const UniquePointer<Type>& rhs) noexcept;

//! Make a unique pointer
template <typename Type, typename ...Types>
UniquePointer<Type> makeUnique(zisc::MemoryChunk* chunk,
                               Types&&... arguments) noexcept;

//! \} Core

} // namespace nanairo

#include "unique_pointer-inl.hpp"

#endif // NANAIRO_UNIQUE_POINTER_HPP
