/*!
  \file unique_pointer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNIQUE_POINTER_INL_HPP
#define NANAIRO_UNIQUE_POINTER_INL_HPP

#include "unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer() noexcept :
    pointer_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer(Type* pointer) noexcept :
    pointer_{pointer}
{
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer(UniquePointer&& pointer) noexcept :
    pointer_{pointer.pointer_}
{
  pointer.pointer_ = nullptr;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::~UniquePointer() noexcept 
{
  reset();
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
Type& UniquePointer<Type>::operator*() noexcept
{
  return *pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
const Type& UniquePointer<Type>::operator*() const noexcept
{
  return *pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
Type* UniquePointer<Type>::operator->() noexcept
{
  return pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
const Type* UniquePointer<Type>::operator->() const noexcept
{
  return pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
auto UniquePointer<Type>::operator=(Type* pointer) noexcept -> UniquePointer&
{
  reset(pointer);
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
auto UniquePointer<Type>::operator=(UniquePointer&& pointer) noexcept
    -> UniquePointer&
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
Type* UniquePointer<Type>::get() noexcept
{
  return pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
const Type* UniquePointer<Type>::get() const noexcept
{
  return pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
bool UniquePointer<Type>::isNull() const noexcept
{
  return pointer_ == nullptr;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
void UniquePointer<Type>::reset(Type* pointer) noexcept
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
void UniquePointer<Type>::swap(UniquePointer& pointer) noexcept
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
bool operator<(const UniquePointer<Type>& a, const UniquePointer<Type>& b) noexcept
{
  return a.get() < b.get();
}

} // namespace nanairo

#endif // NANAIRO_UNIQUE_POINTER_INL_HPP
