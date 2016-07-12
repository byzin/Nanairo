/*!
  \file sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
  Sampler(const uint32 seed) noexcept;


  //! Sample a random number x uniformly satisfing min <= x < max
  template <typename Arithmetic>
  Arithmetic sample(const Arithmetic min, const Arithmetic max) noexcept;

  //! Set seed
  void setSeed(const uint32 seed) noexcept;

 private:
  SamplerType sampler_;
};

//! \} Core

} // namespace nanairo

#include "sampler-inl.hpp"

#endif // NANAIRO_SAMPLER_HPP
