/*!
  \file pcg_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "pcg_sampler.hpp"
// Standard C++ library
#include <array>
// Zisc
#include "zisc/pcg_engine.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
PcgSampler::PcgSampler(const uint32 seed) noexcept : engine_{seed}
{
}

/*!
  */
Float PcgSampler::draw1D(const PathState& /* state */) noexcept
{
  const Float r = engine_.generate01Float<Float>();
  return r;
}

/*!
  */
std::array<Float, 2> PcgSampler::draw2D(const PathState& /* state */) noexcept
{
  const Float r1 = engine_.generate01Float<Float>();
  const Float r2 = engine_.generate01Float<Float>();
  return {{r1, r2}};
}

} // namespace nanairo
