/*!
  \file russian_roulette.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "russian_roulette.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/algorithm.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampled_spectra.hpp"
#include "sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/rendering_method_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

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
  }
}

/*!
  \details
  No detailed.
  */
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
RouletteResult RussianRoulette::playWithPath(const uint path) const noexcept
{
  return RouletteResult{path < max_path_, 1.0};
}

} // namespace nanairo
