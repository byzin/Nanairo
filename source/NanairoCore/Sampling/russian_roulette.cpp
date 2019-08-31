/*!
  \file russian_roulette.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "russian_roulette.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampled_spectra.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Setting/rendering_method_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "Sampler/sampler.hpp"

namespace nanairo {

/*!
  */
RussianRoulette::RussianRoulette(const SettingNodeBase* settings) noexcept
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
void RussianRoulette::initialize(const SettingNodeBase* settings) noexcept
{
  const auto method_settings = castNode<RenderingMethodSettingNode>(settings);

  {
    type_ = method_settings->rouletteType();
    max_path_ = method_settings->roulettePathLength();
  }
}

/*!
  \details
  No detailed.
  */
RouletteResult RussianRoulette::playWithAverage(
    const SampledSpectra& weight,
    Sampler& sampler,
    const PathState& path_state) const noexcept
{
  const Float average = weight.average();
  const Float probability = zisc::min(1.0, average);
  const bool result = sampler.draw1D(path_state) < probability;
  return (result) ? RouletteResult{probability} : RouletteResult{};
}

/*!
  \details
  No detailed.
  */
RouletteResult RussianRoulette::playWithMax(
    const SampledSpectra& weight,
    Sampler& sampler,
    const PathState& path_state) const noexcept
{
  bool result = path_state.length() < 4;
  Float probability = 0.0;
  if (result) {
    const Float max = weight.max();
    probability = zisc::min(1.0, max);
    result = sampler.draw1D(path_state) < probability;
  }
  return (result) ? RouletteResult{probability} : RouletteResult{};
}

/*!
  \details
  No detailed.
  */
RouletteResult RussianRoulette::playWithPath(
    const PathState& path_state) const noexcept
{
  return (path_state.length() < max_path_) ? RouletteResult{1.0} : RouletteResult{};
}

} // namespace nanairo
