/*!
  \file unique_pointer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNIQUE_POINTER_HPP
#define NANAIRO_UNIQUE_POINTER_HPP

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
  UniquePointer(Type* pointer) noexcept;

  //! Receive the pointer from an another unique pointer
  UniquePointer(UniquePointer&& pointer) noexcept;

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
  UniquePointer& operator=(Type* pointer) noexcept;

  //! Move the pointer
  UniquePointer& operator=(UniquePointer&& pointer) noexcept;


  //! Return a pointer to the managed object 
  Type* get() noexcept;

  //! Return a pointer to the managed object 
  const Type* get() const noexcept;

  //! Check if there is not associated managed object
  bool isNull() const noexcept;

  //! Replace the managed object
  void reset(Type* pointer = nullptr) noexcept;

  //! Swap the managed objects with another
  void swap(UniquePointer& pointer) noexcept;

 private:
  Type* pointer_;
};

//! Compare the pointer address
template <typename Type>
bool operator<(const UniquePointer<Type>& a, const UniquePointer<Type>& b) noexcept;

//! \} Core

} // namespace nanairo

#include "unique_pointer-inl.hpp"

#endif // NANAIRO_UNIQUE_POINTER_HPP
