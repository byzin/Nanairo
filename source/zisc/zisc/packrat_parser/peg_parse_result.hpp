/*!
  \file peg_parse_result.hpp
  \author Sho Ikeda
  
  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PEG_PARSE_RESULT_HPP
#define ZISC_PEG_PARSE_RESULT_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "abstract_syntax_node.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
class PegParseResult
{
 public:
  //!
  PegParseResult() noexcept;

  //!
  constexpr PegParseResult(const uint expression_id) noexcept;


  //!
  static constexpr std::ptrdiff_t errorOffset() noexcept;
  
  //!
  bool errorOccured() const noexcept;

  //!
  uint expressionId() const noexcept;

  //!
  bool failed() const noexcept;

  //!
  std::ptrdiff_t offset() const noexcept;

  //!
  void setExpressionId(const uint expression_id) noexcept;

  //!
  void setOffset(const std::ptrdiff_t offset) noexcept;

  //!
  bool succeeded() const noexcept;

  //!
  static constexpr std::ptrdiff_t successOffset() noexcept;
  
  //! Return the abstract syntax tree
  std::shared_ptr<AbstractSyntaxNode>& tree() noexcept;

  //! Return the abstract syntax tree
  const std::shared_ptr<AbstractSyntaxNode>& tree() const noexcept;
  
 private:
  std::shared_ptr<AbstractSyntaxNode> tree_;
  // Error information
  std::ptrdiff_t offset_;
  uint expression_id_;
};

} // namespace zisc

#include "peg_parse_result-inl.hpp"

#endif // ZISC_PEG_PARSE_RESULT_HPP
