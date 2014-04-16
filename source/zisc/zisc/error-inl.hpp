/*!
  \file error-inl.hpp"
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_ERROR_INL_HPP_
#define _ZISC_ERROR_INL_HPP_

#include "error.hpp"
// Standard C++ library
#include <cstdlib>
#include <iostream>

namespace zisc {

namespace zisc_error {

/*!
 \brief エラーメッセージを出力します
 \details
 No detailed.

 \param[in] message エラーメッセージ
 */
template <typename Type> inline
void outputErrorMessage(const Type& message)
{
  std::cerr << message;
}

/*!
 \brief エラーメッセージを出力します
 \details
 No detailed.

 \param[in] message エラーメッセージ
 \param[in] messages エラーメッセージ
 */
template <typename Type, typename ...Types> inline
void outputErrorMessage(const Type& message, const Types&... messages)
{
  outputErrorMessage(message);
  outputErrorMessage(messages...);
}

} // namespace zisc_error

/*!
  \details
  No detailed.
  */
template <typename ...Types> inline
void assertError(const bool condition, const Types& ...messages)
{
  if (!condition) {
    zisc_error::outputErrorMessage(messages..., "\n");
    std::exit(EXIT_FAILURE);
  }
}

/*!
 \details
 No detailed.

 \param[in] messages エラーメッセージ
 */
template <typename ...Types> inline
void raiseError(const Types&... messages)
{
  zisc_error::outputErrorMessage("## ERROR: ", messages..., "\n");
  std::exit(EXIT_FAILURE);
}

} // namespace zisc

#endif // _ZISC_ERROR_INL_HPP_
