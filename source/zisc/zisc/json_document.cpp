/*!
  \file json_document.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "json_document.hpp"
// Standard C++ library
#include <iomanip>
#include <limits>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
// Zisc
#include "error.hpp"
#include "json_object.hpp"
#include "json_value.hpp"
#include "string.hpp"
#include "packrat_parser/packrat_parser.hpp"
#include "packrat_parser/peg_parse_result.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
JsonObject JsonDocument::deserialize(const AbstractSyntaxNode* node) const noexcept
{
  JsonObject object;
  // Object expression
  ZISC_ASSERT(isSameId(ExpressionIdList::kObject, node->expressionId()),
              "The node isn't object.");
  ZISC_ASSERT(node->treatAsInternalNode()->numOfChildNodes() == 1,
              "The number of child nodes of a object node is invalid.");
  // Members option expression
  auto n = node->treatAsInternalNode()->getChildNode(0)->treatAsInternalNode();
  ZISC_ASSERT(isSameId(ExpressionIdList::kMembersOption, n->expressionId()),
              "The node isn't members option.");
  // Members expression
  // The object has no pair
  if (n->numOfChildNodes() == 0)
    return object;
  // The object has pairs
  n = n->getChildNode(0)->treatAsInternalNode();
  ZISC_ASSERT(isSameId(ExpressionIdList::kMembers, n->expressionId()),
              "The node isn't members.");
  // Piars expression
  const auto pairs_loop_node = n->getChildNode(0)->treatAsInternalNode();
  ZISC_ASSERT(isSameId(ExpressionIdList::kPairsLoop, pairs_loop_node->expressionId()),
              "The node isn't pairs loop.");
  for (uint i = 0; i < pairs_loop_node->numOfChildNodes(); ++i) {
    const auto pairs_node = pairs_loop_node->getChildNode(i)->treatAsInternalNode();
    ZISC_ASSERT(isSameId(ExpressionIdList::kPairs, pairs_node->expressionId()),
                "The node isn't pairs.");
    const auto pair_node = pairs_node->getChildNode(0);
    ZISC_ASSERT(isSameId(ExpressionIdList::kPair, pair_node->expressionId()),
                "The node isn't pair.");
    auto pair = toCxxPair(pair_node);
    object.append(std::move(std::get<0>(pair)), std::move(std::get<1>(pair)));
  }
  // Pair expression
  {
    auto pair_node = n->getChildNode(1)->treatAsInternalNode();
    ZISC_ASSERT(isSameId(ExpressionIdList::kPair, pair_node->expressionId()),
                "The node isn't pair.");
    auto pair = toCxxPair(pair_node);
    object.append(std::move(std::get<0>(pair)), std::move(std::get<1>(pair)));
  }
  return object;
}

/*!
  */
void JsonDocument::serialize(const JsonObject& object,
                             std::ostream& json_stream) const noexcept
{
  toJsonObject(object, 0, json_stream);
}

/*!
  */
std::unique_ptr<PackratParser> JsonDocument::makeJsonParser() const noexcept
{
  auto parser = std::make_unique<PackratParser>();
  // Initialize a parser rule 
  // Create expressions
  auto object_expression = parser->makeSequence();
  auto array_expression = parser->makeSequence();
  auto string_expression = parser->makeTerminal();
  auto number_expression = parser->makeTerminal();
  auto boolean_expression = parser->makeTerminal();
  auto null_expression = parser->makeTerminal();
  //
  auto members_option_expression = parser->makeOptional();
  auto members_expression = parser->makeSequence();
  auto pairs_loop_expression = parser->makeZeroOrMore();
  auto pairs_expression = parser->makeSequence();
  auto pair_expression = parser->makeSequence();
  auto elements_option_expression = parser->makeOptional();
  auto elements_expression = parser->makeSequence();
  auto values_loop_expression = parser->makeZeroOrMore();
  auto values_expression = parser->makeSequence();
  auto value_expression = parser->makeOrderedChoise();
  auto comma = parser->makeSkip();
  auto colon = parser->makeSkip();
  auto opening_brace = parser->makeSkip();
  auto closing_brace = parser->makeSkip();
  auto opening_bracket = parser->makeSkip();
  auto closing_bracket = parser->makeSkip();
  // ID check
  ZISC_ASSERT(isSameId(ExpressionIdList::kObject, object_expression->id()), 
              "The object expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kArray, array_expression->id()),
              "The array expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kString, string_expression->id()),
              "The string expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kNumber, number_expression->id()),
              "The number expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kBoolean, boolean_expression->id()),
              "The boolean expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kNull, null_expression->id()),
              "The null expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kMembersOption, members_option_expression->id()),
              "The members option expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kMembers, members_expression->id()),
              "The members expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kPairsLoop, pairs_loop_expression->id()),
              "The pairs loop expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kPairs, pairs_expression->id()),
              "The pairs expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kPair, pair_expression->id()),
              "The pair expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kElementsOption, elements_option_expression->id()),
              "The elements option expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kElements, elements_expression->id()),
              "The elements expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kValuesLoop, values_loop_expression->id()),
              "The values loop expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kValues, values_expression->id()),
              "The values expression has invalid id.");
  ZISC_ASSERT(isSameId(ExpressionIdList::kValue, value_expression->id()),
              "The value expression has invalid id.");
  // Initialize expressions
  parser->setAsTopExpression(object_expression);
  object_expression->setExpression(opening_brace, 
                                   members_option_expression, 
                                   closing_brace);
  members_option_expression->setExpression(members_expression);
  members_expression->setExpression(pairs_loop_expression, pair_expression);
  pairs_loop_expression->setExpression(pairs_expression);
  pairs_expression->setExpression(pair_expression, comma);
  pair_expression->setExpression(string_expression, colon, value_expression);
  array_expression->setExpression(opening_bracket,
                                  elements_option_expression,
                                  closing_bracket);
  elements_option_expression->setExpression(elements_expression);
  elements_expression->setExpression(values_loop_expression, value_expression);
  values_loop_expression->setExpression(values_expression);
  values_expression->setExpression(value_expression, comma);
  value_expression->setExpression(string_expression,
                                  number_expression,
                                  object_expression,
                                  array_expression,
                                  boolean_expression,
                                  null_expression);
  string_expression->setExpression(getStringPattern().toCString());
  number_expression->setExpression(getFloatPattern().toCString());
  boolean_expression->setExpression(getBooleanPattern().toCString());
  null_expression->setExpression(R"(null)");
  comma->setExpression(R"(,)");
  colon->setExpression(R"(:)");
  opening_brace->setExpression(R"(\{)");
  closing_brace->setExpression(R"(\})");
  opening_bracket->setExpression(R"(\[)");
  closing_bracket->setExpression(R"(\])");
  //
  for (uint i = 0; i < parser->numOfExpressions(); ++i) {
    auto expression = parser->expression(i);
    expression->setSkippingSpaces(true);
  }
  return parser;
}

