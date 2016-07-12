/*!
  \file russian_roulette-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RUSSIAN_ROULETTE_INL_HPP
#define NANAIRO_RUSSIAN_ROULETTE_INL_HPP

#include "russian_roulette.hpp"
// Standard C++ library
#include <functional>
// Qt
#include <QString>
// Zisc
#include "zisc/error.hpp"
#include "zisc/algorithm.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampled_spectra.hpp"
#include "sampler.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

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
  \details
  No detailed.
  */
template <uint kSampleSize> inline
RouletteResult playRussianRouletteWithAverage(
    const uint /* path */,
    const SampledSpectra<kSampleSize>& weight,
    Sampler& sampler) noexcept
{
  const Float average = weight.average();
  const Float probability = zisc::min(1.0, average);
  const bool result = sampler.sample(0.0, 1.0) < probability;
  return RouletteResult{result, result ? probability : 1.0 - probability};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
RouletteResult playRussianRouletteWithMax(
    const uint /* path */,
    const SampledSpectra<kSampleSize>& weight,
    Sampler& sampler) noexcept
{
  const Float max = weight.max();
  const Float probability = zisc::min(1.0, max);
  const bool result = sampler.sample(0.0, 1.0) < probability;
  return RouletteResult{result, result ? probability : 1.0 - probability};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
RouletteResult playRussianRouletteWithPath(
    const uint max_path,
    const uint path,
    const SampledSpectra<kSampleSize>& /* weight */,
    Sampler& /* sampler */) noexcept
{
  return RouletteResult{path < max_path, 1.0};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
RussianRouletteFunction<kSampleSize> makeRussianRoulette(
    const SceneSettings& settings,
    const QString& prefix) noexcept
{
  using zisc::cast;
  using zisc::toHash32;

  RussianRouletteFunction<kSampleSize> russian_roulette;

  auto key = prefix + "/" + keyword::russianRoulette;
  const auto type = settings.stringValue(key);
  uint path_length = 0;
  if (type == keyword::roulettePathLength) {
    key = prefix + "/" + keyword::pathLength;
    path_length = cast<uint>(settings.intValue(key));
  }
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::rouletteMaxReflectance):
    russian_roulette = playRussianRouletteWithMax<kSampleSize>;
    break;
   case toHash32(keyword::rouletteAverageReflectance):
    russian_roulette = playRussianRouletteWithAverage<kSampleSize>;
    break;
   case toHash32(keyword::roulettePathLength):
    russian_roulette = [path_length](const uint path,
                                     const SampledSpectra<kSampleSize>& weight,
                                     Sampler& sampler)
    {
      return playRussianRouletteWithPath(path_length, path, weight, sampler);
    };
    break;
   default:
    zisc::raiseError("RussianRouletteError: Unsupported type is supecified.");
    break;
  }
  return russian_roulette;
}

} // namespace nanairo

#endif // NANAIRO_RUSSIAN_ROULETTE_INL_HPP
