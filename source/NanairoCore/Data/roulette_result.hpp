/*!
  \file roulette_result.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_ROULETTE_RESULT_HPP
#define NANAIRO_ROULETTE_RESULT_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class RouletteResult
{
 public:
  //! Create a failure result
  RouletteResult() noexcept;

  //! Create a result
  RouletteResult(const Float probability) noexcept;


  //! Check if the result is success
  explicit operator bool() const noexcept;


  //! Return the failure value
  static constexpr Float failureValue() noexcept;

  //! Check if the result is success
  bool isSuccess() const noexcept;

  //! Return the probability of the result
  Float probability() const noexcept;

  //! Set probability
  void setProbability(const Float probability) noexcept;

 private:
  Float probability_;
};

//! \} Core

} // namespace nanairo

#include "roulette_result-inl.hpp"

#endif // NANAIRO_ROULETTE_RESULT_HPP
