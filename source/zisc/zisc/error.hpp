/*!
  \file error.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_ERROR_HPP_
#define _ZISC_ERROR_HPP_

#ifdef ZISC_DEBUG

#define ZISC_ASSERT(condition, message) \
    zisc::assertError(condition, "Error in ", __FILE__, " at ", __LINE__, ": ", \
                      message)

#define ZISC_VALUE_ASSERT(condition, value, message) \
    zisc::assertError(condition, "Error in ", __FILE__, " at ", __LINE__, ": ", \
                      "value = ", value, ", ", message)

#else // ZISC_DEBUG

#define ZISC_ASSERT(condition, message)

#define ZISC_VALUE_ASSERT(condition, value, message)

#endif // ZISC_DEBUG

namespace zisc {

//! If condition is false, output the message and Raise an error
template <typename ...Types>
void assertError(const bool condition, const Types&... messages);

//! エラーを出力し、プログラムを終了します
template <typename ...Types>
void raiseError(const Types&... error_messages);

} // namespace zisc

#include "error-inl.hpp"

#endif // _ZISC_ERROR_HPP_
