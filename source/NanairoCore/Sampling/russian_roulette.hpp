/*!
  \file russian_roulette.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RUSSIAN_ROULETTE_HPP
#define NANAIRO_RUSSIAN_ROULETTE_HPP

// Zisc
#include "zisc/fnv_1a_hash_engine.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/roulette_result.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class PathState;
class SampledSpectra;
class Sampler;

//! \addtogroup Core
//! \{

enum class RouletteType : uint32
{
  kMaxWeight                   = zisc::Fnv1aHash32::hash("MaxWeight"),
  kAverageWeight               = zisc::Fnv1aHash32::hash("AverageWeight"),
  kPathLength                  = zisc::Fnv1aHash32::hash("PathLength")
};

/*!
  */
class RussianRoulette
{
 public:
  //! Initialize
  RussianRoulette(const SettingNodeBase* settings) noexcept;

  //! Play russian roulette
  RouletteResult operator()(const SampledSpectra& weight,
                            Sampler& sampler,
                            const PathState& path_state) const noexcept;


  //! Play russian roulette
  RouletteResult play(const SampledSpectra& weight,
                      Sampler& sampler,
                      const PathState& path_state) const noexcept;

 private:
  //! Initialize
  void initialize(const SettingNodeBase* settings) noexcept;

  //! Play russian roulette
  RouletteResult playWithAverage(const SampledSpectra& weight,
                                 Sampler& sampler,
                                 const PathState& path_state) const noexcept;

  //! Play russian roulette
  RouletteResult playWithMax(const SampledSpectra& weight,
                             Sampler& sampler,
                             const PathState& path_state) const noexcept;

  //! Play russian roulette
  RouletteResult playWithPath(const PathState& path_state) const noexcept;


  RouletteType type_;
  uint max_path_;
};

//! \} Core

} // namespace nanairo

#include "russian_roulette-inl.hpp"

#endif // NANAIRO_RUSSIAN_ROULETTE_HPP
