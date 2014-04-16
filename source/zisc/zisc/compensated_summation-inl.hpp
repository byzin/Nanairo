/*!
  \file compensated_summation-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_COMPENSATED_SUMMATION_INL_HPP_
#define _ZISC_COMPENSATED_SUMMATION_INL_HPP_

#include "compensated_summation.hpp"
// Zisc
#include "utility.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Float> inline
CompensatedSummation<Float>::CompensatedSummation() :
    sum_{cast<Float>(0.0)},
    compensation_{cast<Float>(0.0)}
{
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
CompensatedSummation<Float>::CompensatedSummation(const Float value) :
    sum_{value},
    compensation_{cast<Float>(0.0)}
{
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float CompensatedSummation<Float>::get() const
{
  return sum_;
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
void CompensatedSummation<Float>::set(const Float value)
{
  sum_ = value;
  compensation_ = cast<Float>(0.0);
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
void CompensatedSummation<Float>::add(const Float value)
{
  const volatile Float c = compensation_;
  const volatile Float tmp1 = value - c;
  const volatile Float tmp2 = sum_ + tmp1;
  compensation_ = (tmp2 - sum_) - tmp1;
  sum_ = tmp2;
}

/*!
  \details
  No detailed.
  */
template <typename Float> template <typename ...Types> inline
void CompensatedSummation<Float>::add(const Float value, const Types... values)
{
  add(value);
  add(values...);
}

} // namespace zisc

#endif // _ZISC_COMPENSATED_SUMMATION_INL_HPP_
