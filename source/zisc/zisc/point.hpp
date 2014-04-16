/*!
  \file point.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_POINT_HPP_
#define _ZISC_POINT_HPP_

// Zisc
#include "arithmetic_array.hpp"
#include "dimension.hpp"
#include "vector.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \brief N dimension point
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN>
class Point : public Dimension<Arithmetic, kN>
{
 public:
  using ArrayType = ArithmeticArray<Arithmetic, kN>;


  //! Initialize with 0
  Point();

  //! Initialize with values
  template <typename ...Types>
  Point(const Types ...values);

  //! Initialize with array
  Point(const ArrayType& array);
};

template <typename Arithmetic, uint kN>
Vector<Arithmetic, kN> operator-(const Point<Arithmetic, kN>& a,
                                 const Point<Arithmetic, kN>& b);

template <typename Arithmetic, uint kN>
Point<Arithmetic, kN> operator+(const Point<Arithmetic, kN>& point,
                                const Vector<Arithmetic, kN>& vector);

template <typename Arithmetic, uint kN>
Point<Arithmetic, kN> operator+(const Vector<Arithmetic, kN>& vector,
                                const Point<Arithmetic, kN>& point);

template <typename Arithmetic, uint kN>
Point<Arithmetic, kN> operator-(const Point<Arithmetic, kN>& point,
                                const Vector<Arithmetic, kN>& vector);

} // namespace zisc

#include "point-inl.hpp"

#endif // _ZISC_POINT_HPP_
