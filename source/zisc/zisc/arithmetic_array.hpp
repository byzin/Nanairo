/*!
  \file arithmetic_array.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_ARITHMETIC_ARRAY_HPP
#define ZISC_ARITHMETIC_ARRAY_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Zisc
#include "type_traits.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN>
class ArithmeticArray
{
  static_assert(std::is_arithmetic<Arithmetic>::value, 
                "The Arithmetic isn't arithmetic type.");
  static_assert(0 < kN, "kN is 0.");
  using Array = std::array<Arithmetic, kN>;
 public:
  // For STL
  using value_type = typename Array::value_type;
  using size_type = typename Array::size_type;
  using difference_type = typename Array::difference_type;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = typename Array::iterator;
  using const_iterator = typename Array::const_iterator;
  using reverse_iterator = typename Array::reverse_iterator;
  using const_reverse_iterator = typename Array::const_reverse_iterator;


  //! Initialize with 0
  ArithmeticArray() noexcept;

  //! Initialize with values
  template <typename ...Types>
  ArithmeticArray(const Types ...values) noexcept;

  //! Initialize with a array
  ArithmeticArray(const Array& other) noexcept;

  //! Initialize with a array
  ArithmeticArray(const ArithmeticArray& other) noexcept;


  // For STL
  //! Return an iterator to the first element of the container
  iterator begin() noexcept;

  //! Return an iterator to the first element of the container
  const_iterator begin() const noexcept;

  //! Return an iterator to the first element of the container
  const_iterator cbegin() const noexcept;

  //! Return an iterator following the last element of the container
  iterator end() noexcept;

  //! Return an iterator following the last element of the container
  const_iterator end() const noexcept;

  //! Return an iterator following the last element of the container
  const_iterator cend() const noexcept;


  //! Apply addition operation to each element in the array
  ArithmeticArray operator+(const ArithmeticArray& other) const noexcept;

  //! Apply subtraction operation to each element in the array
  ArithmeticArray operator-(const ArithmeticArray& other) const noexcept;

  //! Multiply each element by a scalar
  ArithmeticArray operator*(const Arithmetic scalar) const noexcept;

  //! Apply multiplication operation to each element in the array
  ArithmeticArray operator*(const ArithmeticArray& other) const noexcept;

  //! Divide each element by a scalar
  ArithmeticArray operator/(const Arithmetic scalar) const noexcept;

  //! Apply division operation to each element in the array
  ArithmeticArray operator/(const ArithmeticArray& other) const noexcept;

  //!
  ArithmeticArray& operator=(const ArithmeticArray& other) noexcept;

  //!
  ArithmeticArray& operator+=(const ArithmeticArray& other) noexcept;

  //!
  ArithmeticArray& operator-=(const ArithmeticArray& other) noexcept;

  //!
  ArithmeticArray& operator*=(const Arithmetic scalar) noexcept;

  //!
  ArithmeticArray& operator*=(const ArithmeticArray& other) noexcept;

  //!
  ArithmeticArray& operator/=(const Arithmetic scalar) noexcept;

  //!
  ArithmeticArray& operator/=(const ArithmeticArray& other) noexcept;

  //! Return the reference by index
  Arithmetic& operator[](const uint index) noexcept;

  //! Return the reference by index
  const Arithmetic& operator[](const uint index) const noexcept;


  //! Clamp all elements
  void clamp(const Arithmetic min_value, const Arithmetic max_value) noexcept;

  //! Divide a scalar by a array
  ArithmeticArray divideScalar(const Arithmetic scalar) const noexcept;

  //! Fill the container with specified value
  void fill(const Arithmetic value) noexcept;

  //! Return the reference by index.
  Arithmetic& get(const uint index) noexcept;

  //! Return the reference by index.
  const Arithmetic& get(const uint index) const noexcept;

  //! Check whether all elements are zero
  bool isZeroArray() const noexcept;

  //! Return the largest element
  Arithmetic max() const noexcept;

  //! Return the smallest element
  Arithmetic min() const noexcept;

  //! Compare two arrays and store maximum values
  ArithmeticArray maxElements(const ArithmeticArray& other) const noexcept;

  //! Compare two arrays and store minimum values
  ArithmeticArray minElements(const ArithmeticArray& other) const noexcept;

  //! Return the number of elements
  static constexpr uint size() noexcept;

  //! Set value
  void set(const uint index, const Arithmetic value) noexcept;

  //! Set values
  template <typename ...Types>
  void setElements(const Types ...values) noexcept;

  //! Return the sum of elements
  Arithmetic sum() const noexcept;

 private:
  //! Set value
  template <uint index, typename ...Types>
  void setElements(const Arithmetic value, const Types ...values) noexcept;

  //! Set value
  template <uint index>
  void setElements(const Arithmetic value) noexcept;

  //! Return the sum of integer elements
  template <typename Type>
  static Type sumArray(const std::array<Type, kN>& elements,
                       EnableIfInteger<Type> = kEnabler) noexcept;

  //! Return the sum of float elements
  template <typename Type>
  static Type sumArray(const std::array<Type, kN>& elements,
                       EnableIfFloat<Type> = kEnabler) noexcept;


  Array elements_;
};

//! Check whether two arrays are same
template <typename Arithmetic, uint kN>
bool operator==(
    const ArithmeticArray<Arithmetic, kN>& a, 
    const ArithmeticArray<Arithmetic, kN>& b) noexcept;

//! Check whether two arrays are not same
template <typename Arithmetic, uint kN>
bool operator!=(
    const ArithmeticArray<Arithmetic, kN>& a, 
    const ArithmeticArray<Arithmetic, kN>& b) noexcept;

//! Multiply each element by a scalar
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> operator*(
    const Arithmetic scalar,
    const ArithmeticArray<Arithmetic, kN>& array) noexcept;

//! Divide a scalar by a array
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> operator/(
    const Arithmetic scalar,
    const ArithmeticArray<Arithmetic, kN>& array) noexcept;
                                    
//! Calculate the cross product
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> cross(
    const ArithmeticArray<Arithmetic, kN>& a,
    const ArithmeticArray<Arithmetic, kN>& b) noexcept;

//! Calculate the inner product
template <typename Arithmetic, uint kN>
Arithmetic dot(
    const ArithmeticArray<Arithmetic, kN>& a, 
    const ArithmeticArray<Arithmetic, kN>& b) noexcept;

//! Compare two arrays and return maximum values
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> maxElements(
    const ArithmeticArray<Arithmetic, kN>& a,
    const ArithmeticArray<Arithmetic, kN>& b) noexcept;

//! Compare two arrays and return minimum values
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> minElements(
    const ArithmeticArray<Arithmetic, kN>& a,
    const ArithmeticArray<Arithmetic, kN>& b) noexcept;

//! Compare two arrays and store minimu values to a and maximum values to b
template <typename Arithmetic, uint kN>
void minMaxElements(
    ArithmeticArray<Arithmetic, kN>& a,
    ArithmeticArray<Arithmetic, kN>& b) noexcept;

//! Swap two array elements by index
template <typename Arithmetic, uint kN>
void swapElement(
    const ArithmeticArray<Arithmetic, kN>& a, 
    const ArithmeticArray<Arithmetic, kN>& b, 
    const uint index) noexcept;

} // namespace zisc

#include "arithmetic_array-inl.hpp"

#endif // ZISC_ARITHMETIC_ARRAY_HPP
