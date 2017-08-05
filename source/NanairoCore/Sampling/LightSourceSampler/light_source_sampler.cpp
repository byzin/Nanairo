/*!
  \file light_source_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "light_source_sampler.hpp"
// Standard C++ library
#include <vector>
// Nanairo
#include "power_weighted_light_source_sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Data/object.hpp"

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
    const LightSourceSamplerType type,
    const World& world) noexcept
{
  LightSourceSampler* sampler = nullptr;
  switch (type) {
   case LightSourceSamplerType::kUniform: {
//    sampler = new UniformLightSourceSampler{world};
    break;
   }
   case LightSourceSamplerType::kPowerWeighted: {
    sampler = new PowerWeightedLightSourceSampler{world};
    break;
   }
   case LightSourceSamplerType::kContributionWeighted: {
//    sampler = new ContributionWeightedLightSourceSampler{world};
    break;
   }
   default:
    break;
  }
  return std::unique_ptr<LightSourceSampler>{sampler};
}

/*!
  */
const LightSourceReference& LightSourceSampler::getReference(
    const std::vector<LightSourceReference>& reference_list,
    const Object* light_source) const noexcept
{
  const LightSourceReference* light_reference = &invalidReference();
  if (0 < reference_list.size()) {
    const auto comp = [](const LightSourceReference& lhs, const Object* rhs)
    {
      return lhs.object() < rhs;
    };
    auto ite = std::lower_bound(reference_list.begin(),
                                reference_list.end(),
                                light_source,
                                comp);
    if ((ite != reference_list.end()) && (ite->object() == light_source))
      light_reference = &(*ite);
  }
  return *light_reference;
}

/*!
  \details
  No detailed.
  */
void LightSourceSampler::initialize() noexcept
{
}

} // namespace nanairo
