/*!
  \file power_weighted_light_source_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "power_weighted_light_source_sampler.hpp"
// Standard C++ library
#include <algorithm>
#include <utility>
#include <vector>
// Zisc
#include "zisc/compensated_summation.hpp"
#include "zisc/cumulative_distribution_function.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "light_source_sampler.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Shape/shape.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
PowerWeightedLightSourceSampler::PowerWeightedLightSourceSampler(
    const World& world) noexcept
{
  initialize(world);
}

/*!
  */
const LightSourceReference& PowerWeightedLightSourceSampler::getReference(
    const IntersectionInfo& /* info */,
    const Object* light_source) const noexcept
{
  return LightSourceSampler::getReference(referenceList(), light_source);
}

/*!
  \details
  No detailed.
  */
const LightSourceReference& PowerWeightedLightSourceSampler::sample(
    const IntersectionInfo& /* info */,
    Sampler& sampler) const noexcept
{
  return sample(sampler);
}


/*!
  \details
  No detailed.
  */
void PowerWeightedLightSourceSampler::initialize(
    const World& world) noexcept
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
    // Initialize references
    reference_list_.reserve(light_source_list.size());
    for (const auto light_source : light_source_list) {
      const auto flux = light_source->shape().surfaceArea() *
                        light_source->material().emitter().radiantExitance();
      reference_list_.emplace_back(light_source, flux / total_flux.get());
    }
    // Sort reference list
    const auto comp = [](const LightSourceReference& lhs,
                         const LightSourceReference& rhs)
    {
      return lhs.object() < rhs.object();
    };
    std::sort(reference_list_.begin(), reference_list_.end(), comp);
  }

  {
    std::vector<const LightSourceReference*> reference_list;
    std::vector<Float> pdf_list;

    reference_list.reserve(reference_list_.size());
    pdf_list.reserve(reference_list_.size());
    for (const auto& light_source : reference_list_) {
      reference_list.emplace_back(&light_source);
      pdf_list.emplace_back(light_source.weight());
    }

    light_source_cdf_ = std::make_unique<LightSourceCdf>(std::move(reference_list),
                                                         std::move(pdf_list));
  }
}

} // namespace nanairo
