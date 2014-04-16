/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_VECTOR_HPP_
#define _ZISC_VECTOR_HPP_

// Zisc
#include "arithmetic_array.hpp"
#include "dimension.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \brief Geometric vector
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN>
class Vector : public Dimension<Arithmetic, kN>
{
 public:
  using ArrayType = ArithmeticArray<Arithmetic, kN>;


  //! Initialize with 0
  Vector();

  //! Initialize with values
  template <typename ...Types>
  Vector(const Types ...values);

  //! Initialize with array
  Vector(const ArrayType& array);


  Vector& operator+=(const Vector& vector);

  Vector& operator-=(const Vector& vector);

  Vector& operator*=(const Arithmetic scalar);


  //! Return the inverse norm
  Arithmetic inverseNorm() const;

  //! Return the norm
  Arithmetic norm() const;

  //! Return a normalized vector
  Vector normalized() const;

  //! Return the square norm
  Arithmetic squareNorm() const;
};

//! Get a vector which direction is reversed.
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator-(const Vector<Arithmetic, kN>& vector);

//! Add two vectors
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator+(const Vector<Arithmetic, kN>& a,
                                 const Vector<Arithmetic, kN>& b);

//! Subtract two vectors
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator-(const Vector<Arithmetic, kN>& a,
                                 const Vector<Arithmetic, kN>& b);

//! Multiply elements of vector by a scalar
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator*(const Vector<Arithmetic, kN>& vector,
                                 const Arithmetic scalar);

//! Multiply elements of vector by a scalar
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator*(const Arithmetic scalar,
                                 const Vector<Arithmetic, kN>& vector);

//! Compare two vectors for equality.
template <typename Arithmetic, uint kN>
bool operator==(const Vector<Arithmetic, kN>& a, const Vector<Arithmetic, kN>& b);

//! Compare two vectors for not equality.
template <typename Arithmetic, uint kN>
bool operator!=(const Vector<Arithmetic, kN>& a, const Vector<Arithmetic, kN>& b);

//! Calculate dot product
template <typename Arithmetic, uint kN>
Arithmetic dot(const Vector<Arithmetic, kN>& a, const Vector<Arithmetic, kN>& b);

//! Calculate cross product
template <typename Arithmetic>
Vector<Arithmetic, 3> cross(const Vector<Arithmetic, 3>& a,
                            const Vector<Arithmetic, 3>& b);

} // namespace zisc

#include "vector-inl.hpp"

#endif // _ZISC_VECTOR_HPP_
