/*!
  \file simple_progress_bar-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SIMPLE_PROGRESS_BAR_INL_HPP
#define NANAIRO_SIMPLE_PROGRESS_BAR_INL_HPP

// Standard C++ library
#include <ostream>
#include <string_view>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "simple_progress_bar.hpp"

namespace nanairo {

/*!
  */
inline
void SimpleProgressBar::setStream(std::ostream* s) noexcept
{
  stream_ = s;
}

/*!
  */
inline
std::ostream* SimpleProgressBar::stream() noexcept
{
  return stream_;
}

/*!
  */
inline
const std::ostream* SimpleProgressBar::stream() const noexcept
{
  return stream_;
}

/*!
  */
inline
void SimpleProgressBar::update(const double progress,
                               const std::string_view& status) noexcept
{
  ZISC_ASSERT(zisc::isInClosedBounds(progress, 0.0, 1.0),
              "The progress is out of range [0, 1]: ", progress);
  progress_ = zisc::cast<int>(100.0 * progress);
  printProgressBar(status);
}


} // namespace nanairo

#endif // NANAIRO_SIMPLE_PROGRESS_BAR_INL_HPP
