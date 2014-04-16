/*!
  \file unique_pointer.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_UNIQUE_POINTER_HPP_
#define _NANAIRO_UNIQUE_POINTER_HPP_

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
  UniquePointer();

  //! Create a unique pointer
  UniquePointer(Type* pointer);

  //! Receive the pointer from an another unique pointer
  UniquePointer(UniquePointer&& pointer);

  //! Delete the managed object
  ~UniquePointer();


  //! Access to the object
  Type& operator*();

  //! Access to the object
  const Type& operator*() const;

  //! Access to the object pointer
  Type* operator->();

  //! Access to the object pointer
  const Type* operator->() const;

  //! It is same as calling reset(pointer)
  UniquePointer& operator=(Type* pointer);

  //! Move the pointer
  UniquePointer& operator=(UniquePointer&& pointer);


  //! Return a pointer to the managed object 
  Type* get();

  //! Return a pointer to the managed object 
  const Type* get() const;

  //! Check if there is not associated managed object
  bool isNull() const;

  //! Replace the managed object
  void reset(Type* pointer = nullptr);

  //! Swap the managed objects with another
  void swap(UniquePointer& pointer);

 private:
  Type* pointer_;
};

//! Compare the pointer address
template <typename Type>
bool operator<(const UniquePointer<Type>& a, const UniquePointer<Type>& b);
  
//! \} Core

} // namespace nanairo

#include "unique_pointer-inl.hpp"

#endif // _NANAIRO_UNIQUE_POINTER_HPP_
