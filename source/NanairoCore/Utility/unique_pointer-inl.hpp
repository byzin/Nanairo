/*!
  \file unique_pointer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_UNIQUE_POINTER_INL_HPP_
#define _NANAIRO_UNIQUE_POINTER_INL_HPP_

#include "unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer() :
    pointer_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer(Type* pointer) :
    pointer_{pointer}
{
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer(UniquePointer&& pointer) :
    pointer_{pointer.pointer_}
{
  pointer.pointer_ = nullptr;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::~UniquePointer()
{
  reset();
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
Type& UniquePointer<Type>::operator*()
{
  return *pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
const Type& UniquePointer<Type>::operator*() const
{
  return *pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
Type* UniquePointer<Type>::operator->()
{
  return pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
const Type* UniquePointer<Type>::operator->() const
{
  return pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
auto UniquePointer<Type>::operator=(Type* pointer) -> UniquePointer&
{
  reset(pointer);
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
auto UniquePointer<Type>::operator=(UniquePointer&& pointer) -> UniquePointer&
{
  reset(nullptr);
  swap(pointer);
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
Type* UniquePointer<Type>::get()
{
  return pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
const Type* UniquePointer<Type>::get() const
{
  return pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
bool UniquePointer<Type>::isNull() const
{
  return pointer_ == nullptr;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
void UniquePointer<Type>::reset(Type* pointer)
{
  if (!isNull())
    delete pointer_;
  pointer_ = pointer;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
void UniquePointer<Type>::swap(UniquePointer& pointer)
{
  auto* tmp = get();
  pointer_ = pointer.get();
  pointer.pointer_ = tmp;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
bool operator<(const UniquePointer<Type>& a, const UniquePointer<Type>& b)
{
  return a.get() < b.get();
}

} // namespace nanairo

#endif // _NANAIRO_UNIQUE_POINTER_INL_HPP_
