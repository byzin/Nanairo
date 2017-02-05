/*!
  \file light_source_sampler-inl.hpp
  \author zin
  */

#ifndef NANAIRO_LIGHT_SOURCE_SAMPLER_INL_HPP
#define NANAIRO_LIGHT_SOURCE_SAMPLER_INL_HPP

#include "light_source_sampler.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/cumulative_distribution_function.hpp"
// Nanairo
#include "sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Float LightSourceSampler::getPdf(const Object* light_source) const noexcept
{
  const auto flux = light_source->shape().surfaceArea() *
                    light_source->material().emitter().radiantExitance();
  return flux * inverse_total_flux_;
}

/*!
  \details
  No detailed.
  */
inline
const LightSourceReference& LightSourceSampler::sample(Sampler& sampler) const noexcept
{
  const Float y = sampler.sample();
  const auto sampled_lihgt_source = cdf_->inverseFunction(y);
  return *sampled_lihgt_source;
}

} // namespace nanairo

#endif // NANAIRO_LIGHT_SOURCE_SAMPLER_INL_HPP
