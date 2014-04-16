/*!
  \file stopwatch-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "stopwatch.hpp"
// Standard C++ library
#include <chrono>
#include <cstdint>
// Zisc
#include "utility.hpp"

#ifndef _ZISC_STOPWATCH_INL_HPP_
#define _ZISC_STOPWATCH_INL_HPP_

namespace zisc {

/*!
  \details
  No detailed.
  */
inline
Stopwatch::Stopwatch() :
    elapsed_time_{Clock::duration::zero()},
    state_{State::Idle}
{
}

/*!
  \details
  No detailed.
  */
inline
auto Stopwatch::elapsedTime() const -> Clock::duration
{
  auto time = elapsed_time_;
  if (state_ == State::Run) {
    const auto current_time = Clock::now();
    time += current_time - start_time_;
  }
  return time;
}

/*!
  \details
  No detailed.
  */
inline
bool Stopwatch::isIdleState() const
{
  return state_ == State::Idle;
}

/*!
  \details
  No detailed.
  */
inline
bool Stopwatch::isPauseState() const
{
  return state_ == State::Pause;
}

/*!
  \details
  No detailed.
  */
inline
bool Stopwatch::isRunState() const
{
  return state_ == State::Run;
}

/*!
  \details
  No detailed.
  */
inline
auto Stopwatch::pause() -> Clock::duration
{
  if (state_ != State::Run)
    return Clock::duration::zero();

  const auto current_time = Clock::now();
  const auto time = current_time - start_time_;
  elapsed_time_ += time;
  state_ = State::Pause;
  return time;
}

/*!
  \details
  No detailed.
  */
inline
void Stopwatch::start()
{
  start_time_ = Clock::now();
  state_ = State::Run;
}

/*!
  \details
  No detailed.
  */
inline
void Stopwatch::stop()
{
  elapsed_time_ = Clock::duration::zero();
  state_ = State::Idle;
}

} // namespace zisc

#endif // _ZISC_STOPWATCH_INL_HPP_
