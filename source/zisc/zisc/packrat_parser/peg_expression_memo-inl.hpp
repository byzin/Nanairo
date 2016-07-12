/*!
  \file peg_expression_memo-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PEG_EXPRESSION_MEMO_INL_HPP
#define ZISC_PEG_EXPRESSION_MEMO_INL_HPP

#include "peg_expression.hpp"
// Standard C++ library
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
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
PegExpressionMemo::PegExpressionMemo(const uint text_size,
                                     const uint num_of_expressions) noexcept
{
  initialize(text_size, num_of_expressions);
}

/*!
  */
inline
PegExpressionMemo::PegExpressionMemo(PegExpressionMemo&& other) noexcept :
    memo_table_{std::move(other.memo_table_)}
{
}

/*!
  */
inline
PegExpressionMemo& PegExpressionMemo::operator=(PegExpressionMemo&& other) noexcept
{
  memo_table_ = std::move(other.memo_table_);
  return *this;
}


/*!
  */
inline
bool PegExpressionMemo::isMemorized(const uint index) const noexcept
{
  ZISC_ASSERT(isInBounds(index, cast<uint>(0), cast<uint>(memo_table_.size())),
              "The index is out of range.");
  const auto& memo = memo_table_[index];
  return (memo.get() != nullptr);
}

/*!
  */
inline
void PegExpressionMemo::memorize(const uint index,
                                 std::unique_ptr<MemoType>&& memo) noexcept
{
  ZISC_ASSERT(isInBounds(index, cast<uint>(0), cast<uint>(memo_table_.size())),
              "The index is out of range.");
  memo_table_[index] = std::move(memo);
}

/*!
  */
inline
const std::string::const_iterator& PegExpressionMemo::nextPosition(
    const uint index) const noexcept
{
  ZISC_ASSERT(isInBounds(index, cast<uint>(0), cast<uint>(memo_table_.size())),
              "The index is out of range.");
  ZISC_ASSERT(isMemorized(index), "The memo by the index isn't memorized.");
  return std::get<1>(*memo_table_[index]);
}

/*!
  */
inline
const PegParseResult& PegExpressionMemo::parseResult(
    const uint index) const noexcept
{
  ZISC_ASSERT(isInBounds(index, cast<uint>(0), cast<uint>(memo_table_.size())),
              "The index is out of range.");
  ZISC_ASSERT(isMemorized(index), "The memo by the index isn't memorized.");
  return std::get<0>(*memo_table_[index]);
}

/*!
  */
inline
void PegExpressionMemo::initialize(const uint text_size,
                                   const uint num_of_expressions) noexcept
{
  ZISC_ASSERT(0 < text_size, "The text_size is zero.");
  ZISC_ASSERT(0 < num_of_expressions, "The num_of_expression is zero.");
  const auto table_size = text_size * num_of_expressions;
  memo_table_.resize(table_size);
}

} // namespace zisc

#endif // ZISC_PEG_EXPRESSION_MEMO_INL_HPP
