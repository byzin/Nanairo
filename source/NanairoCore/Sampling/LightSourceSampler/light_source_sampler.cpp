/*!
  \file light_source_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "light_source_sampler.hpp"
// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "contribution_weighted_light_source_sampler.hpp"
#include "power_weighted_light_source_sampler.hpp"
#include "uniform_light_source_sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Setting/rendering_method_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
LightSourceSampler::LightSourceSampler() noexcept
{
  initialize();
}

/*!
  \details
  No detailed.
  */
LightSourceSampler::~LightSourceSampler() noexcept
{
}

/*!
  */
std::unique_ptr<LightSourceSampler> LightSourceSampler::makeSampler(
    const LightSourceSamplerType sampler_type,
    const World& world,
    System& /* system */) noexcept
{
  std::unique_ptr<LightSourceSampler> sampler;
  switch (sampler_type) {
   case LightSourceSamplerType::kUniform: {
    sampler = std::make_unique<UniformLightSourceSampler>(world);
    break;
   }
   case LightSourceSamplerType::kPowerWeighted: {
    sampler = std::make_unique<PowerWeightedLightSourceSampler>(world);
    break;
   }
   case LightSourceSamplerType::kContributionWeighted: {
//    const uint max_surface_split =
//        zisc::cast<uint>(sampler_settings->lightSamplerMaxSurfaceSplit());
//    const uint num_of_photons =
//        zisc::cast<uint>(sampler_settings->lightSamplerNumOfPhotons());
//    sampler = std::make_unique<ContributionWeightedLightSourceSampler>(
//        system, world, max_surface_split, num_of_photons);
    break;
   }
   default:
    break;
  }
  return sampler;
}

/*!
  \details
  No detailed.
  */
void LightSourceSampler::initialize() noexcept
{
}

} // namespace nanairo
