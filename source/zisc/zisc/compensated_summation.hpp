/*!
  \file compensated_summation.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_COMPENSATED_SUMMATION_HPP_
#define _ZISC_COMPENSATED_SUMMATION_HPP_

// Standard C++ library
#include <type_traits>
// Zisc
#include "type_traits.hpp"

namespace zisc {

/*!
  \details
  The details of this algorithm are explained in the following URL
  http://en.wikipedia.org/wiki/Kahan_summation_algorithm
  */
template <typename Float>
class CompensatedSummation
{
  static_assert(kIsFloat<Float>, "Float must be floating point type.");

 public:
  //! Initialize with 0
  CompensatedSummation();

  //! Initialize with value
  CompensatedSummation(const Float value);


  //! Return the value
  Float get() const;

  //! Set the value and clear compensation
  void set(const Float value);

  //! Sum value
  void add(const Float value);

  //! Sum value
  template <typename ...Types>
  void add(const Float value, const Types... values);

 private:
  Float sum_,
        compensation_;
};

} // namespace zisc

#include "compensated_summation-inl.hpp"

#endif // _ZISC_COMPENSATED_SUMMATION_HPP_
