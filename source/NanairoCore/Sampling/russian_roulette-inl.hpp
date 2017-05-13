/*!
  \file russian_roulette-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RUSSIAN_ROULETTE_INL_HPP
#define NANAIRO_RUSSIAN_ROULETTE_INL_HPP

#include "russian_roulette.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/algorithm.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampled_spectra.hpp"
#include "sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
RouletteResult::RouletteResult() noexcept :
    probability_{1.0},
    result_{true}
{
}

/*!
  \details
  No detailed.
  */
inline
RouletteResult::RouletteResult(const bool result, const Float probability) noexcept :
    probability_{probability},
    result_{result}
{
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
bool RouletteResult::result() const noexcept
{
  return result_;
}

/*!
  \details
  No detailed.
  */
inline
void RouletteResult::setProbability(const Float probability) noexcept
{
  probability_ = probability;
}

/*!
  \details
  No detailed.
  */
inline
void RouletteResult::setResult(const bool result) noexcept
{
  result_ = result;
}

/*!
  */
inline
RouletteResult RussianRoulette::operator()(const uint path,
                                           const SampledSpectra& weight,
                                           Sampler& sampler) const noexcept
{
  return play(path, weight, sampler);
}

/*!
  */
inline
RouletteResult RussianRoulette::play(const uint path,
                                     const SampledSpectra& weight,
                                     Sampler& sampler) const noexcept
{
  return (type_ == RouletteType::kMaxWeight)     ? playWithMax(weight, sampler) :
         (type_ == RouletteType::kAverageWeight) ? playWithAverage(weight, sampler)
                                                 : playWithPath(path);
}

} // namespace nanairo

#endif // NANAIRO_RUSSIAN_ROULETTE_INL_HPP
