/*!
  \file compensated_summation.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_COMPENSATED_SUMMATION_HPP
#define ZISC_COMPENSATED_SUMMATION_HPP

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
  static_assert(kIsFloat<Float>, "Float isn't floating point type.");

 public:
  //! Initialize with 0
  CompensatedSummation() noexcept;

  //! Initialize with value
  CompensatedSummation(const Float value) noexcept;


  //! Return the value
  Float get() const noexcept;

  //! Set the value and clear compensation
  void set(const Float value) noexcept;

  //! Sum value
  void add(const Float value) noexcept;

  //! Sum value
  template <typename ...Types>
  void add(const Float value, const Types... values) noexcept;

 private:
  Float sum_,
        compensation_;
};

} // namespace zisc

#include "compensated_summation-inl.hpp"

#endif // ZISC_COMPENSATED_SUMMATION_HPP
