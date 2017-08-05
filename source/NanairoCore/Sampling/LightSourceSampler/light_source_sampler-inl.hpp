/*!
  \file light_source_sampler-inl.hpp
  \author zin
  */

#ifndef NANAIRO_LIGHT_SOURCE_SAMPLER_INL_HPP
#define NANAIRO_LIGHT_SOURCE_SAMPLER_INL_HPP

#include "light_source_sampler.hpp"
// Standard C++ library
#include <algorithm>
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Data/object.hpp"

namespace nanairo {

/*!
  */
inline
const LightSourceReference& LightSourceSampler::invalidReference() const noexcept
{
  return invalid_reference_;
}

} // namespace nanairo

#endif // NANAIRO_LIGHT_SOURCE_SAMPLER_INL_HPP
