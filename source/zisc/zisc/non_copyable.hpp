/*!
  \file non_copyable.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_NON_COPYABLE_HPP
#define ZISC_NON_COPYABLE_HPP

namespace zisc {

/*!
 \brief A class that is super class of NonCopyable is Prohibited copy operation
 \details
 No detailed.
 */
class NonCopyable
{
 protected:
  NonCopyable() noexcept {}

 private:
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

} // namespace misc

#endif // ZISC_NON_COPYABLE_HPP
