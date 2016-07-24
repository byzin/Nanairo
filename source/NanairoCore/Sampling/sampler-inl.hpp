/*!
  \file sampler-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLER_INL_HPP
#define NANAIRO_SAMPLER_INL_HPP

#include "sampler.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Sampler::Sampler(const uint32 seed) noexcept :
    sampler_{seed}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic> inline
Arithmetic Sampler::sample(const Arithmetic min, const Arithmetic max) noexcept
{ 
  return sampler_.generate(min, max);
}

/*!
  \details
  No detailed.
  */
inline
void Sampler::setSeed(const uint32 seed) noexcept
{
  sampler_.setSeed(seed);
}

} // namespace nanairo

#endif // NANAIRO_SAMPLER_INL_HPP
