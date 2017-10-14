/*!
  \file uniform_light_source_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "uniform_light_source_sampler.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "light_source_sampler.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_info.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

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
LightSourceInfo UniformLightSourceSampler::sample(Sampler& sampler) const noexcept
{
  return sampleInfo(sampler);
}

/*!
  \details
  No detailed.
  */
LightSourceInfo UniformLightSourceSampler::sample(
    const IntersectionInfo& /* info */,
    Sampler& sampler) const noexcept
{
  return sampleInfo(sampler);
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
const std::vector<const Object*>& UniformLightSourceSampler::lightSourceList()
    const noexcept
{
  return *light_source_list_;
}

/*!
  */
inline
LightSourceInfo UniformLightSourceSampler::sampleInfo(Sampler& sampler)
    const noexcept
{
  const auto& light_source_list = lightSourceList();
  const Float k = zisc::cast<Float>(light_source_list.size());
  const uint light_number = zisc::cast<uint>(k * sampler.sample());
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
