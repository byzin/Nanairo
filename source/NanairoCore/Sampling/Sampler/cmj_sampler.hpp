/*!
  \file cmj_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CMJ_SAMPLER_HPP
#define NANAIRO_CMJ_SAMPLER_HPP

// Standard C++ library
#include <array>
#include <tuple>
// Zisc
#include "zisc/correlated_multi_jittered_engine.hpp"
// Nanairo
#include "sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class PathState;

//! \addtogroup Core
//! \{

class CmjSampler : public Sampler
{
 public:
  //! Initialize a sampler
  CmjSampler(const uint32 seed) noexcept;


  //! Sample a [0, 1) float random number
  Float draw1D(const PathState& state) noexcept override;

  //! Sample a [0, 1) float random number
  std::array<Float, 2> draw2D(const PathState& state) noexcept override;

 private:
  using Engine = zisc::CmjN256;


  //! Calculate CMJ parameters
  std::tuple<uint32, uint32> calcParameters(const PathState& state) const noexcept;


  uint32 seed_;
};

//! \}

} // namespace nanairo

#endif // NANAIRO_CMJ_SAMPLER_HPP
