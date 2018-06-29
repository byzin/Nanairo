/*!
  \file xoshiro_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_XOSHIRO_SAMPLER_HPP
#define NANAIRO_XOSHIRO_SAMPLER_HPP

// Standard C++ library
#include <array>
// Zisc
#include "zisc/xoshiro_2star_engine.hpp"
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
class XoshiroSampler : public Sampler
{
 public:
  //! Initialize a sampler
  XoshiroSampler(const uint32 seed) noexcept;


  //! Sample a [0, 1) float random number
  Float draw1D(const PathState& state) noexcept override;

  //! Sample two [0, 1) float random numbers
  std::array<Float, 2> draw2D(const PathState& state) noexcept override;

 private:
  zisc::Xoshiro2Star32 engine_;
};

//! \}

} // namespace nanairo

#endif // NANAIRO_XOSHIRO_SAMPLER_HPP
