/*!
  \file sampler-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SAMPLER_INL_HPP_
#define _NANAIRO_SAMPLER_INL_HPP_

#include "sampler.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Sampler::Sampler(const uint32 seed) :
    sampler_{seed}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic> inline
Arithmetic Sampler::sample(const Arithmetic min, const Arithmetic max)
{
  return sampler_.generate(min, max);
}

/*!
  \details
  No detailed.
  */
inline
void Sampler::setSeed(const uint32 seed)
{
  sampler_.setSeed(seed);
}

} // namespace nanairo

#endif // _NANAIRO_SAMPLER_INL_HPP_
