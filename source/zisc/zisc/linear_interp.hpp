/*!
  \file linear_interp.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_LINEAR_INTERP_HPP_
#define _ZISC_LINEAR_INTERP_HPP_

// Standard C++ library
#include <list>
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
  static_assert(std::is_floating_point<Float>::value, "## Float must be float type.");

  using Pair = std::pair<Float, Float>;
  using Iterator = typename std::list<Pair>::iterator;
  using ConstIterator = typename std::list<Pair>::const_iterator;

 public:
  //! Create empty instance
  LinearInterp() {}

  //! Move instance
  LinearInterp(LinearInterp&& interpolation);


  //! Interpolate 
  Float operator()(const Float x) const;


  //! Add data
  template <typename XType, typename YType>
  void add(const XType x, const YType y);

  //! Check whether the data at x exists
  template <typename Type>
  bool exists(const Type x) const;

  //! Interpolate 
  Float interpolate(const Float x) const;

 private:
  Iterator lowerBound(const Float x);

  ConstIterator lowerBound(const Float x) const;

  bool isSamePosition(const ConstIterator& a, const Float b) const;


  std::list<Pair> data_;
};

} // namespace zisc

#include "linear_interp-inl.hpp"

#endif // _ZISC_LINEAR_INTERP_HPP_
