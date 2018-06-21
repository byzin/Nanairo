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
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
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
zisc::UniqueMemoryPointer<LightSourceSampler> LightSourceSampler::makeSampler(
    System& system,
    const LightSourceSamplerType sampler_type,
    const World& world,
    zisc::pmr::memory_resource* work_resource) noexcept
{
  zisc::UniqueMemoryPointer<LightSourceSampler> sampler;
  auto data_resource = &system.dataMemoryManager();
  switch (sampler_type) {
   case LightSourceSamplerType::kUniform: {
    sampler = zisc::UniqueMemoryPointer<UniformLightSourceSampler>::make(
        data_resource,
        world);
    break;
   }
   case LightSourceSamplerType::kPowerWeighted: {
    sampler = zisc::UniqueMemoryPointer<PowerWeightedLightSourceSampler>::make(
        data_resource,
        system,
        world,
        work_resource);
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
