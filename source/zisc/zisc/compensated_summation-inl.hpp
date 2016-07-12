/*!
  \file compensated_summation-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_COMPENSATED_SUMMATION_INL_HPP
#define ZISC_COMPENSATED_SUMMATION_INL_HPP

#include "compensated_summation.hpp"
// Zisc
#include "utility.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Float> inline
CompensatedSummation<Float>::CompensatedSummation() noexcept :
    sum_{cast<Float>(0.0)},
    compensation_{cast<Float>(0.0)}
{
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
CompensatedSummation<Float>::CompensatedSummation(const Float value) noexcept :
    sum_{value},
    compensation_{cast<Float>(0.0)}
{
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float CompensatedSummation<Float>::get() const noexcept
{
  return sum_;
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
void CompensatedSummation<Float>::set(const Float value) noexcept
{
  sum_ = value;
  compensation_ = cast<Float>(0.0);
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
void CompensatedSummation<Float>::add(const Float value) noexcept
{
  {
    const volatile Float c = compensation_;
    const volatile Float tmp1 = value - c;
    const volatile Float tmp2 = sum_ + tmp1;
    compensation_ = (tmp2 - sum_) - tmp1;
    sum_ = tmp2;
  }
}

/*!
  \details
  No detailed.
  */
template <typename Float> template <typename ...Types> inline
void CompensatedSummation<Float>::add(const Float value, 
                                      const Types... values) noexcept
{
  add(value);
  add(values...);
}

} // namespace zisc

#endif // ZISC_COMPENSATED_SUMMATION_INL_HPP
