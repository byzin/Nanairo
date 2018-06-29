/*!
  \file cmj_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cmj_sampler.hpp"
// Standard C++ library
#include <array>
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/correlated_multi_jittered_engine.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/path_state.hpp"

namespace nanairo {

/*!
  */
CmjSampler::CmjSampler(const uint32 seed) noexcept :
    seed_{seed}
{
}

/*!
  */
Float CmjSampler::draw1D(const PathState& state) noexcept
{
  const auto [s, p] = calcParameters(state);
  return Engine::generate1D<Float>(s, p);
}

/*!
  */
std::array<Float, 2> CmjSampler::draw2D(const PathState& state) noexcept
{
  const auto [s, p] = calcParameters(state);
  return Engine::generate2D<Float>(s, p);
}

/*!
  */
std::tuple<uint32, uint32> CmjSampler::calcParameters(const PathState& state)
    const noexcept
{
  constexpr uint32 n = Engine::getPeriod();
  constexpr uint32 bounce = zisc::cast<uint32>(SampleDimension::kBounce);

  const uint32 offset = state.sample() / n;
  const uint32 d = (state.length() * bounce) + state.dimension();
  const uint32 s = state.sample() - (offset * n);
  ZISC_ASSERT(zisc::isInBounds(s, 0u, n), "The s is wrong: ", s);
  const uint32 p = seed_ + offset + zisc::Fnv1aHash32::hash(d);
  return std::make_tuple(s, p);
}

} // namespace nanairo
