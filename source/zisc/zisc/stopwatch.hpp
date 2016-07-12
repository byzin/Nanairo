/*!
  \file stopwatch.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_STOPWATCH_HPP
#define ZISC_STOPWATCH_HPP

// Standard C++ library
#include <chrono>
#include <cstdint>
// Zisc
#include "non_copyable.hpp"

namespace zisc {

/*!
 \brief Measure elapsed processor time
 \details
 No detailed.
 */
class Stopwatch : public NonCopyable
{
 public:
  //! Clock type
  using Clock = std::chrono::high_resolution_clock;


  //! Initialize
  Stopwatch() noexcept;


  //! Return the total elapsed time
  Clock::duration elapsedTime() const noexcept;

  //! Check if stopwatch is idle state
  bool isIdleState() const noexcept;

  //! Check if stopwatch is pause state
  bool isPauseState() const noexcept;

  //! Check if stopwatch is run state
  bool isRunState() const noexcept;

  //! Pause time measurement and return the elapsed time
  Clock::duration pause() noexcept;

  //! Start to measure the time
  void start() noexcept;

  //! Stop time measurement
  void stop() noexcept;

 private:
  enum class State : Clock::rep
  {
    Idle,
    Run,
    Pause
  };


  Clock::time_point start_time_;
  Clock::duration elapsed_time_;
  State state_;
};

} // namespace zisc

#include "stopwatch-inl.hpp"

#endif // ZISC_STOPWATCH_HPP
