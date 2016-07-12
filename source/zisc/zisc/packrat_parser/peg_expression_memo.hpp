/*!
  \file peg_expression_memo.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PEG_EXPRESSION_MEMO_HPP
#define ZISC_PEG_EXPRESSION_MEMO_HPP

// Standard C++ library
#include <memory>
#include <string>
#include <vector>
#include <tuple>
// Zisc
#include "peg_parse_result.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

class PegExpressionMemo
{
 public:
  using MemoType = std::tuple<PegParseResult, std::string::const_iterator>;


  //!
  PegExpressionMemo(const uint text_size,
                    const uint num_of_expressions) noexcept;

  //!
  PegExpressionMemo(PegExpressionMemo&& other) noexcept;

  //!
  PegExpressionMemo& operator=(PegExpressionMemo&& other) noexcept;

  
  //!
  bool isMemorized(const uint index) const noexcept;

  //!
  void memorize(const uint index, std::unique_ptr<MemoType>&& memo) noexcept;

  //!
  const std::string::const_iterator& nextPosition(const uint index) const noexcept;
                   
  //!
  const PegParseResult& parseResult(const uint index) const noexcept;

 private:
  // Delete copy methods
  PegExpressionMemo(const PegExpressionMemo& other) = delete;
  PegExpressionMemo& operator=(const PegExpressionMemo& other) = delete;


  //!
  void initialize(const uint text_size,
                  const uint num_of_expressions) noexcept;


  std::vector<std::unique_ptr<MemoType>> memo_table_;
};

} // namespace zisc

#include "peg_expression_memo-inl.hpp"

#endif // ZISC_PEG_EXPRESSION_MEMO_HPP
