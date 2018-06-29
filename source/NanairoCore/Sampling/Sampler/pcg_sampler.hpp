/*!
  \file pcg_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PCG_SAMPLER_HPP
#define NANAIRO_PCG_SAMPLER_HPP

// Standard C++ library
#include <array>
// Zisc
#include "zisc/pcg_engine.hpp"
// Nanairo
#include "sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class PathState;

//! \addtogroup Core
//! \{

/*!
  */
class PcgSampler : public Sampler
{
 public:
  //! Initialize a sampler
  PcgSampler(const uint32 seed) noexcept;


  //! Sample a [0, 1) float random number
  Float draw1D(const PathState& state) noexcept override;

  //! Sample two [0, 1) float random numbers
  std::array<Float, 2> draw2D(const PathState& state) noexcept override;

 private:
  zisc::PcgLcgRxsMXs32 engine_;
};

//! \}

} // namespace nanairo

#endif // NANAIRO_PCG_SAMPLER_HPP
