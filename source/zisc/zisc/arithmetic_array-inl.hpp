/*!
  \file arithmetic_array-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_ARITHMETIC_ARRAY_INL_HPP
#define ZISC_ARITHMETIC_ARRAY_INL_HPP

#include "arithmetic_array.hpp"
// Zisc
#include "compensated_summation.hpp"
#include "math.hpp"
#include "type_traits.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN>::ArithmeticArray() noexcept
{
  fill(cast<Arithmetic>(0));
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
ArithmeticArray<Arithmetic, kN>::ArithmeticArray(const Types ...values) noexcept :
    elements_{{cast<Arithmetic>(values)...}}
{
  constexpr auto num_of_arguments = sizeof...(Types);
  static_assert(num_of_arguments == size(), "The num of arguments isn't match kN.");
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN>::ArithmeticArray(const Array& other) noexcept :
    elements_{other}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN>::ArithmeticArray(
    const ArithmeticArray& other) noexcept :
        elements_{other.elements_}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::begin() noexcept -> iterator
{
  return elements_.begin();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::begin() const noexcept -> const_iterator
{
  return elements_.begin();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::cbegin() const noexcept -> const_iterator
{
  return elements_.cbegin();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::end() noexcept -> iterator
{
  return elements_.end();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::end() const noexcept -> const_iterator
{
  return elements_.end();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::cend() const noexcept -> const_iterator
{
  return elements_.cend();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator+(
    const ArithmeticArray& other) const noexcept -> ArithmeticArray
{
  Array array;
  for (uint index = 0; index < size(); ++index)
    array[index] = elements_[index] + other.elements_[index];
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator-(
    const ArithmeticArray& other) const noexcept -> ArithmeticArray
{
  Array array;
  for (uint index = 0; index < size(); ++index)
    array[index] = elements_[index] - other.elements_[index];
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator*(
    const Arithmetic scalar) const noexcept -> ArithmeticArray
{
  Array array;
  for (uint index = 0; index < size(); ++index)
    array[index] = elements_[index] * scalar;
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator*(
    const ArithmeticArray& other) const noexcept -> ArithmeticArray
{
  Array array;
  for (uint index = 0; index < size(); ++index)
    array[index] = elements_[index] * other.elements_[index];
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator/(
    const Arithmetic scalar) const noexcept -> ArithmeticArray
{
  ZISC_ASSERT(scalar != cast<Arithmetic>(0), "The scalar is zero.");
  Array array;
  for (uint index = 0; index < size(); ++index)
    array[index] = elements_[index] / scalar;
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator/(
    const ArithmeticArray& other) const noexcept -> ArithmeticArray
{
  Array array;
  for (uint index = 0; index < size(); ++index) {
    ZISC_ASSERT(other.elements_[index] != cast<Arithmetic>(0), "The scalar is zero.");
    array[index] = elements_[index] / other.elements_[index];
  }
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator=(
    const ArithmeticArray& other) noexcept -> ArithmeticArray&
{
  for (uint index = 0; index < size(); ++index)
    elements_[index] = other.elements_[index];
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator+=(
    const ArithmeticArray& other) noexcept -> ArithmeticArray&
{
  for (uint index = 0; index < size(); ++index)
    elements_[index] += other.elements_[index];
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator-=(
    const ArithmeticArray& other) noexcept -> ArithmeticArray&
{
  for (uint index = 0; index < size(); ++index)
    elements_[index] -= other.elements_[index];
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator*=(
    const Arithmetic scalar) noexcept -> ArithmeticArray&
{
  for (uint index = 0; index < size(); ++index)
    elements_[index] *= scalar;
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator*=(
    const ArithmeticArray& other) noexcept -> ArithmeticArray&
{
  for (uint index = 0; index < size(); ++index)
    elements_[index] *= other.elements_[index];
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator/=(
    const Arithmetic scalar) noexcept -> ArithmeticArray&
{
  ZISC_ASSERT(scalar != cast<Arithmetic>(0), "The scalar is zero.");
  for (uint index = 0; index < size(); ++index)
    elements_[index] /= scalar;
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator/=(
    const ArithmeticArray& other) noexcept -> ArithmeticArray&
{
  for (uint index = 0; index < size(); ++index) {
    ZISC_ASSERT(other.elements_[index] != cast<Arithmetic>(0), "The scalar is zero.");
    elements_[index] /= other.elements_[index];
  }
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic& ArithmeticArray<Arithmetic, kN>::operator[](
    const uint index) noexcept
{
  return get(index);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
const Arithmetic& ArithmeticArray<Arithmetic, kN>::operator[](
    const uint index) const noexcept
{
  return get(index);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void ArithmeticArray<Arithmetic, kN>::clamp(const Arithmetic min_value,
                                            const Arithmetic max_value) noexcept
{
  for (auto& element : elements_)
    element = zisc::clamp(element, min_value, max_value);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::divideScalar(
    const Arithmetic scalar) const noexcept -> ArithmeticArray
{
  Array array;
  for (uint index = 0; index < size(); ++index) {
    ZISC_ASSERT(elements_[index] != cast<Arithmetic>(0), "The scalar is zero.");
    array[index] = scalar / elements_[index];
  }
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void ArithmeticArray<Arithmetic, kN>::fill(const Arithmetic value) noexcept
{
  elements_.fill(value);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic& ArithmeticArray<Arithmetic, kN>::get(const uint index) noexcept
{
  ZISC_ASSERT(index < size(), "The index is out of range.");
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
const Arithmetic& ArithmeticArray<Arithmetic, kN>::get(const uint index) const noexcept
{
  ZISC_ASSERT(index < size(), "The index is out of range.");
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool ArithmeticArray<Arithmetic, kN>::isZeroArray() const noexcept
{
  bool flag = true;
  for (uint index = 1; index < size(); ++index) {
    constexpr auto zero = cast<Arithmetic>(0);
    if (elements_[index] != zero) {
      flag = false;
      break;
    }
  }
  return flag;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic ArithmeticArray<Arithmetic, kN>::max() const noexcept
{
  Arithmetic max = elements_[0];
  for (uint index = 1; index < size(); ++index)
    max = zisc::max(elements_[index], max);
  return max;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic ArithmeticArray<Arithmetic, kN>::min() const noexcept
{
  Arithmetic min = elements_[0];
  for (uint index = 1; index < size(); ++index)
    min = zisc::min(elements_[index], min);
  return min;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::maxElements(
    const ArithmeticArray& other) const noexcept -> ArithmeticArray
{
  Array array;
  for (uint index = 0; index < size(); ++index)
    array[index] = zisc::max(other.elements_[index], elements_[index]);
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::minElements(
    const ArithmeticArray& other) const noexcept -> ArithmeticArray
{
  Array array;
  for (uint index = 0; index < size(); ++index)
    array[index] = zisc::min(other.elements_[index], elements_[index]);
  return ArithmeticArray{array};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
constexpr uint ArithmeticArray<Arithmetic, kN>::size() noexcept
{
  return kN;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void ArithmeticArray<Arithmetic, kN>::set(const uint index, 
                                          const Arithmetic value) noexcept
{
  ZISC_ASSERT(index < size(), "The index is out of range.");
  elements_[index] = value;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
void ArithmeticArray<Arithmetic, kN>::setElements(const Types ...values) noexcept
{
  constexpr auto num_of_arguments = sizeof...(Types);
  static_assert(num_of_arguments == size(), "The num of arguments isn't match kN.");
  setElements<0>(cast<Arithmetic>(values)...);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic ArithmeticArray<Arithmetic, kN>::sum() const noexcept
{
  return sumArray(elements_);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> 
template <uint index, typename ...Types> inline
void ArithmeticArray<Arithmetic, kN>::setElements(const Arithmetic value, 
                                                  const Types ...values) noexcept
{
  setElements<index>(value);
  setElements<index + 1>(values...);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <uint index> inline
void ArithmeticArray<Arithmetic, kN>::setElements(const Arithmetic value) noexcept
{
  elements_[index] = value;
}

template <typename Arithmetic, uint kN> template <typename Type> inline
Type ArithmeticArray<Arithmetic, kN>::sumArray(
    const std::array<Type, kN>& elements,
    EnableIfInteger<Type>) noexcept
{
  Type sum = elements[0];
  for (uint index = 1; index < kN; ++index)
    sum += elements[index];
  return sum;
}

template <typename Arithmetic, uint kN> template <typename Type> inline
Type ArithmeticArray<Arithmetic, kN>::sumArray(
    const std::array<Type, kN>& elements,
    EnableIfFloat<Type>) noexcept
{
  CompensatedSummation<Type> sum{elements[0]};
  for (uint index = 1; index < kN; ++index)
    sum.add(elements[index]);
  return sum.get();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool operator==(
    const ArithmeticArray<Arithmetic, kN>& a, 
    const ArithmeticArray<Arithmetic, kN>& b) noexcept
{
  bool flag = true;
  for (uint index = 0; index < kN; ++index) {
    if (a[index] != b[index]) {
      flag = false;
      break;
    }
  }
  return flag;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool operator!=(
    const ArithmeticArray<Arithmetic, kN>& a, 
    const ArithmeticArray<Arithmetic, kN>& b) noexcept
{
  return !(a == b);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN> operator*(
    const Arithmetic scalar,
    const ArithmeticArray<Arithmetic, kN>& array) noexcept
{
  return array * scalar;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN> operator/(
    const Arithmetic scalar,
    const ArithmeticArray<Arithmetic, kN>& array) noexcept
{
  return array.divideScalar(scalar);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic> inline
ArithmeticArray<Arithmetic, 3>  cross(
    const ArithmeticArray<Arithmetic, 3>& a, 
    const ArithmeticArray<Arithmetic, 3>& b) noexcept
{
  return ArithmeticArray<Arithmetic, 3>{a[1] * b[2] - a[2] * b[1],
                                        a[2] * b[0] - a[0] * b[2],
                                        a[0] * b[1] - a[1] * b[0]};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic dot(
    const ArithmeticArray<Arithmetic, kN>& a, 
    const ArithmeticArray<Arithmetic, kN>& b) noexcept
{
  Arithmetic dot = a[0] * b[0];
  for (uint index = 1; index < kN; ++index)
    dot += a[index] * b[index];
  return dot;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN> maxElements(
    const ArithmeticArray<Arithmetic, kN>& a,
    const ArithmeticArray<Arithmetic, kN>& b) noexcept
{
  return a.maxElements(b);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN> minElements(
    const ArithmeticArray<Arithmetic, kN>& a,
    const ArithmeticArray<Arithmetic, kN>& b) noexcept
{
  return a.minElements(b);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void minMaxElements(
    ArithmeticArray<Arithmetic, kN>& a,
    ArithmeticArray<Arithmetic, kN>& b) noexcept
{
  for (uint i = 0; i < kN; ++i) {
    if (b[i] < a[i])
      swapElement(a, b, i);
  }
}

template <typename Arithmetic, uint kN> inline
void swapElement(
    const ArithmeticArray<Arithmetic, kN>& a, 
    const ArithmeticArray<Arithmetic, kN>& b, 
    const uint index) noexcept
{
  ZISC_ASSERT(index < kN, "The index is out of range.");
  const Arithmetic tmp = b[index];
  b[index] = a[index];
  a[index] = tmp;
}

} // namespace zisc

#endif // ZISC_ARITHMETIC_ARRAY_INL_HPP
