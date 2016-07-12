/*!
  \file linear_interp-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_LINEAR_INTERP_INL_HPP
#define ZISC_LINEAR_INTERP_INL_HPP

#include "linear_interp.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <tuple>
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
LinearInterp<Float>::LinearInterp() noexcept
{
}

/*!
 \details
 No detailed.
 */
template <typename Float> inline
LinearInterp<Float>::LinearInterp(LinearInterp&& other) noexcept :
    data_(std::move(other.data_))
{
}

/*!
 \details
 No detailed.
 */
template <typename Float> inline
Float LinearInterp<Float>::operator()(const Float x) const noexcept
{
  return interpolate(x);
}

/*!
 \details
 No detailed.
 */
template <typename Float> template <typename XType, typename YType> inline
void LinearInterp<Float>::add(const XType x, const YType y) noexcept
{
  static_assert(std::is_arithmetic<XType>::value,"XType isn't arithmetic type.");
  static_assert(std::is_arithmetic<YType>::value,"YType isn't arithmetic type.");

  const auto x_value = cast<Float>(x);
  const auto y_value = cast<Float>(y);
  auto position = lowerBound(x_value);
  if (exists(x_value, position)) 
    std::get<1>(*position) = y_value;
  else
    position = data_.emplace(position, x_value, y_value);
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
bool LinearInterp<Float>::exists(const Float x) const noexcept
{
  const auto position = lowerBound(x);
  return exists(x, position);
}

/*!
 \details
 No detailed.
 */
template <typename Float> inline
Float LinearInterp<Float>::interpolate(const Float x) const noexcept
{
  ZISC_ASSERT(
      isInClosedBounds(x, std::get<0>(data_.front()), std::get<0>(data_.back())),
      "The x is out of range.");

  const auto upper = lowerBound(x);
  if (exists(x, upper))
    return std::get<1>(*upper);
  const auto lower = std::prev(upper);
  const Float weight = (x - std::get<0>(*lower)) / 
                       (std::get<0>(*upper) - std::get<0>(*lower));
  return std::get<1>(*lower) + weight * (std::get<1>(*upper) - std::get<1>(*lower));
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
bool LinearInterp<Float>::exists(const Float x, 
                                 const ConstIterator& position) const noexcept
{
  return (position != data_.end()) && (std::get<0>(*position) == x);
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
auto LinearInterp<Float>::lowerBound(const Float x) noexcept -> Iterator
{
  const auto compare = [](const Pair& a, const Float& b)
  {
    return std::get<0>(a) < b;
  };
  return std::lower_bound(data_.begin(), data_.end(), x, compare);
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
auto LinearInterp<Float>::lowerBound(const Float x) const noexcept -> ConstIterator
{
  const auto compare = [](const Pair& a, const Float& b)
  {
    return std::get<0>(a) < b;
  };
  return std::lower_bound(data_.begin(), data_.end(), x, compare);
}

} // namespace zisc

#endif // ZISC_LINEAR_INTERP_INL_HPP
