/*!
  \file peg_expression_error.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PEG_EXPRESSION_ERROR_HPP
#define ZISC_PEG_EXPRESSION_ERROR_HPP

// Standard C++ library
#include <array>
// Zisc
#include "peg_parse_result.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
class PegExpressionError
{
 public:
  enum class Error : uint
  {
    kIndexIsOutOfRange = 0,
    kIdLimitsExceeded,
    //! It is used for the num of error elements
    kNumOfErrors,
  };


  //!
  PegExpressionError() noexcept;


  //! Return the error by the index
  const PegParseResult& error(const Error error) const noexcept;

  //! Return the error expression ID
  static constexpr uint errorId(const Error error) noexcept;

  //! Return the num of errors
  static constexpr uint numOfErrors() noexcept;

 private:
  //!
  void initialize() noexcept;


  static constexpr uint kNumOfErrors_ = static_cast<uint>(Error::kNumOfErrors);

  std::array<PegParseResult, kNumOfErrors_> error_result_list_;
};

} // namespace zisc

#include "peg_expression_error-inl.hpp"

#endif // ZISC_PEG_EXPRESSION_ERROR_HPP
