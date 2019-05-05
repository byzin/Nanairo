/*!
  \file simple_progress_bar.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string_view>
// Nanairo
#include "simple_progress_bar.hpp"

namespace nanairo {

/*!
  */
SimpleProgressBar::SimpleProgressBar() noexcept :
    stream_{&std::cout},
    progress_{-1}
{
  initialize();
}

/*!
  */
SimpleProgressBar::~SimpleProgressBar() noexcept
{
  if (stream_ != nullptr) {
    (*stream_) << std::endl;
  }
}

/*!
  */
void SimpleProgressBar::initialize() noexcept
{
  using namespace std::literals;
  printProgressBar(""sv);
}

/*!
  */
void SimpleProgressBar::printProgressBar(const std::string_view& status)
    const noexcept
{
  if (stream_ != nullptr) {
    constexpr std::size_t bar_length = 100 / kBarInterval;
    std::array<char, bar_length + 6> bar;
    bar.fill(' ');

    // Left side of bar
    bar[0] = '%';
    bar[2] = '[';

    // Bar body
    const std::size_t level = (0 <= progress_) ? progress_ / kBarInterval : 0;
    for (std::size_t i = 0; i < level; ++i)
      bar[3 + i] = '=';
    bar[3 + level] = (0 <= progress_) ? '>' : ' ';

    // Right side of bar
    bar[bar.size() - 3] = ']';
    bar[bar.size() - 1] = '\0';

    (*stream_) << '\r' << std::setw(3) << progress_ << bar.data() << status
               << std::flush;
  }
}

} // namespace nanairo
