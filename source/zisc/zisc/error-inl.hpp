/*!
  \file error-inl.hpp"
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_ERROR_INL_HPP
#define ZISC_ERROR_INL_HPP

#include "error.hpp"
// Standard C++ library
#include <cstdlib>
#include <iostream>

namespace zisc {

namespace zisc_error {

/*!
  \details
  No detailed.
  */
template <typename Type> inline
void outputErrorMessage(const Type& message) noexcept
{
  std::cerr << message;
}

/*!
  \details
  No detailed.
  */
template <typename Type, typename ...Types> inline
void outputErrorMessage(const Type& message, const Types&... messages) noexcept
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
void assertError(const bool condition, const Types& ...messages) noexcept
{
  if (!condition) {
    raiseError(messages...);
  }
}

/*!
  \details
  No detailed.
  */
template <typename ...Types> inline
void raiseError(const Types&... messages) noexcept
{
  zisc_error::outputErrorMessage(messages..., "\n");
  std::exit(EXIT_FAILURE);
}

} // namespace zisc

#endif // ZISC_ERROR_INL_HPP
