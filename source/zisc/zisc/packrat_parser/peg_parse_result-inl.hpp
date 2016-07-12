/*!
  \file peg_parse_result-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PEG_PARSE_RESULT_INL_HPP
#define ZISC_PEG_PARSE_RESULT_INL_HPP

#include "peg_parse_result.hpp"
// Standard C++ library
#include <cstddef>
#include <cstdint>
#include <memory>
// Zisc
#include "abstract_syntax_node.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
inline
PegParseResult::PegParseResult() noexcept :
    offset_{PTRDIFF_MIN},
    expression_id_{0}
{
}

/*!
  */
inline
constexpr PegParseResult::PegParseResult(const uint expression_id) noexcept :
    offset_{PTRDIFF_MIN},
    expression_id_{expression_id}
{
}

/*!
  */
inline
constexpr std::ptrdiff_t PegParseResult::errorOffset() noexcept
{
  return cast<std::ptrdiff_t>(-1);
}

/*!
  */
inline
bool PegParseResult::errorOccured() const noexcept
{
  return (offset() == errorOffset());
}

/*!
  */
inline
uint PegParseResult::expressionId() const noexcept
{
  return expression_id_;
}

/*!
  */
inline
bool PegParseResult::failed() const noexcept
{
  return !succeeded();
}

/*!
  */
inline
std::ptrdiff_t PegParseResult::offset() const noexcept
{
  return offset_;
}

/*!
  */
inline
void PegParseResult::setExpressionId(const uint expression_id) noexcept
{
  expression_id_ = expression_id;
}

/*!
  */
inline
constexpr std::ptrdiff_t PegParseResult::successOffset() noexcept
{
  return PTRDIFF_MIN;
}

/*!
  */
inline
bool PegParseResult::succeeded() const noexcept
{
  return (offset() == successOffset());
}

/*!
  */
inline
void PegParseResult::setOffset(const std::ptrdiff_t offset) noexcept
{
  offset_ = offset;
}

/*!
  */
inline
std::shared_ptr<AbstractSyntaxNode>& PegParseResult::tree() noexcept
{
  return tree_;
}

/*!
  */
inline
const std::shared_ptr<AbstractSyntaxNode>& PegParseResult::tree() const noexcept
{
  return tree_;
}

} // namespace zisc

#endif // ZISC_PEG_PARSE_RESULT_INL_HPP
