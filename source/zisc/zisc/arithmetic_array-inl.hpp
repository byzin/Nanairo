/*!
  \file arithmetic_array-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_ARITHMETIC_ARRAY_INL_HPP_
#define _ZISC_ARITHMETIC_ARRAY_INL_HPP_

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
ArithmeticArray<Arithmetic, kN>::ArithmeticArray()
{
  fill(cast<Arithmetic>(0));
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
ArithmeticArray<Arithmetic, kN>::ArithmeticArray(const Types ...values)
{
  fill(cast<Arithmetic>(0));
  setElements(values...);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN>::ArithmeticArray(const ArithmeticArray& array)
{
  for (uint index = 0; index < size(); ++index)
    elements_[index] = array.elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::begin() -> iterator
{
  return &elements_[0];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::begin() const -> const_iterator
{
  return &elements_[0];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::cbegin() const -> const_iterator
{
  return &elements_[0];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::end() -> iterator
{
  return &elements_[size()];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::end() const -> const_iterator
{
  return &elements_[size()];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::cend() const -> const_iterator
{
  return &elements_[size()];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator+(const ArithmeticArray& other) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = elements_[index] + other.elements_[index];
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator-(const ArithmeticArray& other) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = elements_[index] - other.elements_[index];
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator*(const Arithmetic scalar) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = elements_[index] * scalar;
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator*(const ArithmeticArray& other) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = elements_[index] * other.elements_[index];
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator/(const Arithmetic scalar) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = elements_[index] / scalar;
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator/(const ArithmeticArray& other) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = elements_[index] / other.elements_[index];
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator+=(const ArithmeticArray& other)
    -> ArithmeticArray&
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
auto ArithmeticArray<Arithmetic, kN>::operator-=(const ArithmeticArray& other)
    -> ArithmeticArray&
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
auto ArithmeticArray<Arithmetic, kN>::operator*=(const Arithmetic scalar)
    -> ArithmeticArray&
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
auto ArithmeticArray<Arithmetic, kN>::operator*=(const ArithmeticArray& other)
    -> ArithmeticArray&
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
auto ArithmeticArray<Arithmetic, kN>::operator/=(const Arithmetic scalar)
    -> ArithmeticArray&
{
  for (uint index = 0; index < size(); ++index)
    elements_[index] /= scalar;
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::operator/=(const ArithmeticArray& other)
    -> ArithmeticArray&
{
  for (uint index = 0; index < size(); ++index)
    elements_[index] /= other.elements_[index];
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic& ArithmeticArray<Arithmetic, kN>::operator[](const uint index)
{
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
const Arithmetic& ArithmeticArray<Arithmetic, kN>::operator[](const uint index) const
{
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void ArithmeticArray<Arithmetic, kN>::clamp(const Arithmetic min,
                                            const Arithmetic max)
{
  for (auto& element : elements_)
    element = zisc::clamp(element, min, max);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::divideScalar(const Arithmetic scalar) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = scalar / elements_[index];
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN>
void ArithmeticArray<Arithmetic, kN>::fill(const Arithmetic value)
{
  for (auto& element : elements_)
    element = value;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic ArithmeticArray<Arithmetic, kN>::get(const uint index) const
{
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool ArithmeticArray<Arithmetic, kN>::isZero() const
{
  for (const auto value : elements_) {
    if (value != cast<Arithmetic>(0))
      return false;
  }
  return true;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic ArithmeticArray<Arithmetic, kN>::max() const
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
Arithmetic ArithmeticArray<Arithmetic, kN>::min() const
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
auto ArithmeticArray<Arithmetic, kN>::maxElements(const ArithmeticArray& other) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = zisc::max(other.elements_[index], elements_[index]);
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto ArithmeticArray<Arithmetic, kN>::minElements(const ArithmeticArray& other) const 
    -> ArithmeticArray
{
  ArithmeticArray array{this};
  for (uint index = 0; index < size(); ++index)
    array.elements_[index] = zisc::min(other.elements_[index], elements_[index]);
  return array;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
constexpr uint ArithmeticArray<Arithmetic, kN>::size()
{
  return kN;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void ArithmeticArray<Arithmetic, kN>::set(const uint index, const Arithmetic value)
{
  elements_[index] = value;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
void ArithmeticArray<Arithmetic, kN>::setElements(const Types ...values)
{
  static_assert(sizeof...(Types) <= size(), "The number of parameters is wrong.");
  setElements<0>(cast<Arithmetic>(values)...);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
ArithmeticArray<Arithmetic, kN>::ArithmeticArray(const ArithmeticArray* /* array */)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> 
template <uint index, typename ...Types> inline
void ArithmeticArray<Arithmetic, kN>::setElements(const Arithmetic value, 
                                                  const Types ...values)
{
  setElements<index>(value);
  setElements<index + 1>(values...);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <uint index> inline
void ArithmeticArray<Arithmetic, kN>::setElements(const Arithmetic value)
{
  elements_[index] = value;
}

namespace zisc_arithmetic_array {

template <typename Arithmetic, uint kN> inline
Arithmetic sumArray(const Arithmetic* elements, EnableIfInteger<Arithmetic> = kEnabler)
{
  Arithmetic sum = elements[0];
  for (uint index = 1; index < kN; ++index)
    sum += elements[index];
  return sum;
}

template <typename Arithmetic, uint kN> inline
Arithmetic sumArray(const Arithmetic* elements, EnableIfFloat<Arithmetic> = kEnabler)
{
  CompensatedSummation<Arithmetic> sum{elements[0]};
  for (uint index = 1; index < kN; ++index)
    sum.add(elements[index]);
  return sum.get();
}

} // namespace zisc_arithmetic_array

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN>
Arithmetic ArithmeticArray<Arithmetic, kN>::sum() const
{
  return zisc_arithmetic_array::sumArray<Arithmetic, kN>(elements_);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool operator==(const ArithmeticArray<Arithmetic, kN>& a, 
                const ArithmeticArray<Arithmetic, kN>& b)
{
  for (uint index = 0; index < kN; ++index) {
    if (a[index] != b[index])
      return false;
  }
  return true;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool operator!=(const ArithmeticArray<Arithmetic, kN>& a, 
                const ArithmeticArray<Arithmetic, kN>& b)
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
    const ArithmeticArray<Arithmetic, kN>& array)
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
    const ArithmeticArray<Arithmetic, kN>& array)
{
  return array.divideScalar(scalar);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic> inline
ArithmeticArray<Arithmetic, 3>  cross(const ArithmeticArray<Arithmetic, 3>& a, 
                                      const ArithmeticArray<Arithmetic, 3>& b)
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
Arithmetic dot(const ArithmeticArray<Arithmetic, kN>& a, 
               const ArithmeticArray<Arithmetic, kN>& b)
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
    const ArithmeticArray<Arithmetic, kN>& b)
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
    const ArithmeticArray<Arithmetic, kN>& b)
{
  return a.minElements(b);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void minMaxElements(ArithmeticArray<Arithmetic, kN>& a,
                    ArithmeticArray<Arithmetic, kN>& b)
{
  for (uint i = 0; i < kN; ++i) {
    if (b[i] < a[i]) {
      const Arithmetic t = a[i];
      a[i] = b[i];
      b[i] = t;
    }
  }
}

} // namespace zisc

#endif // _ZISC_ARITHMETIC_ARRAY_INL_HPP_