/*!
  */
bool JsonDocument::isSameId(const ExpressionIdList lhs, const uint rhs) const noexcept
{
  return (cast<uint>(lhs) == rhs);
}

/*!
  */
JsonValue JsonDocument::toCxxArray(const AbstractSyntaxNode* node) const noexcept
{
  ZISC_ASSERT(isSameId(ExpressionIdList::kArray, node->expressionId()),
              "The node isn't array.");
  JsonValue value;
  std::vector<JsonValue> array;
  auto n = node->treatAsInternalNode()->getChildNode(0)->treatAsInternalNode();
  ZISC_ASSERT(isSameId(ExpressionIdList::kElementsOption, n->expressionId()),
              "The node isn't elements option.");
  if (n->numOfChildNodes() != 0) {
    n = n->getChildNode(0)->treatAsInternalNode();
    ZISC_ASSERT(isSameId(ExpressionIdList::kElements, n->expressionId()),
                "The node isn't elements.");
    // Elements
    const auto values_loop_node = n->getChildNode(0)->treatAsInternalNode();
    ZISC_ASSERT(isSameId(ExpressionIdList::kValuesLoop, 
                         values_loop_node->expressionId()),
                "The node isn't values loop.");
    const auto num_of_values = values_loop_node->numOfChildNodes() + 1;
    array.resize(num_of_values);
    for (uint i = 0; i < values_loop_node->numOfChildNodes(); ++i) {
      const auto values_node = 
          values_loop_node->getChildNode(i)->treatAsInternalNode();
      const auto value_node = values_node->getChildNode(0);
      array[i] = toCxxValue(value_node);
    }
    // Element
    {
      const auto value_node = n->getChildNode(1);
      array.back() = toCxxValue(value_node);
    }
  }
  value.setAsArray(std::move(array));
  return value;
}

/*!
  */
JsonValue JsonDocument::toCxxBoolean(const AbstractSyntaxNode* node) const noexcept
{
  ZISC_ASSERT(isSameId(ExpressionIdList::kBoolean, node->expressionId()),
              "The node isn't boolean.");
  const auto value_node = node->treatAsLeafNode();
  JsonValue value;
  ZISC_ASSERT(isBooleanString(value_node->symbol()), 
              "The symbol isn't boolean.");
  value.setAsBoolean(toBoolean(value_node->symbol()));
  return value;
}

/*!
  */
JsonValue JsonDocument::toCxxNumber(const AbstractSyntaxNode* node) const noexcept
{
  ZISC_ASSERT(isSameId(ExpressionIdList::kNumber, node->expressionId()),
              "The node isn't number.");
  const auto value_node = node->treatAsLeafNode();
  JsonValue value;
  ZISC_ASSERT(isFloatString(value_node->symbol()), 
              "The symbol isn't floating point.");
  value.setAsNumber(toFloat<double>(value_node->symbol()));
  return value;
}

/*!
  */
