/*!
  \file arithmetic_array.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_ARITHMETIC_ARRAY_HPP_
#define _ZISC_ARITHMETIC_ARRAY_HPP_

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN>
class ArithmeticArray
{
 public:
  // For STL
  using value_type = Arithmetic;
  using size_type = uint;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;


  using ValueType = Arithmetic;


  //! Initialize with 0
  ArithmeticArray();

  //! Initialize with values
  template <typename ...Types>
  ArithmeticArray(const Types ...values);

  //! Initialize with array
  ArithmeticArray(const ArithmeticArray& array);


  // For STL
  //! Return an iterator to the first element of the container
  iterator begin();

  //! Return an iterator to the first element of the container
  const_iterator begin() const;

  //! Return an iterator to the first element of the container
  const_iterator cbegin() const;

  //! Return an iterator following the last element of the container
  iterator end();

  //! Return an iterator following the last element of the container
  const_iterator end() const;

  //! Return an iterator following the last element of the container
  const_iterator cend() const;


  //! Apply addition operation to each element in the array
  ArithmeticArray operator+(const ArithmeticArray& other) const;

  //! Apply subtraction operation to each element in the array
  ArithmeticArray operator-(const ArithmeticArray& other) const;

  //! Multiply each element by a scalar
  ArithmeticArray operator*(const Arithmetic scalar) const;

  //! Apply multiplication operation to each element in the array
  ArithmeticArray operator*(const ArithmeticArray& other) const;

  //! Divide each element by a scalar
  ArithmeticArray operator/(const Arithmetic scalar) const;

  //! Apply division operation to each element in the array
  ArithmeticArray operator/(const ArithmeticArray& other) const;

  //!
  ArithmeticArray& operator+=(const ArithmeticArray& other);

  //!
  ArithmeticArray& operator-=(const ArithmeticArray& other);

  //!
  ArithmeticArray& operator*=(const Arithmetic scalar);

  //!
  ArithmeticArray& operator*=(const ArithmeticArray& other);

  //!
  ArithmeticArray& operator/=(const Arithmetic scalar);

  //!
  ArithmeticArray& operator/=(const ArithmeticArray& other);

  //! Get the reference by index
  Arithmetic& operator[](const uint index);

  //! Get the reference by index
  const Arithmetic& operator[](const uint index) const;


  //! Clamp all elements
  void clamp(const Arithmetic min, const Arithmetic max);

  //! Divide a scalar by a array
  ArithmeticArray divideScalar(const Arithmetic scalar) const;

  //! Fill the container with specified value
  void fill(const Arithmetic value);

  //! Get an element by index.
  Arithmetic get(const uint index) const;

  //! Check whether all elements are zero
  bool isZero() const;

  //! Return the largest element
  Arithmetic max() const;

  //! Return the smallest element
  Arithmetic min() const;

  //! Compare two arrays and store maximum values
  ArithmeticArray maxElements(const ArithmeticArray& other) const;

  //! Compare two arrays and store minimum values
  ArithmeticArray minElements(const ArithmeticArray& other) const;

  //! Return the number of elements
  static constexpr uint size();

  //! Set value
  void set(const uint index, const Arithmetic value);

  //! Set values
  template <typename ...Types>
  void setElements(const Types ...values);

  //! Get sum of elements
  Arithmetic sum() const;

 private:
  //! Create a instance without initialization
  ArithmeticArray(const ArithmeticArray* array);


  //! Set value
  template <uint index, typename ...Types>
  void setElements(const Arithmetic value, const Types ...values);

  //! Set value
  template <uint index>
  void setElements(const Arithmetic value);


  Arithmetic elements_[kN];
};

//! Check whether two arrays are same
template <typename Arithmetic, uint kN>
bool operator==(const ArithmeticArray<Arithmetic, kN>& a, 
                const ArithmeticArray<Arithmetic, kN>& b);

//! Check whether two arrays are not same
template <typename Arithmetic, uint kN>
bool operator!=(const ArithmeticArray<Arithmetic, kN>& a, 
                const ArithmeticArray<Arithmetic, kN>& b);

//! Multiply each element by a scalar
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> operator*(
    const Arithmetic scalar,
    const ArithmeticArray<Arithmetic, kN>& array);

//! Divide a scalar by a array
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> operator/(
    const Arithmetic scalar,
    const ArithmeticArray<Arithmetic, kN>& array);
                                    
//! Calculate the cross product
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> cross(const ArithmeticArray<Arithmetic, kN>& a,
                                      const ArithmeticArray<Arithmetic, kN>& b);

//! Calculate the inner product
template <typename Arithmetic, uint kN>
Arithmetic dot(const ArithmeticArray<Arithmetic, kN>& a, 
               const ArithmeticArray<Arithmetic, kN>& b);

//! Compare two arrays and store maximum values
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> maxElements(
    const ArithmeticArray<Arithmetic, kN>& a,
    const ArithmeticArray<Arithmetic, kN>& b);

//! Compare two arrays and store minimum values
template <typename Arithmetic, uint kN>
ArithmeticArray<Arithmetic, kN> minElements(
    const ArithmeticArray<Arithmetic, kN>& a,
    const ArithmeticArray<Arithmetic, kN>& b);

//! Compare two arrays and store minimu values to a and maximum values to b
template <typename Arithmetic, uint kN>
void minMaxElements(ArithmeticArray<Arithmetic, kN>& a,
                    ArithmeticArray<Arithmetic, kN>& b);

} // namespace zisc

#include "arithmetic_array-inl.hpp"

#endif // _ZISC_ARITHMETIC_ARRAY_HPP_
