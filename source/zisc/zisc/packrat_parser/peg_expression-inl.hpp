/*!
  \file peg_expression-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PEG_EXPRESSION_INL_HPP
#define ZISC_PEG_EXPRESSION_INL_HPP

#include "peg_expression.hpp"
// Standard C++ library
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <ostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>
// Zisc
#include "abstract_syntax_node.hpp"
#include "peg_expression_error.hpp"
#include "peg_expression_memo.hpp"
#include "peg_parse_result.hpp"
#include "../error.hpp"
#include "../utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
inline
PegExpression::PegExpression(const uint id) noexcept :
    id_{id}
{
}

/*!
  */
inline
constexpr uint PegExpression::creationLimits() noexcept
{
  return std::numeric_limits<uint>::max() - PegExpressionError::numOfErrors();
}

/*!
  */
inline
const PegParseResult& PegExpression::evaluate(
    const std::string& text,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  // Error check
  // Check the position of the current iterator
  const auto position = std::distance(text.cbegin(), *current);
  constexpr auto lower = cast<decltype(position)>(0);
  const auto upper = cast<decltype(position)>(text.size());
  if (!isInBounds(position, lower, upper)) {
    return error_result_.error(PegExpressionError::Error::kIndexIsOutOfRange);
  }
  if (log_stream != nullptr)
    logEvaluationBeginning(text, *current, *log_stream);
  const auto& result = evaluateExpression(text, current, memo, log_stream);
  if (log_stream != nullptr)
    logEvaluationEnding(text, *current, result, *log_stream);
  return result;
}

/*!
  */
inline
constexpr int PegExpression::logStringLength() noexcept
{
  return kLogStringLength_;
}

/*!
  */
inline
uint PegExpression::id() const noexcept
{
  return id_;
}

/*!
  */
inline
bool PegExpression::skippingSpaces() const noexcept
{
  constexpr auto index = cast<uint>(ExpressionFlags::kSkippingSpaces);
  return flags_[index];
}

/*!
  */
inline
void PegExpression::setSkippingSpaces(const bool flag) noexcept
{
  constexpr auto index = cast<uint>(ExpressionFlags::kSkippingSpaces);
  flags_.set(index, flag);
}

/*!
  */
inline
const PegParseResult& PegExpression::evaluateExpression(
    const std::string& text,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  // Evaluate a expression
  const auto position = std::distance(text.cbegin(), *current);
  const uint index = cast<uint>(text.size()) * id() + cast<uint>(position);
  if (memo->isMemorized(index)) {
    *current = memo->nextPosition(index);
  }
  else {
    // Create a result
    auto result = std::make_unique<PegExpressionMemo::MemoType>();
    auto& parse_result = std::get<0>(*result);
    parse_result.setExpressionId(id());
    // Evaluate the expression
    auto next = *current;
    if (skippingSpaces())
      skipSpaces(text, &next);
    if (evaluateExpression(text, &(parse_result.tree()), &next, memo, log_stream)) {
      *current = next;
    }
    else {
      const auto offset = std::distance(text.cbegin(), *current);
      parse_result.setOffset(offset);
    }
    std::get<1>(*result) = *current;
    memo->memorize(index, std::move(result));
  }
  return memo->parseResult(index);
}

/*!
  */
inline
void PegExpression::skipSpaces(
    const std::string& text,
    std::string::const_iterator* iterator) const noexcept
{
  ZISC_ASSERT(iterator != nullptr, "The iterator is NULL.");
  auto& it = *iterator;
  while ((it != text.cend()) && (*it == ' '))
    ++it;
}

/*!
  */
template <int kType> inline
PegTerminalExpression<kType>::PegTerminalExpression(const uint id) noexcept :
    PegExpression(id)
{
}

/*!
  */
template <int kType> inline
void PegTerminalExpression<kType>::setExpression(const char* regex) noexcept
{
  ZISC_ASSERT(regex != nullptr, "The regex is NULL.");
  const auto pattern = (regex[0] != '^') 
      ? '^' + std::string{regex} 
      : std::string{regex};
  constexpr auto flag = std::regex_constants::nosubs |
                        std::regex_constants::optimize |
                        std::regex_constants::ECMAScript;
  regex_ = std::regex{pattern, flag};
}

/*!
  */
template <int kType> inline
PegNonterminalMultipleExpression<kType>::PegNonterminalMultipleExpression(
    const uint id) noexcept :
        PegExpression(id)
{
}

/*!
  */
template <int kType> template <typename ...ExpressionPointers> inline
void PegNonterminalMultipleExpression<kType>::setExpression(
    const ExpressionPointers... expressions) noexcept
{
  constexpr auto expression_length = sizeof...(ExpressionPointers);
  expression_list_.resize(expression_length);
  setPegExpression<0>(expressions...);
}

/*!
  */
template <int kType> template <uint index> inline
void PegNonterminalMultipleExpression<kType>::setPegExpression(
    const PegExpression* expression) noexcept
{
  expression_list_[index] = expression;
}

/*!
  */
template <int kType> template <uint index, typename ...ExpressionPointers> inline
void PegNonterminalMultipleExpression<kType>::setPegExpression(
    const PegExpression* expression, 
    const ExpressionPointers... expressions) noexcept
{
  setPegExpression<index>(expression);
  setPegExpression<index + 1>(expressions...);
}

/*!
  */
template <int kType> inline
PegNonterminalSingleExpression<kType>::PegNonterminalSingleExpression(
    const uint id) noexcept :
        PegExpression(id)
{
}

/*!
  */
template <int kType> inline
void PegNonterminalSingleExpression<kType>::setExpression(
    const PegExpression* expression) noexcept
{
  expression_ = expression;
}

} // namespace zisc

#endif // ZISC_PEG_EXPRESSION_INL_HPP
