/*!
  \file sampler-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLER_INL_HPP
#define NANAIRO_SAMPLER_INL_HPP

#include "sampler.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/path_state.hpp"

namespace nanairo {

/*!
  */
inline
Sampler::Sampler(const uint32 sample) noexcept : path_state_{sample}
{
}

/*!
  */
inline
PathState& Sampler::pathState() noexcept
{
  return path_state_;
}

/*!
  */
inline
const PathState& Sampler::pathState() const noexcept
{
  return path_state_;
}

} // namespace nanairo

#endif // NANAIRO_SAMPLER_INL_HPP
