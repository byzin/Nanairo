/*!
  \file russian_roulette-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
#include "sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/roulette_result.hpp"

namespace nanairo {

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
