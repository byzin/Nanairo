/*!
  \file simple_progress_bar.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SIMPLE_PROGRESS_BAR_HPP
#define NANAIRO_SIMPLE_PROGRESS_BAR_HPP

// Standard C++ library
#include <ostream>
#include <string_view>
// Zisc
#include "zisc/utility.hpp"

namespace nanairo {

/*!
  */
class SimpleProgressBar
{
 public:
  //! Initialize a progress bar
  SimpleProgressBar() noexcept;

  //!
  ~SimpleProgressBar() noexcept;


  //! Set a stream to output the progress bar
  void setStream(std::ostream* s) noexcept;

  //! Return the current stream to output the progress bar
  std::ostream* stream() noexcept;

  //! Return the current stream to output the progress bar
  const std::ostream* stream() const noexcept;

  //! Update progress
  void update(const double progress, const std::string_view& status) noexcept;

 private:
  static constexpr int kBarInterval = 5; //!< per 5%
  static_assert(zisc::isInBounds(kBarInterval, 1, 100) && (100 % kBarInterval == 0));


  //! Initialize a progress bar
  void initialize() noexcept;

  //! Update progress bar
  void printProgressBar(const std::string_view& status) const noexcept;


  std::ostream* stream_;
  int progress_;
};

} // namespace nanairo

#include "simple_progress_bar-inl.hpp"

#endif // NANAIRO_SIMPLE_PROGRESS_BAR_HPP
