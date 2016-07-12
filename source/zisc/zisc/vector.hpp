/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_VECTOR_HPP
#define ZISC_VECTOR_HPP

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
  Vector() noexcept;

  //! Initialize with values
  template <typename ...Types>
  Vector(const Types ...values) noexcept;

  //! Initialize with array
  Vector(const ArrayType& array) noexcept;


  //!
  Vector& operator+=(const Vector& vector) noexcept;

  //!
  Vector& operator-=(const Vector& vector) noexcept;

  //!
  Vector& operator*=(const Arithmetic scalar) noexcept;


  //! Return the inverse norm
  Arithmetic inverseNorm() const noexcept;

  //! Return the norm
  Arithmetic norm() const noexcept;

  //! Return a normalized vector
  Vector normalized() const noexcept;

  //! Return the square norm
  Arithmetic squareNorm() const noexcept;
};

//! Get a vector which direction is reversed.
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator-(const Vector<Arithmetic, kN>& vector) noexcept;

//! Add two vectors
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator+(const Vector<Arithmetic, kN>& a,
                                 const Vector<Arithmetic, kN>& b) noexcept;

//! Subtract two vectors
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator-(const Vector<Arithmetic, kN>& a,
                                 const Vector<Arithmetic, kN>& b) noexcept;

//! Multiply elements of vector by a scalar
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator*(const Vector<Arithmetic, kN>& vector,
                                 const Arithmetic scalar) noexcept;

//! Multiply elements of vector by a scalar
template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator*(const Arithmetic scalar,
                                 const Vector<Arithmetic, kN>& vector) noexcept;

//! Compare two vectors for equality.
template <typename Arithmetic, uint kN>
bool operator==(const Vector<Arithmetic, kN>& a,
                const Vector<Arithmetic, kN>& b) noexcept;

//! Compare two vectors for not equality.
template <typename Arithmetic, uint kN>
bool operator!=(const Vector<Arithmetic, kN>& a, 
                const Vector<Arithmetic, kN>& b) noexcept;

//! Calculate dot product
template <typename Arithmetic, uint kN>
Arithmetic dot(const Vector<Arithmetic, kN>& a, 
               const Vector<Arithmetic, kN>& b) noexcept;

//! Calculate cross product
template <typename Arithmetic>
Vector<Arithmetic, 3> cross(const Vector<Arithmetic, 3>& a,
                            const Vector<Arithmetic, 3>& b) noexcept;

} // namespace zisc

#include "vector-inl.hpp"

#endif // ZISC_VECTOR_HPP
