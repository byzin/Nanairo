/*!
  \file vector-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_VECTOR_INL_HPP_
#define _ZISC_VECTOR_INL_HPP_

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
Vector<Arithmetic, kN>::Vector()
{
}

/*!
  \details
  No detailed.

  \param[in] values 要素の値
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
Vector<Arithmetic, kN>::Vector(const Types ...values) :
    Dimension<Arithmetic, kN>(values...)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN>::Vector(const ArrayType& array) :
    Dimension<Arithmetic, kN>(array)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Vector<Arithmetic, kN>::operator+=(const Vector& vector) -> Vector&
{
  return *this = *this + vector;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Vector<Arithmetic, kN>::operator-=(const Vector& vector) -> Vector&
{
  return *this = *this - vector;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Vector<Arithmetic, kN>::operator*=(const Arithmetic scalar) -> Vector&
{
  return *this = *this * scalar;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic Vector<Arithmetic, kN>::inverseNorm() const
{
  return invSqrt(squareNorm());
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic Vector<Arithmetic, kN>::norm() const
{
  return zisc::sqrt(squareNorm());
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Vector<Arithmetic, kN>::normalized() const -> Vector
{
  return *this * inverseNorm();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic Vector<Arithmetic, kN>::squareNorm() const
{
  return dot(*this, *this);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator-(const Vector<Arithmetic, kN>& vector)
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
                                 const Vector<Arithmetic, kN>& b)
{
  return Vector<Arithmetic, kN>{a.data() + b.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator-(const Vector<Arithmetic, kN>& a,
                                 const Vector<Arithmetic, kN>& b)
{
  return Vector<Arithmetic, kN>{a.data() - b.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator*(const Vector<Arithmetic, kN>& vector,
                                 const Arithmetic scalar)
{
  return Vector<Arithmetic, kN>{vector.data() * scalar};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator*(const Arithmetic scalar,
                                 const Vector<Arithmetic, kN>& vector)
{
  return vector * scalar;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator/(const Arithmetic scalar,
                                 const Vector<Arithmetic, kN>& vector)
{
  return Vector<Arithmetic, kN>{scalar / vector.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool operator==(const Vector<Arithmetic, kN>& a,const Vector<Arithmetic, kN>& b)
{
  return a.data() == b.data();
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
bool operator!=(const Vector<Arithmetic, kN>& a,const Vector<Arithmetic, kN>& b)
{
  return !(a == b);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic dot(const Vector<Arithmetic, kN>& a, const Vector<Arithmetic, kN>& b)
{
  return dot(a.data(), b.data());
}

template <typename Arithmetic> inline
Vector<Arithmetic, 3> cross(const Vector<Arithmetic, 3>& a,
                            const Vector<Arithmetic, 3>& b)
{
  return Vector<Arithmetic, 3>{cross(a.data(), b.data())};
}

} // namespace zisc

#endif // _ZISC_VECTOR_INL_HPP_