JsonValue JsonDocument::toCxxObject(const AbstractSyntaxNode* node) const noexcept
{
  ZISC_ASSERT(isSameId(ExpressionIdList::kObject, node->expressionId()),
              "The node isn't object.");
  JsonValue value;
  value.setAsObject(deserialize(node));
  return value;
}

/*!
  */
std::tuple<std::string, JsonValue> JsonDocument::toCxxPair(
    const AbstractSyntaxNode* pair_node) const noexcept
{
  const auto pair = pair_node->treatAsInternalNode();
  const auto key_node = pair->getChildNode(0)->treatAsLeafNode();
  ZISC_ASSERT(isSameId(ExpressionIdList::kString, key_node->expressionId()),
              "The node isn't string node.");
  const auto value_node = pair->getChildNode(1)->treatAsInternalNode();
  ZISC_ASSERT(isSameId(ExpressionIdList::kValue, value_node->expressionId()),
              "The node isn't value node.");
  return std::make_tuple(toCxxString(key_node->symbol()), toCxxValue(value_node));
}

/*!
  */
JsonValue JsonDocument::toCxxValue(
    const AbstractSyntaxNode* value_node) const noexcept
{
  JsonValue value;
  const auto v_node = value_node->treatAsInternalNode()->getChildNode(0);
  switch (v_node->expressionId()) {
   case cast<uint>(ExpressionIdList::kObject):
    value = toCxxObject(v_node);
    break;
   case cast<uint>(ExpressionIdList::kArray):
    value = toCxxArray(v_node);
    break;
   case cast<uint>(ExpressionIdList::kString):
    value.setAsString(toCxxString(v_node->treatAsLeafNode()->symbol()));
    break;
   case cast<uint>(ExpressionIdList::kNumber):
    value = toCxxNumber(v_node);
    break;
   case cast<uint>(ExpressionIdList::kBoolean):
    value = toCxxBoolean(v_node);
    break;
   case cast<uint>(ExpressionIdList::kNull):
    break;
   default:
    ZISC_ASSERT(false, "The expression id is invalid.");
    break;
  }
  return value;
}

/*!
  */
void JsonDocument::toJsonArray(const std::vector<JsonValue>& array,
                               const uint indent_level,
                               std::ostream& json_stream) const noexcept
{
  json_stream << "[";
  for (auto i = array.cbegin(); i != array.cend(); ++i) {
    toJsonValue(*i, indent_level, json_stream);
    if (std::next(i) != array.cend())
      json_stream << ", ";
  }
  json_stream << "]";
}

/*!
  */
void JsonDocument::toJsonBoolean(const bool boolean,
                                 std::ostream& json_stream) const noexcept
{
  json_stream << std::boolalpha << boolean;
}

/*!
  */
void JsonDocument::toJsonNumber(const double number,
                                std::ostream& json_stream) const noexcept
{
  json_stream << std::scientific 
              << std::setprecision(std::numeric_limits<double>::digits10 + 1)
              << number;
}

/*!
  */
void JsonDocument::toJsonObject(const JsonObject& object,
                                const uint indent_level,
                                std::ostream& json_stream) const noexcept
{
  std::string indent;
  indent.resize((indent_level + 1) * 2, ' ');
  json_stream << "{" << std::endl;
  for (auto i = object.cbegin(); i != object.cend(); ++i) {
    json_stream << indent;
    toJsonPair(*i, indent_level + 1, json_stream);
    if (std::next(i) != object.cend())
      json_stream << ",";
    json_stream << std::endl;
  }
  indent.resize(indent_level * 2, ' ');
  json_stream << indent << "}";
}

/*!
  */
void JsonDocument::toJsonPair(const JsonObject::value_type& pair,
                              const uint indent_level,
                              std::ostream& json_stream) const noexcept
{
  const auto& key = std::get<0>(pair);
  const auto& value = std::get<1>(pair);
  toJsonString(key, json_stream);
  json_stream << ": ";
  toJsonValue(value, indent_level, json_stream);
}

/*!
  */
void JsonDocument::toJsonString(const std::string& string,
                                std::ostream& json_stream) const noexcept
{
  json_stream << R"(")" << string << R"(")";
}

/*!
  */
void JsonDocument::toJsonValue(const JsonValue& value,
                               const uint indent_level,
                               std::ostream& json_stream) const noexcept
{
  switch (value.type()) {
   case JsonValue::ValueType::kArray:
    toJsonArray(value.asArray(), indent_level, json_stream);
    break;
   case JsonValue::ValueType::kBoolean:
    toJsonBoolean(value.asBoolean(), json_stream);
    break;
   case JsonValue::ValueType::kNull:
    json_stream << "null";
    break;
   case JsonValue::ValueType::kNumber:
    toJsonNumber(value.asNumber(), json_stream);
    break;
   case JsonValue::ValueType::kObject:
    toJsonObject(value.asObject(), indent_level, json_stream);
    break;
   case JsonValue::ValueType::kString:
    toJsonString(value.asString(), json_stream);
    break;
   default:
    ZISC_ASSERT(false, "The type is invalid.");
    break;
  }
}

} // namespace zisc
