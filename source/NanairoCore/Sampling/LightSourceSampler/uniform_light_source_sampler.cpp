/*!
  \file uniform_light_source_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "uniform_light_source_sampler.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "light_source_sampler.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_info.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
UniformLightSourceSampler::UniformLightSourceSampler(const World& world) noexcept
{
  initialize(world);
}

/*!
  */
LightSourceInfo UniformLightSourceSampler::getInfo(
    const IntersectionInfo& /* info */,
    const Object* light_source) const noexcept
{
  return getInfo(light_source);
}

/*!
  */
LightSourceInfo UniformLightSourceSampler::sample(
    Sampler& sampler,
    const PathState& path_state) const noexcept
{
  return sampleInfo(sampler, path_state);
}

/*!
  \details
  No detailed.
  */
LightSourceInfo UniformLightSourceSampler::sample(
    const IntersectionInfo& /* info */,
    Sampler& sampler,
    const PathState& path_state) const noexcept
{
  return sampleInfo(sampler, path_state);
}

/*!
  */
inline
LightSourceInfo UniformLightSourceSampler::getInfo(const Object* light_source)
    const noexcept
{
  ZISC_ASSERT(light_source != nullptr, "The light source is null.");
  ZISC_ASSERT(light_source->material().isLightSource(), 
              "The object isn't light source.");
  return LightSourceInfo{light_source, weightPerLight()};
}

/*!
  \details
  No detailed.
  */
void UniformLightSourceSampler::initialize(const World& world) noexcept
{
  const auto& light_source_list = world.lightSourceList();
  ZISC_ASSERT(0 < light_source_list.size(), "The scene has no light source.");
  light_source_list_ = &light_source_list;
  weight_per_light_ = zisc::invert(zisc::cast<Float>(light_source_list.size()));
}

/*!
  */
inline
const zisc::pmr::vector<const Object*>& UniformLightSourceSampler::lightSourceList()
    const noexcept
{
  return *light_source_list_;
}

/*!
  */
inline
LightSourceInfo UniformLightSourceSampler::sampleInfo(
    Sampler& sampler,
    const PathState& path_state) const noexcept
{
  const auto& light_source_list = lightSourceList();
  const Float k = zisc::cast<Float>(light_source_list.size());
  const Float r = sampler.draw1D(path_state);
  const uint light_number = zisc::cast<uint>(k * r);
  return LightSourceInfo{light_source_list[light_number], weightPerLight()};
}

/*!
  */
inline
Float UniformLightSourceSampler::weightPerLight() const noexcept
{
  return weight_per_light_;
}

} // namespace nanairo
