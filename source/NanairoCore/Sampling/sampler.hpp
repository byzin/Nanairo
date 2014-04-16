/*!
  \file sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SAMPLER_HPP_
#define _NANAIRO_SAMPLER_HPP_

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
  Sampler(const uint32 seed);


  //! Sample a random number x uniformly satisfing min <= x < max
  template <typename Arithmetic>
  Arithmetic sample(const Arithmetic min, const Arithmetic max);

  //! Set seed
  void setSeed(const uint32 seed);

 private:
  SamplerType sampler_;
};

//! \} Core

} // namespace nanairo

#include "sampler-inl.hpp"

#endif // _NANAIRO_SAMPLER_HPP_
