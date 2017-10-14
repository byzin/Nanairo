/*!
  \file roulette_result-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_ROULETTE_RESULT_INL_HPP
#define NANAIRO_ROULETTE_RESULT_INL_HPP

#include "roulette_result.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
RouletteResult::RouletteResult() noexcept :
    probability_{failureValue()}
{
}

/*!
  \details
  No detailed.
  */
inline
RouletteResult::RouletteResult(const Float probability) noexcept :
    probability_{probability}
{
  ZISC_ASSERT(zisc::isInClosedBounds(probability, 0.0, 1.0),
              "The probability is out of range [0, 1]: ", probability);
}

/*!
  */
inline
RouletteResult::operator bool() const noexcept
{
  return isSuccess();
}

/*!
  */
inline
constexpr Float RouletteResult::failureValue() noexcept
{
  constexpr Float failure = 0.0;
  return failure;
}

/*!
  */
inline
bool RouletteResult::isSuccess() const noexcept
{
  const bool result = probability() != failureValue();
  return result;
}

/*!
  \details
  No detailed.
  */
inline
Float RouletteResult::probability() const noexcept
{
  return probability_;
}

/*!
  \details
  No detailed.
  */
inline
void RouletteResult::setProbability(const Float probability) noexcept
{
  ZISC_ASSERT(zisc::isInClosedBounds(probability, 0.0, 1.0),
              "The probability is out of range [0, 1]: ", probability);
  probability_ = probability;
}

} // namespace nanairo

#endif // NANAIRO_ROULETTE_RESULT_INL_HPP
