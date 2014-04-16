/*!
  \file cumulative_distribution_function-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_CUMULATIVE_DISTRIBUTION_FUNCTION_INL_HPP_
#define _ZISC_CUMULATIVE_DISTRIBUTION_FUNCTION_INL_HPP_

#include "cumulative_distribution_function.hpp"
// Standard C++ library
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <tuple>
#include <vector>
#include <utility>
// Zisc
#include "algorithm.hpp"
#include "compensated_summation.hpp"
#include "error.hpp"
#include "type_traits.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename XType, typename PdfType> 
template <typename XIterator, typename PdfIterator> inline
CumulativeDistributionFunction<XType, PdfType>::CumulativeDistributionFunction(
    XIterator x_begin,
    XIterator x_end,
    PdfIterator pdf_begin,
    PdfIterator pdf_end)
{
  initialize(x_begin, x_end, pdf_begin, pdf_end);
}

/*!
  \details
  No detailed.
  */
template <typename XType, typename PdfType> inline
CumulativeDistributionFunction<XType, PdfType>::CumulativeDistributionFunction(
    CumulativeDistributionFunction&& other) :
  x_list_{std::move(other.x_list_)},
  y_list_{std::move(other.y_list_)}
{
}

/*!
  \details
  No detailed.
  */
template <typename XType, typename PdfType> inline
auto CumulativeDistributionFunction<XType, PdfType>::operator=(
    CumulativeDistributionFunction&& other)
        -> CumulativeDistributionFunction&
{
  x_list_ = std::move(other.x_list_);
  y_list_ = std::move(other.y_list_);
}

/*!
  \details
  No detailed.
  */
template <typename XType, typename PdfType> inline
const XType& CumulativeDistributionFunction<XType, PdfType>::inverseFunction(
    const PdfType& y) const
{
  ZISC_ASSERT((0.0 <= y) && (y <= 1.0), "y must be [0, 1].");
  const auto position = searchBinaryTree(y_list_.begin(), y_list_.end(), y);
  const auto index = std::distance(y_list_.begin(), position);
  return x_list_[index];
}

/*!
  \details
  No detailed.
  */
template <typename XType, typename PdfType> inline
auto CumulativeDistributionFunction<XType, PdfType>::xList() const
    -> const std::vector<XType>& 
{
  return x_list_;
}

/*!
  \details
  No detailed.
  */
template <typename XType, typename PdfType> inline
auto CumulativeDistributionFunction<XType, PdfType>::yList() const
    -> const std::vector<PdfType>& 
{
  return y_list_;
}

/*!
  \details
  No detailed.
  */
template <typename XType, typename PdfType> 
template <typename XIterator, typename PdfIterator> inline
bool CumulativeDistributionFunction<XType, PdfType>::initialize(
    XIterator x_begin,
    XIterator x_end,
    PdfIterator pdf_begin,
    PdfIterator pdf_end)
{
  // Type check
  using XType_ = typename std::iterator_traits<XIterator>::value_type;
  using PdfType_ = typename std::iterator_traits<PdfIterator>::value_type;
  static_assert(std::is_same<XType, XType_>::value, 
                "XIterator must be XType iterator.");
  static_assert(kIsFloat<PdfType_>, 
                "PdfIterator must be float iterator.");

  // Size check
  const auto size = std::distance(x_begin, x_end);
  const auto pdf_size = std::distance(pdf_begin, pdf_end);
  ZISC_ASSERT(size == pdf_size, "x and pdf array size must be same.");

  // Make a binary tree
  using CdfType = std::tuple<XType*, PdfType>;
  std::vector<CdfType> cdf_list;
  cdf_list.resize(size);
  CompensatedSummation<PdfType> sum{0.0};
  auto x_iterator = x_begin;
  auto pdf_iterator = pdf_begin;
  for (uint index = 0; index < size; ++index) {
    cdf_list[index] = std::make_tuple(&(*x_iterator), sum.get());
    sum.add(*pdf_iterator);
    ++x_iterator;
    ++pdf_iterator;
  }
  ZISC_ASSERT(((1.0 - 0.0000001) < sum.get()) && (sum.get() < (1.0 + 0.0000001)),
              "Sum of pdf must be 1.0.");
  toBinaryTree(cdf_list.begin(), cdf_list.end());

  // 
  x_list_.reserve(size);
  y_list_.reserve(size);
  for (auto& element : cdf_list) {
    x_list_.emplace_back(std::move(*std::get<0>(element)));
    y_list_.emplace_back(std::get<1>(element));
  }

  return (size == pdf_size);
}

} // namespace zisc

#endif // _ZISC_CUMULATIVE_DISTRIBUTION_FUNCTION_INL_HPP_
