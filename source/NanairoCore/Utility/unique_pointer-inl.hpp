/*!
  \file unique_pointer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNIQUE_POINTER_INL_HPP
#define NANAIRO_UNIQUE_POINTER_INL_HPP

#include "unique_pointer.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_chunk.hpp"
#include "zisc/type_traits.hpp"
#include "zisc/utility.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer() noexcept :
    pointer_{nullptr},
    chunk_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer(zisc::MemoryChunk* chunk) noexcept :
    pointer_{nullptr},
    chunk_{nullptr}
{
  reset(chunk);
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
UniquePointer<Type>::UniquePointer(UniquePointer&& other) noexcept :
    pointer_{other.pointer_},
    chunk_{other.release()}
{
}

/*!
  */
template <typename Type> template <typename Super> inline
UniquePointer<Type>::UniquePointer(UniquePointer<Super>&& other,
                                   zisc::EnableIfBaseOf<Type, Super>) noexcept :
    pointer_{zisc::cast<Type*>(other.get())},
    chunk_{other.release()}
{
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
  ZISC_ASSERT(pointer_ != nullptr, "The pointer is null.");
  return *pointer_;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
const Type& UniquePointer<Type>::operator*() const noexcept
{
  ZISC_ASSERT(pointer_ != nullptr, "The pointer is null.");
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
auto UniquePointer<Type>::operator=(zisc::MemoryChunk* chunk) noexcept
    -> UniquePointer&
{
  reset(chunk);
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
auto UniquePointer<Type>::operator=(UniquePointer&& other) noexcept
    -> UniquePointer&
{
  swap(other);
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
  */
template <typename Type> inline
zisc::MemoryChunk* UniquePointer<Type>::getChunk() noexcept
{
  return chunk_;
}

/*!
  */
template <typename Type> inline
const zisc::MemoryChunk* UniquePointer<Type>::getChunk() const noexcept
{
  return chunk_;
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
  */
template <typename Type> inline
zisc::MemoryChunk* UniquePointer<Type>::release() noexcept
{
  auto chunk = chunk_;
  pointer_ = nullptr;
  chunk_ = nullptr;
  return chunk;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
void UniquePointer<Type>::reset(zisc::MemoryChunk* chunk) noexcept
{
  if (!isNull())
    Type::operator delete(pointer_, chunk_);
  chunk_ = chunk;
  pointer_ = (chunk_ != nullptr) ? chunk_->data<Type>() : nullptr;
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
void UniquePointer<Type>::swap(UniquePointer& other) noexcept
{
  {
    auto* tmp = get();
    pointer_ = other.get();
    other.pointer_ = tmp;
  }
  {
    auto* tmp = getChunk();
    chunk_ = other.getChunk();
    other.chunk_ = tmp;
  }
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
bool operator<(const UniquePointer<Type>& lhs,
               const UniquePointer<Type>& rhs) noexcept
{
  return lhs.get() < rhs.get();
}

/*!
  */
template <typename Type, typename ...Types> inline
UniquePointer<Type> makeUnique(zisc::MemoryChunk* chunk,
                               Types&&... arguments) noexcept
{
  new(chunk) Type{std::forward<Types>(arguments)...};
  auto ptr = UniquePointer<Type>{chunk};
  return ptr;
}

} // namespace nanairo

#endif // NANAIRO_UNIQUE_POINTER_INL_HPP
