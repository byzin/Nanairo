/*!
  \file point-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_POINT_INL_HPP_
#define _ZISC_POINT_INL_HPP_

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
Point<Arithmetic, kN>::Point()
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
Point<Arithmetic, kN>::Point(const Types ...values) :
    Dimension<Arithmetic, kN>(values...)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN>::Point(const ArrayType& array) :
    Dimension<Arithmetic, kN>(array)
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Vector<Arithmetic, kN> operator-(const Point<Arithmetic, kN>& a,
                                 const Point<Arithmetic, kN>& b)
{
  return Vector<Arithmetic, kN>{a.data() - b.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN> operator+(const Point<Arithmetic, kN>& point,
                                const Vector<Arithmetic, kN>& vector)
{
  return Point<Arithmetic, kN>{point.data() + vector.data()};
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN> operator+(const Vector<Arithmetic, kN>& vector,
                                const Point<Arithmetic, kN>& point)
{
  return point + vector;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Point<Arithmetic, kN> operator-(const Point<Arithmetic, kN>& point,
                                const Vector<Arithmetic, kN>& vector)
{
  return Point<Arithmetic, kN>{point.data() - vector.data()};
}

} // namespace zisc

#endif // _ZISC_POINT_INL_HPP_
