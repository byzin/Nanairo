/*!
  \file denoiser.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_DENOISER_HPP
#define NANAIRO_DENOISER_HPP

// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class SampleStatistics;
class System;

/*!
  */
class Denoiser
{
 public:
  //! Initialize a denoiser
  Denoiser() noexcept;

  //! Finalize the denoiser
  virtual ~Denoiser() noexcept;


  //! Denoise input value
  virtual void denoise(System& system,
                       const uint32 cycle,
                       SampleStatistics* statistics) const noexcept = 0;

  //! Make a denoiser
  static zisc::UniqueMemoryPointer<Denoiser> makeDenoiser(
      System& system) noexcept;

 private:
};

} // namespace nanairo

#endif // NANAIRO_DENOISER_HPP
