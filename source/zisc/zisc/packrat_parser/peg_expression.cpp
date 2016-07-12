/*!
  \file peg_expression.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "peg_expression.hpp"
// Standard C++ library
#include <memory>
#include <ostream>
#include <regex>
#include <string>
#include <utility>
// Zisc
#include "abstract_syntax_node.hpp"
#include "peg_parse_result.hpp"
#include "../error.hpp"
#include "../utility.hpp"

namespace zisc {

/*!
  */
PegExpression::~PegExpression()
{
}

/*!
  */
std::string PegExpression::getLogString(
    const std::string& text,
    const std::string::const_iterator& current) const noexcept
{
  const auto d = std::distance(current, text.cend());
  const auto last = (logStringLength() < d) 
      ? std::next(current, logStringLength())
      : text.cend();
  return std::string{current, last};
}

/*!
  */
void PegExpression::logEvaluationEnding(
    const std::string& text,
    const std::string::const_iterator& current,
    const PegParseResult& result,
    std::ostream& log_stream) const noexcept
{
  std::string state;
  if (result.succeeded())
    state = "Succeeded";
  else
    state = "Failed";
  log_stream << "[" << id() << "]" << state << ": "
             << getLogString(text, current) << std::endl;
}

/*!
  */
void PegExpression::logEvaluationBeginning(
    const std::string& text,
    const std::string::const_iterator& current,
    std::ostream& log_stream) const noexcept
{
  log_stream << "[" << id() << "]" << "Start: "
             << getLogString(text, current) << std::endl;
}

/*!
  */
template <>
bool PegTerminal::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* node,
    std::string::const_iterator* current,
    PegExpressionMemo* /* memo */,
    std::ostream* /* log_stream */) const noexcept
{
  ZISC_ASSERT(node != nullptr, "The node is NULL.");
  ZISC_ASSERT(node->get() == nullptr, "The syntax node isn't NULL.");
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  std::smatch match;
  const bool result = std::regex_search(*current, text.cend(), match, regex_);
  if (result) {
    auto next = match[0].second;
    node->reset(new AbstractSyntaxLeafNode{id(), std::string{*current, next}});
    *current = next;
  }
  return result;
}

/*!
  */
template <>
bool PegSkip::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* /* node */,
    std::string::const_iterator* current,
    PegExpressionMemo* /* memo */,
    std::ostream* /* log_stream */) const noexcept
{
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  std::smatch match;
  const bool result = std::regex_search(*current, text.cend(), match, regex_);
  if (result) {
    auto next = match[0].second;
    *current = next;
  }
  return result;
}

/*!
  */
template <>
bool PegSequence::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* node,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(node != nullptr, "The node is NULL.");
  ZISC_ASSERT(node->get() == nullptr, "The syntax node isn't NULL.");
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  uint success_count = 0;
  auto n = std::make_unique<AbstractSyntaxInternalNode>(id());
  for (const auto expression : expression_list_) {
    const auto& parse_result = expression->evaluate(text, current, memo, log_stream);
    if (parse_result.failed())
      break;
    const auto& child_node = parse_result.tree();
    if (child_node.get() != nullptr)
      n->appendChildNode(child_node.get());
    ++success_count;
  }
  const bool expression_is_success = (success_count == expression_list_.size());
  if (expression_is_success)
    node->reset(n.release());
  return expression_is_success;
}

/*!
  */
template <>
bool PegOrderedChoise::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* node,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(node != nullptr, "The node is NULL.");
  ZISC_ASSERT(node->get() == nullptr, "The syntax node isn't NULL.");
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  bool expression_is_success = false;
  auto n = std::make_unique<AbstractSyntaxInternalNode>(id());
  for (const auto expression : expression_list_) {
    const auto& parse_result = expression->evaluate(text, current, memo, log_stream);
    if (parse_result.succeeded()) {
      expression_is_success = true;
      const auto& child_node = parse_result.tree();
      n->appendChildNode(child_node.get());
      break;
    }
  }
  if (expression_is_success)
    node->reset(n.release());
  return expression_is_success;
}

/*!
  */
template <>
bool PegZeroOrMore::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* node,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(node != nullptr, "The node is NULL.");
  ZISC_ASSERT(node->get() == nullptr, "The syntax node isn't NULL.");
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  ZISC_ASSERT(expression_ != nullptr, "The expression is NULL.");
  auto n = std::make_unique<AbstractSyntaxInternalNode>(id());
  auto next = *current;
  while (true) {
    const auto& parse_result = expression_->evaluate(text, &next, memo, log_stream);
    if (parse_result.failed())
      break;
    if (parse_result.tree().get() != nullptr)
      n->appendChildNode(parse_result.tree().get());
    *current = next;
  }
  node->reset(n.release());
  return true;
}

/*!
  */
template <>
bool PegOneOrMore::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* node,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(node != nullptr, "The node is NULL.");
  ZISC_ASSERT(node->get() == nullptr, "The syntax node isn't NULL.");
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  ZISC_ASSERT(expression_ != nullptr, "The expression is NULL.");
  uint success_count = 0;
  auto n = std::make_unique<AbstractSyntaxInternalNode>(id());
  while (true) {
    const auto& parse_result = expression_->evaluate(text, current, memo, log_stream);
    if (parse_result.failed())
      break;
    if (parse_result.tree().get() != nullptr)
      n->appendChildNode(parse_result.tree().get());
    ++success_count;
  }
  const bool expression_is_success = (0 < success_count);
  if (expression_is_success)
    node->reset(n.release());
  return expression_is_success;
}

/*!
  */
template <>
bool PegOptional::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* node,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(node != nullptr, "The node is NULL.");
  ZISC_ASSERT(node->get() == nullptr, "The syntax node isn't NULL.");
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  ZISC_ASSERT(expression_ != nullptr, "The expression is NULL.");
  auto n = std::make_unique<AbstractSyntaxInternalNode>(id());
  const auto& parse_result = expression_->evaluate(text, current, memo, log_stream);
  if (parse_result.succeeded()) {
    if (parse_result.tree().get() != nullptr)
      n->appendChildNode(parse_result.tree().get());
  }
  node->reset(n.release());
  return true;
}

/*!
  */
template <>
bool PegAndPredicate::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* /* node */,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  ZISC_ASSERT(expression_ != nullptr, "The expression is NULL.");
  auto next = *current;
  const auto& parse_result = expression_->evaluate(text, &next, memo, log_stream);
  return parse_result.succeeded();
}

/*!
  */
template <>
bool PegNotPredicate::evaluateExpression(
    const std::string& text,
    std::shared_ptr<AbstractSyntaxNode>* /* node */,
    std::string::const_iterator* current,
    PegExpressionMemo* memo,
    std::ostream* log_stream) const noexcept
{
  ZISC_ASSERT(current != nullptr, "The current is NULL.");
  ZISC_ASSERT(memo != nullptr, "The memo is NULL.");
  ZISC_ASSERT(expression_ != nullptr, "The expression is NULL.");
  auto next = *current;
  const auto& parse_result = expression_->evaluate(text, &next, memo, log_stream);
  return parse_result.failed();
}

PegExpressionError PegExpression::error_result_;

} // namespace zisc
