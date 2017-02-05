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
// Qt
#include <QJsonObject>
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
#include "NanairoCore/Utility/scene_value.hpp"

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
RussianRoulette::RussianRoulette(const QJsonObject& settings) noexcept
{
  initialize(settings);
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
  return (type_ == RouletteType::kMax)     ? playWithMax(weight, sampler) :
         (type_ == RouletteType::kAverage) ? playWithAverage(weight, sampler)
                                           : playWithPath(path);
}

/*!
  \details
  No detailed.
  */
inline
void RussianRoulette::initialize(const QJsonObject& settings) noexcept
{
  using zisc::cast;
  using zisc::toHash32;

  const auto type = SceneValue::toString(settings, keyword::russianRoulette);

  if (type == keyword::roulettePathLength)
    max_path_ = SceneValue::toInt<uint>(settings, keyword::pathLength);

  switch (keyword::toHash32(type)) {
   case toHash32(keyword::rouletteAverageReflectance): {
    type_ = RouletteType::kAverage;
    break;
   }
   case toHash32(keyword::rouletteMaxReflectance): {
    type_ = RouletteType::kMax;
    break;
   }
   case toHash32(keyword::roulettePathLength): {
    type_ = RouletteType::kPath;
    break;
   }
   default: {
    zisc::raiseError("SamplingError: Unsupported type is supecified.");
    break;
   }
  }
}

/*!
  \details
  No detailed.
  */
inline
RouletteResult RussianRoulette::playWithAverage(
    const SampledSpectra& weight,
    Sampler& sampler) const noexcept
{
  const Float average = weight.average();
  const Float probability = zisc::min(1.0, average);
  const bool result = sampler.sample() < probability;
  return RouletteResult{result, result ? probability : 1.0 - probability};
}

/*!
  \details
  No detailed.
  */
inline
RouletteResult RussianRoulette::playWithMax(
    const SampledSpectra& weight,
    Sampler& sampler) const noexcept
{
  const Float max = weight.max();
  const Float probability = zisc::min(1.0, max);
  const bool result = sampler.sample() < probability;
  return RouletteResult{result, result ? probability : 1.0 - probability};
}

/*!
  \details
  No detailed.
  */
inline
RouletteResult RussianRoulette::playWithPath(const uint path) const noexcept
{
  return RouletteResult{path < max_path_, 1.0};
}

} // namespace nanairo

#endif // NANAIRO_RUSSIAN_ROULETTE_INL_HPP
