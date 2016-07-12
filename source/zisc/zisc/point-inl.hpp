/*!
  \file point-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_POINT_INL_HPP
#define ZISC_POINT_INL_HPP

#include "point.hpp"
// Zisc
#include "arithmetic_array.hpp"
#include "dimension.hpp"
#include "vector.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN>::Point() noexcept
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
Point<Arithmetic, kN>::Point(const Types ...values) noexcept :
    Dimension<Arithmetic, kN>(values...)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN>::Point(const ArrayType& array) noexcept :
    Dimension<Arithmetic, kN>(array)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator-(const Point<Arithmetic, kN>& a,
                                 const Point<Arithmetic, kN>& b) noexcept
{
  return Vector<Arithmetic, kN>{a.data() - b.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN> operator+(const Point<Arithmetic, kN>& point,
                                const Vector<Arithmetic, kN>& vector) noexcept
{
  return Point<Arithmetic, kN>{point.data() + vector.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN> operator+(const Vector<Arithmetic, kN>& vector,
                                const Point<Arithmetic, kN>& point) noexcept
{
  return point + vector;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN> operator-(const Point<Arithmetic, kN>& point,
                                const Vector<Arithmetic, kN>& vector) noexcept
{
  return Point<Arithmetic, kN>{point.data() - vector.data()};
}

} // namespace zisc

#endif // ZISC_POINT_INL_HPP
