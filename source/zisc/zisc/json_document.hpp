/*!
  \file json_document.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_JSON_DOCUMENT_HPP
#define ZISC_JSON_DOCUMENT_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>
// Zisc
#include "json_object.hpp"
#include "json_value.hpp"
#include "packrat_parser/packrat_parser.hpp"
#include "packrat_parser/peg_expression_memo.hpp"
#include "packrat_parser/peg_parse_result.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

// Forward declaration
class AbstractSyntaxNode;

/*!
  */
class JsonDocument
{
 public:
  //!
  JsonDocument();


  //! Convert a JSON syntax tree to C++ objects
  JsonObject deserialize(const AbstractSyntaxNode* node) const noexcept;

  //! Parse a text
  std::tuple<const PegParseResult*, PegExpressionMemo> parse(
      std::istream& stream,
      std::ostream* log_stream = nullptr) const noexcept;

  //! Parse a text
  std::tuple<const PegParseResult*, PegExpressionMemo>  parse(
      const std::string& text,
      std::ostream* log_stream = nullptr) const noexcept;

  //! Parse a text
  std::tuple<const PegParseResult*, PegExpressionMemo> parse(
      std::string&& text,
      std::ostream* log_stream = nullptr) const noexcept;

  //! Convert C++ objects to a JSON text
  void serialize(const JsonObject& object, std::ostream& json_stream) const noexcept;

 private:
  enum class ExpressionIdList : uint
  {
    kObject = 0,
    kArray,
    kString,
    kNumber,
    kBoolean,
    kNull,
    kMembersOption,
    kMembers,
    kPairsLoop,
    kPairs,
    kPair,
    kElementsOption,
    kElements,
    kValuesLoop,
    kValues,
    kValue,
  };

  //! Initialize the json parser
  std::unique_ptr<PackratParser> makeJsonParser() const noexcept;

  //!
  bool isSameId(const ExpressionIdList lhs, const uint rhs) const noexcept;

  //! Convert a JSON array to a C++ array
  JsonValue toCxxArray(const AbstractSyntaxNode* value_node) const noexcept;

  //! Convert a JSON boolean to a C++ boolean
  JsonValue toCxxBoolean(const AbstractSyntaxNode* value_node) const noexcept;

  //! Convert a JSON number to a C++ number
  JsonValue toCxxNumber(const AbstractSyntaxNode* value_node) const noexcept;

  //! Convert a JSON object to a C++ object 
  JsonValue toCxxObject(const AbstractSyntaxNode* value_node) const noexcept;

  //! Convert a JSON pair to a C++ pair
  std::tuple<std::string, JsonValue> toCxxPair(
      const AbstractSyntaxNode* pair_node) const noexcept;

  //! Convert a JSON value to C++ value
  JsonValue toCxxValue(const AbstractSyntaxNode* value_node) const noexcept;

  //! Convert a C++ array to JSON array 
  void toJsonArray(const std::vector<JsonValue>& array,
                   const uint indent_level,
                   std::ostream& json_stream) const noexcept;

  //! Convert a C++ boolean to JSON boolean 
  void toJsonBoolean(const bool boolean, 
                     std::ostream& json_stream) const noexcept;

  //! Convert a C++ number to JSON number 
  void toJsonNumber(const double number, 
                    std::ostream& json_stream) const noexcept;

  //! Convert a C++ object to JSON object
  void toJsonObject(const JsonObject& value, 
                    const uint indent_level,
                    std::ostream& json_stream) const noexcept;

  //! Convert a C++ pair to JSON pair 
  void toJsonPair(const JsonObject::value_type& pair, 
                  const uint indent_level,
                  std::ostream& json_stream) const noexcept;

  //! Convert a C++ string to JSON string 
  void toJsonString(const std::string& string, 
                    std::ostream& json_stream) const noexcept;

  //! Convert a C++ value to JSON value
  void toJsonValue(const JsonValue& value, 
                   const uint indent_level,
                   std::ostream& json_stream) const noexcept;
};

} // namespace zisc

#include "json_document-inl.hpp"

#endif // ZISC_JSON_DOCUMENT_HPP
