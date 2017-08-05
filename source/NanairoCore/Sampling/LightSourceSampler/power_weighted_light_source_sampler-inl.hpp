/*!
  \file power_weighted_light_source_sampler-inl.hpp
  \author zin
  */

#ifndef NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_INL_HPP
#define NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_INL_HPP

#include "power_weighted_light_source_sampler.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/cumulative_distribution_function.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  */
inline
auto PowerWeightedLightSourceSampler::lightSourceCdf() const noexcept
    -> const LightSourceCdf&
{
  return *light_source_cdf_;
}

/*!
  */
inline
auto PowerWeightedLightSourceSampler::lightSourceCdf() noexcept ->
    LightSourceCdf&
{
  return *light_source_cdf_;
}

/*!
  */
inline
const std::vector<LightSourceReference>&
PowerWeightedLightSourceSampler::referenceList() const noexcept
{
  return reference_list_;
}

/*!
  */
inline
std::vector<LightSourceReference>&
PowerWeightedLightSourceSampler::referenceList() noexcept
{
  return reference_list_;
}

/*!
  */
inline
const LightSourceReference& PowerWeightedLightSourceSampler::sample(
    Sampler& sampler) const noexcept
{
  const auto& light_source_cdf = lightSourceCdf();

  const Float y = sampler.sample();
  const auto sampled_lihgt_source = light_source_cdf.inverseFunction(y);

  return *sampled_lihgt_source;
}

} // namespace nanairo

#endif // NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_INL_HPP
