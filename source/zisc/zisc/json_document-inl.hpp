/*!
  \file json_document-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_JSON_DOCUMENT_INL_HPP
#define ZISC_JSON_DOCUMENT_INL_HPP

#include "json_document.hpp"
// Standard C++ library
#include <istream>
#include <ostream>
#include <string>
#include <tuple>
// Zisc
#include "string.hpp"
#include "packrat_parser/peg_expression_memo.hpp"
#include "packrat_parser/peg_parse_result.hpp"

namespace zisc {

/*!
  */
inline
JsonDocument::JsonDocument()
{
}

/*!
  */
inline
std::tuple<const PegParseResult*, PegExpressionMemo> JsonDocument::parse(
    std::istream& stream,
    std::ostream* log_stream) const noexcept
{
  std::string text{std::istreambuf_iterator<char>{stream},
                   std::istreambuf_iterator<char>{}};
  return parse(std::move(text), log_stream);
}

/*!
  */
inline
std::tuple<const PegParseResult*, PegExpressionMemo> JsonDocument::parse(
    const std::string& text,
    std::ostream* log_stream) const noexcept
{
  return parse(std::string{text}, log_stream);
}

/*!
  */
inline
std::tuple<const PegParseResult*, PegExpressionMemo> JsonDocument::parse(
    std::string&& text,
    std::ostream* log_stream) const noexcept
{
  replaceLineFeedCodeToSpaces(&text);
  auto parser = makeJsonParser();
  return parser->parse(text, log_stream);
}

} // namespace zisc

#endif // ZISC_JSON_DOCUMENT_INL_HPP
