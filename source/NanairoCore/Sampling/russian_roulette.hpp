/*!
  \file russian_roulette.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RUSSIAN_ROULETTE_HPP
#define NANAIRO_RUSSIAN_ROULETTE_HPP

// Standard C++ library
#include <functional>
// Zisc
#include "zisc/algorithm.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
template <uint> class SampledSpectra;
class Sampler;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class RouletteResult
{
 public:
  //! Create result
  RouletteResult() noexcept;

  //! Create result
  RouletteResult(const bool result, const Float probability) noexcept;


  //! Return the probability
  Float probability() const noexcept;

  //! Return the result
  bool result() const noexcept;

  //! Set probability
  void setProbability(const Float probability) noexcept;

  //! Set result
  void setResult(const bool result) noexcept;

 private:
  Float probability_;
  bool result_;
};

//! Play russian roulette
template <uint kSampleSize>
RouletteResult playRussianRouletteWithAverage(
    const uint path,
    const SampledSpectra<kSampleSize>& weight,
    Sampler& sampler) noexcept;

//! Play russian roulette
template <uint kSampleSize>
RouletteResult playRussianRouletteWithMax(
    const uint path,
    const SampledSpectra<kSampleSize>& weight,
    Sampler& sampler) noexcept;

//! Play russian roulette
template <uint kSampleSize>
RouletteResult playRussianRouletteWithPath(
    const uint max_path,
    const uint path,
    const SampledSpectra<kSampleSize>& weight,
    Sampler& sampler) noexcept;

template <uint kSampleSize>
using RussianRouletteFunction = 
    std::function<RouletteResult (const uint,
                                  const SampledSpectra<kSampleSize>&,
                                  Sampler&)>;

//! Make a russian roulette function
template <uint kSampleSize>
RussianRouletteFunction<kSampleSize> makeRussianRoulette(
    const QJsonObject& settings) noexcept;

//! \} Core

} // namespace nanairo

#include "russian_roulette-inl.hpp"

#endif // NANAIRO_RUSSIAN_ROULETTE_HPP
