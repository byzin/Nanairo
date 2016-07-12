/*!
  \file packrat_parser-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PACKRAT_PARSER_INL_HPP
#define ZISC_PACKRAT_PARSER_INL_HPP

#include "packrat_parser.hpp"
// Stadnard C++ library
#include <iterator>
#include <list>
#include <memory>
#include <tuple>
#include <utility>
// Zisc
#include "peg_expression.hpp"
#include "peg_expression_memo.hpp"
#include "peg_parse_result.hpp"
#include "../error.hpp"
#include "../utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
inline
PegExpression* PackratParser::expression(const uint expression_id) noexcept
{
  ZISC_ASSERT(isInBounds(expression_id, cast<uint>(0), numOfExpressions()),
              "The expression_id is out of range.");
  auto expression = std::next(expression_list_.begin(), expression_id);
  return expression->get();
}

/*!
  */
inline
const PegExpression* PackratParser::expression(
    const uint expression_id) const noexcept
{
  ZISC_ASSERT(isInBounds(expression_id, cast<uint>(0), numOfExpressions()),
              "The expression_id is out of range.");
  const auto expression = std::next(expression_list_.cbegin(), expression_id);
  return expression->get();
}

/*!
  */
inline
PegAndPredicate* PackratParser::makeAndPredicate() noexcept
{
  return makeExpression<PegAndPredicate>();
}

/*!
  */
inline
PegNotPredicate* PackratParser::makeNotPredicate() noexcept
{
  return makeExpression<PegNotPredicate>();
}

/*!
  */
inline
PegOneOrMore* PackratParser::makeOneOrMore() noexcept
{
  return makeExpression<PegOneOrMore>();
}

/*!
  */
inline
PegOptional* PackratParser::makeOptional() noexcept
{
  return makeExpression<PegOptional>();
}

/*!
  */
inline
PegOrderedChoise* PackratParser::makeOrderedChoise() noexcept
{
  return makeExpression<PegOrderedChoise>();
}

/*!
  */
inline
PegSkip* PackratParser::makeSkip() noexcept
{
  return makeExpression<PegSkip>();
}

/*!
  */
inline
PegSequence* PackratParser::makeSequence() noexcept
{
  return makeExpression<PegSequence>();
}

/*!
  */
inline
PegTerminal* PackratParser::makeTerminal() noexcept
{
  return makeExpression<PegTerminal>();
}

/*!
  */
inline
PegZeroOrMore* PackratParser::makeZeroOrMore() noexcept
{
  return makeExpression<PegZeroOrMore>();
}

/*!
  */
inline
uint PackratParser::numOfExpressions() const noexcept
{
  return cast<uint>(expression_list_.size());
}

/*!
  */
inline
std::tuple<const PegParseResult*, PegExpressionMemo> PackratParser::parse(
    const std::string& text,
    std::ostream* log_stream) const noexcept
{
  const auto e = expression(topExpressionId());
  auto begin = text.cbegin();
  PegExpressionMemo memo{static_cast<uint>(text.size()), numOfExpressions()};
  const auto& result = e->evaluate(text, &begin, &memo, log_stream);
  return std::make_tuple(&result, std::move(memo));
}

/*!
  */
inline
const PegExpression* PackratParser::setAsTopExpression(
    const PegExpression* expression) noexcept
{
  ZISC_ASSERT(expression != nullptr, "The expression is NULL.");
  const uint expression_id = expression->id();
  ZISC_ASSERT(isInBounds(expression_id, cast<uint>(0), numOfExpressions()),
              "The expression_id is out of range.");
  top_expression_id_ = expression_id;
  return expression;
}

/*!
  */
inline
const PegExpression* PackratParser::setAsTopExpression(
    const uint expression_id) noexcept
{
  ZISC_ASSERT(isInBounds(expression_id, cast<uint>(0), numOfExpressions()),
              "The expression_id is out of range.");
  auto expression = std::next(expression_list_.begin(), expression_id);
  top_expression_id_ = expression_id;
  return expression->get();
}

/*
  */
inline
uint PackratParser::topExpressionId() const noexcept
{
  return top_expression_id_;
}

/*!
  */
template <typename ExpressionType> inline
ExpressionType* PackratParser::makeExpression() noexcept
{
  const uint expression_id = numOfExpressions();
  ZISC_ASSERT(expression_id < (PegExpression::creationLimits() - 1),
              "The num of expressions exceeds the limit.");
  auto expression = new ExpressionType{expression_id};
  expression_list_.emplace_back(expression);
  return expression;
}

} // namespace zisc

#endif // ZISC_PACKRAT_PARSER_INL_HPP
