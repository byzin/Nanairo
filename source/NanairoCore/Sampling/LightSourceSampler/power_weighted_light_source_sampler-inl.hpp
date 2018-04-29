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
#include "zisc/memory_resource.hpp"
#include "zisc/cumulative_distribution_function.hpp"
// Nanairo
#include "light_source_sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_info.hpp"

namespace nanairo {

/*!
  */
inline
zisc::pmr::vector<LightSourceInfo>&
PowerWeightedLightSourceSampler::infoList() noexcept
{
  return info_list_;
}

/*!
  */
inline
const zisc::pmr::vector<LightSourceInfo>&
PowerWeightedLightSourceSampler::infoList() const noexcept
{
  return info_list_;
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
auto PowerWeightedLightSourceSampler::lightSourceCdf() const noexcept
    -> const LightSourceCdf&
{
  return *light_source_cdf_;
}

} // namespace nanairo

#endif // NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_INL_HPP
