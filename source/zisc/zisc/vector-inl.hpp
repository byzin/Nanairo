/*!
  \file vector-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_VECTOR_INL_HPP
#define ZISC_VECTOR_INL_HPP

#include "vector.hpp"
// Zisc
#include "arithmetic_array.hpp"
#include "math.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN>::Vector() noexcept
{
}

/*!
  \details
  No detailed.

  \param[in] values 要素の値
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
Vector<Arithmetic, kN>::Vector(const Types ...values) noexcept :
    Dimension<Arithmetic, kN>(values...)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN>::Vector(const ArrayType& array) noexcept :
    Dimension<Arithmetic, kN>(array)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Vector<Arithmetic, kN>::operator+=(const Vector& vector) noexcept -> Vector&
{
  *this = *this + vector;
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Vector<Arithmetic, kN>::operator-=(const Vector& vector) noexcept -> Vector&
{
  *this = *this - vector;
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Vector<Arithmetic, kN>::operator*=(const Arithmetic scalar) noexcept -> Vector&
{
  *this = *this * scalar;
  return *this;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic Vector<Arithmetic, kN>::inverseNorm() const noexcept
{
  return invSqrt(squareNorm());
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic Vector<Arithmetic, kN>::norm() const noexcept
{
  return sqrt(squareNorm());
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Vector<Arithmetic, kN>::normalized() const noexcept -> Vector
{
  return *this * inverseNorm();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic Vector<Arithmetic, kN>::squareNorm() const noexcept
{
  return dot(*this, *this);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator-(const Vector<Arithmetic, kN>& vector) noexcept
{
  auto reverse_vector = vector;
  for (uint i = 0; i < kN; ++i)
    reverse_vector.set(i, -vector[i]);
  return reverse_vector;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator+(const Vector<Arithmetic, kN>& a,
                                 const Vector<Arithmetic, kN>& b) noexcept
{
  return Vector<Arithmetic, kN>{a.data() + b.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator-(const Vector<Arithmetic, kN>& a,
                                 const Vector<Arithmetic, kN>& b) noexcept
{
  return Vector<Arithmetic, kN>{a.data() - b.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator*(const Vector<Arithmetic, kN>& vector,
                                 const Arithmetic scalar) noexcept
{
  return Vector<Arithmetic, kN>{vector.data() * scalar};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator*(const Arithmetic scalar,
                                 const Vector<Arithmetic, kN>& vector) noexcept
{
  return vector * scalar;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator/(const Arithmetic scalar,
                                 const Vector<Arithmetic, kN>& vector) noexcept
{
  return Vector<Arithmetic, kN>{scalar / vector.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool operator==(const Vector<Arithmetic, kN>& a,
                const Vector<Arithmetic, kN>& b) noexcept
{
  return (a.data() == b.data());
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool operator!=(const Vector<Arithmetic, kN>& a,
                const Vector<Arithmetic, kN>& b) noexcept
{
  return !(a == b);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic dot(const Vector<Arithmetic, kN>& a,
               const Vector<Arithmetic, kN>& b) noexcept
{
  return dot(a.data(), b.data());
}

template <typename Arithmetic> inline
Vector<Arithmetic, 3> cross(const Vector<Arithmetic, 3>& a,
                            const Vector<Arithmetic, 3>& b) noexcept
{
  return Vector<Arithmetic, 3>{cross(a.data(), b.data())};
}

} // namespace zisc

#endif // ZISC_VECTOR_INL_HPP
