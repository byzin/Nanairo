/*!
  \file xoshiro_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "xoshiro_sampler.hpp"
// Standard C++ library
#include <array>
// Zisc
#include "zisc/xoshiro_2star_engine.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
XoshiroSampler::XoshiroSampler(const uint32 seed) noexcept : engine_{seed}
{
}

/*!
  */
Float XoshiroSampler::draw1D(const PathState& /* state */) noexcept
{
  const Float r = engine_.generate01Float<Float>();
  return r;
}

/*!
  */
std::array<Float, 2> XoshiroSampler::draw2D(const PathState& /* state */) noexcept
{
  const Float r1 = engine_.generate01Float<Float>();
  const Float r2 = engine_.generate01Float<Float>();
  return {{r1, r2}};
}

} // namespace nanairo
