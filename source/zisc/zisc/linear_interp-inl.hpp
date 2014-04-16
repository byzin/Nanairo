/*!
  \file linear_interp-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_LINEAR_INTERP_INL_HPP_
#define _ZISC_LINEAR_INTERP_INL_HPP_

#include "linear_interp.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "error.hpp"
#include "utility.hpp"

namespace zisc {

/*!
 \details
 No detailed.
 */
template <typename Float> inline
LinearInterp<Float>::LinearInterp(LinearInterp&& interpolation) :
    data_(std::move(interpolation.data_))
{
}

/*!
 \details
 No detailed.
 */
template <typename Float> inline
Float LinearInterp<Float>::operator()(const Float x) const
{
  return interpolate(x);
}

/*!
 \details
 No detailed.
 */
template <typename Float> template <typename XType, typename YType>
void LinearInterp<Float>::add(const XType x, const YType y)
{
  static_assert(std::is_arithmetic<XType>::value,"## XType must be arithmetic type.");
  static_assert(std::is_arithmetic<YType>::value,"## YType must be arithmetic type.");

  const auto position = lowerBound(cast<Float>(x));
  if (isSamePosition(position, cast<Float>(x)))
    raiseError("LinstInterpError: x (", x, ") is duplicated.");
  data_.emplace(position, cast<Float>(x), cast<Float>(y));
}

/*!
  \details
  No detailed.
  */
template <typename Float> template <typename Type> inline
bool LinearInterp<Float>::exists(const Type x) const
{
  static_assert(std::is_arithmetic<Type>::value, "## Type must be arithmetic type.");
  const auto position = lowerBound(cast<Float>(x));
  return isSamePosition(position, cast<Float>(x));
}

/*!
 \details
 No detailed.
 */
template <typename Float> inline
Float LinearInterp<Float>::interpolate(const Float x) const
{
  if ((x < data_.front().first) || (data_.back().first < x))
    raiseError("LinearInterpError: x (", x, ") is out of range.");

  const auto upper = lowerBound(x);
  if (upper->first == x)
    return upper->second;
  const auto lower = std::prev(upper);
  const Float weight = (x - lower->first) / (upper->first - lower->first);
  return lower->second + weight * (upper->second - lower->second);
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
auto LinearInterp<Float>::lowerBound(const Float x) -> Iterator
{
  const auto compare = [](const Pair& a, const Float& b)
  {
    return a.first < b;
  };
  return std::lower_bound(data_.begin(), data_.end(), x, compare);
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
auto LinearInterp<Float>::lowerBound(const Float x) const -> ConstIterator
{
  const auto compare = [](const Pair& a, const Float& b)
  {
    return a.first < b;
  };
  return std::lower_bound(data_.begin(), data_.end(), x, compare);
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
bool LinearInterp<Float>::isSamePosition(const ConstIterator& a, const Float b) const
{
  return (!(a == data_.end()) && !(cast<Float>(b) < a->first));
}

} // namespace zisc

#endif // _ZISC_LINEAR_INTERP_INL_HPP_
