/*!
  \file packrat_parser.hpp
  \author Sho Ikeda
  
  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PACKRAT_PARSER_HPP
#define ZISC_PACKRAT_PARSER_HPP

// Standard C++ library
#include <list>
#include <memory>
#include <ostream>
#include <tuple>
// Zisc
#include "peg_parse_result.hpp"
#include "peg_expression.hpp"
#include "peg_expression_memo.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
class PackratParser
{
 public:
  //!
  PegExpression* expression(const uint expression_id) noexcept;

  //!
  const PegExpression* expression(const uint expression_id) const noexcept;

  //! Make a and-predicate expression
  PegAndPredicate* makeAndPredicate() noexcept;

  //! Make a not-predicate expression
  PegNotPredicate* makeNotPredicate() noexcept;

  //! Make a one-ore-more expression
  PegOneOrMore* makeOneOrMore() noexcept;

  //! Make a optional expression
  PegOptional* makeOptional() noexcept;

  //! Make a ordered choise
  PegOrderedChoise* makeOrderedChoise() noexcept;

  //! Make a skip expression
  PegSkip* makeSkip() noexcept;

  //! Make a sequence expression
  PegSequence* makeSequence() noexcept;

  //! Make a terminal expression
  PegTerminal* makeTerminal() noexcept;

  //! Make a zero-or-more expression
  PegZeroOrMore* makeZeroOrMore() noexcept;

  //! Return the number of expression
  uint numOfExpressions() const noexcept;

  //!
  std::tuple<const PegParseResult*, PegExpressionMemo> parse(
      const std::string& text,
      std::ostream* log_stream = nullptr) const noexcept;

  //!
  const PegExpression* setAsTopExpression(const PegExpression* expression) noexcept;

  //!
  const PegExpression* setAsTopExpression(const uint expression_id) noexcept;

  //!
  uint topExpressionId() const noexcept;

 private:
  //! Make a expression
  template <typename ExpressionType>
  ExpressionType* makeExpression() noexcept;


  std::list<std::unique_ptr<PegExpression>> expression_list_;
  uint top_expression_id_;
};

} // namespace zisc

#include "packrat_parser-inl.hpp"

#endif // ZISC_PACKRAT_PARSER_HPP
