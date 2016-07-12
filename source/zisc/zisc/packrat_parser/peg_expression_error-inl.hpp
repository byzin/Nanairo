/*!
  \file peg_expression_error-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PEG_EXPRESSION_ERROR_INL_HPP
#define ZISC_PEG_EXPRESSION_ERROR_INL_HPP

#include "peg_expression.hpp"
// Standard C++ library
#include <limits>
#include <utility>
// Zisc
#include "peg_parse_result.hpp"
#include "../error.hpp"
#include "../utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
inline
PegExpressionError::PegExpressionError() noexcept
{
  initialize();
}

/*!
  */
inline
const PegParseResult& PegExpressionError::error(const Error error) const noexcept
{
  return error_result_list_[cast<uint>(error)];
}

/*!
  */
inline
constexpr uint PegExpressionError::errorId(const Error error) noexcept
{
  return std::numeric_limits<uint>::max() - cast<uint>(error);
}

/*!
  */
inline
constexpr uint PegExpressionError::numOfErrors() noexcept
{
  return kNumOfErrors_;
}

/*!
  */
inline
void PegExpressionError::initialize() noexcept
{
  for (uint i = 0; i < numOfErrors(); ++i) {
    error_result_list_[i] = PegParseResult{errorId(cast<Error>(i))};
    error_result_list_[i].setOffset(PegParseResult::errorOffset());
  }
}

} // namespace zisc

#endif // ZISC_PEG_EXPRESSION_ERROR_INL_HPP
