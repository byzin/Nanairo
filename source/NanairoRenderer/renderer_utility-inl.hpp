/*!
  \file renderer_utility-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RENDERER_UTILITY_INL_HPP_
#define _NANAIRO_RENDERER_UTILITY_INL_HPP_

// Standard C++ library
#include <chrono>
// Qt
#include <QString>
// Zisc
#include "zisc/stopwatch.hpp"
#include "zisc/utility.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
QString getTimeString(const zisc::Stopwatch::Clock::duration time)
{
  using zisc::cast;
  using std::chrono::duration_cast;
  using Hours = std::chrono::hours;
  using Minutes = std::chrono::minutes;
  using Seconds = std::chrono::seconds;
  using Millis = std::chrono::milliseconds;

  const auto hours = duration_cast<Hours>(time);
  auto rest_time = time - hours;
  const auto minutes = duration_cast<Minutes>(rest_time);
  rest_time = rest_time - minutes;
  const auto seconds = duration_cast<Seconds>(rest_time);
  rest_time = rest_time - seconds;
  const auto millis = duration_cast<Millis>(rest_time);

  const auto time_string = QStringLiteral("%1 h %2 m %3.%4 s");
  constexpr auto zero = QChar('0');
  return (((time_string.arg(hours.count(), 2, 10, zero))
                       .arg(minutes.count(), 2, 10, zero))
                       .arg(seconds.count(), 2, 10, zero))
                       .arg(millis.count(), 3, 10, zero);
}

} // namespace nanairo

#endif // _NANAIRO_RENDERER_UTILITY_INL_HPP_
