/*!
  \file peg_expression.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PEG_EXPRESSION_HPP
#define ZISC_PEG_EXPRESSION_HPP

// Standard C++ library
#include <bitset>
#include <cstddef>
#include <memory>
#include <ostream>
#include <regex>
#include <string>
#include <vector>
// Zisc
#include "abstract_syntax_node.hpp"
#include "peg_expression_error.hpp"
#include "peg_parse_result.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

// Forward declaration
class PegExpressionMemo;

/*!
  */
class PegExpression
{
 public:
  //!
  PegExpression(const uint id) noexcept;

  //!
  virtual ~PegExpression();


  //! Return the max expression ID
  static constexpr uint creationLimits() noexcept;

  //! Evaluate the expression
  const PegParseResult& evaluate(const std::string& text,
                                 std::string::const_iterator* current,
                                 PegExpressionMemo* memo,
                                 std::ostream* log_stream = nullptr) const noexcept;

  //! Return the expression ID
  uint id() const noexcept;

  //! Return the length of text string of log
  static constexpr int logStringLength() noexcept;

  //! Return a skipping spaces flag
  bool skippingSpaces() const noexcept;

  //! Set a skipping spaces flag
  void setSkippingSpaces(const bool flag) noexcept;

 private:
  //!
  enum class ExpressionFlags : uint
  {
    kSkippingSpaces = 0,
    //! It is used for the num of flags
    kNumOfFlags
  };


  //! Evaluate the expression
  const PegParseResult& evaluateExpression(
      const std::string& text,
      std::string::const_iterator* current,
      PegExpressionMemo* memo,
      std::ostream* log_stream) const noexcept;

  //! Evaluate the expression
  virtual bool evaluateExpression(
      const std::string& text,
      std::shared_ptr<AbstractSyntaxNode>* node,
      std::string::const_iterator* current,
      PegExpressionMemo* memo,
      std::ostream* log_stream) const noexcept = 0;

  //! Get the text string for log
  std::string getLogString(const std::string& text,
                           const std::string::const_iterator& current) const noexcept;

  //! Log the start status of the evaluation of the expression
  void logEvaluationEnding(const std::string& text,
                           const std::string::const_iterator& current,
                           const PegParseResult& result,
                           std::ostream& log_stream) const noexcept;

  //! Log the end status of the evaluation of the expression
  void logEvaluationBeginning(const std::string& text,
                              const std::string::const_iterator& current,
                              std::ostream& log_stream) const noexcept;

  //! Advance a string iterator to non-space character
  void skipSpaces(const std::string& text,
                  std::string::const_iterator* iterator) const noexcept;


  static constexpr int kLogStringLength_ = 32;
  static PegExpressionError error_result_;

  uint id_;
  std::bitset<sizeof(uint) * 8> flags_;
};

/*!
  */
template <int kType>
class PegTerminalExpression : public PegExpression
{
 public:
  //!
  PegTerminalExpression(const uint id) noexcept;


  //! Set a regex as terminal expression
  void setExpression(const char* regex) noexcept;

 private:
  //! Evaluate a terminal expression
  bool evaluateExpression(
      const std::string& text,
      std::shared_ptr<AbstractSyntaxNode>* node,
      std::string::const_iterator* current,
      PegExpressionMemo* memo,
      std::ostream* log_stream) const noexcept override;


  std::regex regex_;
};

using PegTerminal = PegTerminalExpression<0>;
using PegSkip = PegTerminalExpression<1>;

/*!
  */
template <int kType>
class PegNonterminalMultipleExpression : public PegExpression
{
 public:
  //!
  PegNonterminalMultipleExpression(const uint id) noexcept;


  //! Set expressions as a nonterminal expression
  template <typename ...ExpressionPointers>
  void setExpression(const ExpressionPointers... expressions) noexcept;

 private:
  //! Evaluate a nonterminal expression
  bool evaluateExpression(
      const std::string& text,
      std::shared_ptr<AbstractSyntaxNode>* node,
      std::string::const_iterator* current,
      PegExpressionMemo* memo,
      std::ostream* log_stream) const noexcept override;

  //! Set expressions as a nonterminal expression
  template <uint index>
  void setPegExpression(const PegExpression* expression) noexcept;

  //! Set expressions as a nonterminal expression
  template <uint index, typename ...ExpressionPointers>
  void setPegExpression(const PegExpression* expression, 
                        const ExpressionPointers... expressions) noexcept;


  std::vector<const PegExpression*> expression_list_;
};

using PegSequence = PegNonterminalMultipleExpression<0>;
using PegOrderedChoise = PegNonterminalMultipleExpression<1>;

/*!
  */
template <int kType>
class PegNonterminalSingleExpression : public PegExpression
{
 public:
  //!
  PegNonterminalSingleExpression(const uint id) noexcept;


  //! Set expression as a nonterminal expression
  void setExpression(const PegExpression* expression) noexcept;

 private:
  //! Evaluate a nonterminal expression
  bool evaluateExpression(
      const std::string& text,
      std::shared_ptr<AbstractSyntaxNode>* node,
      std::string::const_iterator* current,
      PegExpressionMemo* memo,
      std::ostream* log_stream) const noexcept override;


  const PegExpression* expression_;
};

using PegZeroOrMore = PegNonterminalSingleExpression<0>;
using PegOneOrMore = PegNonterminalSingleExpression<1>;
using PegOptional = PegNonterminalSingleExpression<2>;
using PegAndPredicate = PegNonterminalSingleExpression<3>;
using PegNotPredicate = PegNonterminalSingleExpression<4>;

} // namespace zisc

#include "peg_expression-inl.hpp"

#endif // ZISC_PEG_EXPRESSION_HPP
