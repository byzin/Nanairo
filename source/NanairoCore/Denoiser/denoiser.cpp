/*!
  \file denoiser.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "denoiser.hpp"
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "bayesian_collaborative_denoiser.hpp"
#include "NanairoCore/system.hpp"

namespace nanairo {

/*!
  */
Denoiser::Denoiser() noexcept
{
}

/*!
  */
Denoiser::~Denoiser() noexcept
{
}

/*!
  */
zisc::UniqueMemoryPointer<Denoiser> Denoiser::makeDenoiser(
    System& system) noexcept
{
  zisc::UniqueMemoryPointer<Denoiser> denoiser;
  auto data_resource = &system.dataMemoryManager();

  denoiser = zisc::UniqueMemoryPointer<BayesianCollaborativeDenoiser>::make(
      data_resource);

  return denoiser;
}

} // namespace nanairo
