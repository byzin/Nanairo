/*!
  \file linear_interp.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_LINEAR_INTERP_HPP
#define ZISC_LINEAR_INTERP_HPP

// Standard C++ library
#include <list>
#include <tuple>
#include <type_traits>
#include <utility>
// Zisc
#include "non_copyable.hpp"

namespace zisc {

/*!
 \brief Linear interpolation class
 \details.
 No detailed.

 \todo ensure to be sorted.
 */
template <typename Float>
class LinearInterp : public NonCopyable
{
  static_assert(std::is_floating_point<Float>::value, 
                "Float isn't floating point type.");

  using Pair = std::tuple<Float, Float>;
  using Iterator = typename std::list<Pair>::iterator;
  using ConstIterator = typename std::list<Pair>::const_iterator;

 public:
  //! Create empty instance
  LinearInterp() noexcept;

  //! Move instance
  LinearInterp(LinearInterp&& other) noexcept;


  //! Interpolate 
  Float operator()(const Float x) const noexcept;


  //! Add data
  template <typename XType, typename YType>
  void add(const XType x, const YType y) noexcept;

  //! Check whether the data at x exists
  bool exists(const Float x) const noexcept;

  //! Interpolate 
  Float interpolate(const Float x) const noexcept;

 private:
  //! Check whether the data at x exists
  bool exists(const Float x, const ConstIterator& position) const noexcept;

  //!
  Iterator lowerBound(const Float x) noexcept;

  //!
  ConstIterator lowerBound(const Float x) const noexcept;


  std::list<Pair> data_;
};

} // namespace zisc

#include "linear_interp-inl.hpp"

#endif // ZISC_LINEAR_INTERP_HPP
