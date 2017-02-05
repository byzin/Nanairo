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
// Zisc
#include "zisc/split_mix64_engine.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Sampler::Sampler(const uint64 seed) noexcept :
    sampler_{}
{
  initialize(seed);
}

/*!
  \details
  No detailed.
  */
inline
Float Sampler::sample() noexcept
{
  return zisc::cast<Float>(sampler_.generate01());
}

/*!
  \details
  No detailed.
  */
inline
void Sampler::setSeed(const uint64 seed) noexcept
{
  sampler_.setSeed(seed);
}

/*!
  */
inline
void Sampler::initialize(const uint64 seed) noexcept
{
  zisc::SplitMix64Engine seeder{seed};
  const uint64 s = seeder.generate();
  setSeed(s);
}

} // namespace nanairo

#endif // NANAIRO_SAMPLER_INL_HPP
