/*!
  \file power_weighted_light_source_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "power_weighted_light_source_sampler.hpp"
// Standard C++ library
#include <algorithm>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/cumulative_distribution_function.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "light_source_sampler.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_info.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Shape/shape.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
PowerWeightedLightSourceSampler::PowerWeightedLightSourceSampler(
    System& system,
    const World& world,
    zisc::pmr::memory_resource* work_resource) noexcept :
        info_list_{&system.dataMemoryManager()}
{
  initialize(system, world, work_resource);
}

/*!
  */
LightSourceInfo PowerWeightedLightSourceSampler::getInfo(
    const IntersectionInfo& /* info */,
    const Object* light_source) const noexcept
{
  return getInfo(light_source);
}

/*!
  \details
  No detailed.
  */
LightSourceInfo PowerWeightedLightSourceSampler::sample(
    Sampler& sampler) const noexcept
{
  return sampleInfo(sampler);
}

/*!
  \details
  No detailed.
  */
LightSourceInfo PowerWeightedLightSourceSampler::sample(
    const IntersectionInfo& /* info */,
    Sampler& sampler) const noexcept
{
  return sampleInfo(sampler);
}

/*!
  */
inline
LightSourceInfo PowerWeightedLightSourceSampler::getInfo(
    const Object* light_source) const noexcept
{
  ZISC_ASSERT(light_source != nullptr, "The light source is null.");
  ZISC_ASSERT(light_source->material().isLightSource(),
              "The object isn't light source.");
  const auto comp = [](const LightSourceInfo& lhs, const Object* rhs)
  {
    return lhs.object() < rhs;
  };
  const auto& info_list = infoList();
  auto info = std::lower_bound(info_list.begin(),
                               info_list.end(),
                               light_source,
                               comp);
  ZISC_ASSERT((info != info_list.end()) && (info->object() == light_source),
              "The light source isn't in the light source list.");
  return *info;
}

/*!
  \details
  No detailed.
  */
void PowerWeightedLightSourceSampler::initialize(
    System& system,
    const World& world,
    zisc::pmr::memory_resource* work_resource) noexcept
{
  {
    const auto& light_source_list = world.lightSourceList();
    // Calculate the total flux of the light sources
    zisc::CompensatedSummation<Float> total_flux{0.0};
    for (const auto light_source : light_source_list) {
      const auto flux = light_source->shape().surfaceArea() *
                        light_source->material().emitter().radiantExitance();
      total_flux.add(flux);
    }
    // Initialize info list
    info_list_.reserve(light_source_list.size());
    for (const auto light_source : light_source_list) {
      const auto flux = light_source->shape().surfaceArea() *
                        light_source->material().emitter().radiantExitance();
      info_list_.emplace_back(light_source, flux / total_flux.get());
    }
    // Sort the info list
    const auto comp = [](const LightSourceInfo& lhs, const LightSourceInfo& rhs)
    {
      return lhs.object() < rhs.object();
    };
    std::sort(info_list_.begin(), info_list_.end(), comp);
  }
  {
    auto data_resource = &system.dataMemoryManager();
    zisc::pmr::vector<const LightSourceInfo*> info_list{data_resource};
    zisc::pmr::vector<Float> pdf_list{data_resource};
    info_list.reserve(info_list_.size());
    pdf_list.reserve(info_list_.size());
    for (const auto& info : info_list_) {
      info_list.emplace_back(&info);
      pdf_list.emplace_back(info.weight());
    }

    light_source_cdf_ = zisc::UniqueMemoryPointer<LightSourceCdf>::make(
        data_resource,
        std::move(info_list),
        std::move(pdf_list),
        work_resource);
  }
}

/*!
  */
inline
const LightSourceInfo& PowerWeightedLightSourceSampler::sampleInfo(
    Sampler& sampler) const noexcept
{
  const auto& light_source_cdf = lightSourceCdf();
  const Float y = sampler.sample();
  const auto sampled_lihgt_source = light_source_cdf.invert(y);
  return *sampled_lihgt_source;
}


} // namespace nanairo
