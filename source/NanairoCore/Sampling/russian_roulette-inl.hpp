/*!
  \file russian_roulette-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RUSSIAN_ROULETTE_INL_HPP
#define NANAIRO_RUSSIAN_ROULETTE_INL_HPP

#include "russian_roulette.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampled_spectra.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/roulette_result.hpp"
#include "Sampler/sampler.hpp"

namespace nanairo {

/*!
  */
inline
RouletteResult RussianRoulette::operator()(
    const SampledSpectra& weight,
    Sampler& sampler,
    const PathState& path_state) const noexcept
{
  return play(weight, sampler, path_state);
}

/*!
  */
inline
RouletteResult RussianRoulette::play(
    const SampledSpectra& weight,
    Sampler& sampler,
    const PathState& path_state) const noexcept
{
  return (type_ == RouletteType::kMaxWeight)
      ? playWithMax(weight, sampler, path_state) :
         (type_ == RouletteType::kAverageWeight)
      ? playWithAverage(weight, sampler, path_state)
      : playWithPath(path_state);
}

} // namespace nanairo

#endif // NANAIRO_RUSSIAN_ROULETTE_INL_HPP
