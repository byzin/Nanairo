/*!
  \file sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLER_HPP
#define NANAIRO_SAMPLER_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Sampler
{
 public:
  //! Initialize a sampler
  Sampler(const uint64 seed) noexcept;


  //! Sample a [0, 1) float random number
  Float sample() noexcept;

  //! Set seed
  void setSeed(const uint64 seed) noexcept;

 private:
  //! Initialize sampler
  void initialize(const uint64 seed) noexcept;


  SamplerType sampler_;
};

//! \} Core

} // namespace nanairo

#include "sampler-inl.hpp"

#endif // NANAIRO_SAMPLER_HPP
