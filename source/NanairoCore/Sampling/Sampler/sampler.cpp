/*!
  \file sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "sampler.hpp"
// Standard C++ library
#include <array>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "cmj_sampler.hpp"
#include "pcg_sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
Sampler::~Sampler() noexcept
{
}

/*!
  */
zisc::UniqueMemoryPointer<Sampler> Sampler::make(
    const SamplerType type,
    const uint32 seed,
    zisc::pmr::memory_resource* mem_resource) noexcept
{
  zisc::UniqueMemoryPointer<Sampler> sampler;
  switch (type) {
   case SamplerType::kPcg: {
    sampler = zisc::UniqueMemoryPointer<PcgSampler>::make(mem_resource, seed);
    break;
   }
   case SamplerType::kCmj: {
    sampler = zisc::UniqueMemoryPointer<CmjSampler>::make(mem_resource, seed);
    break;
   }
   default:
    break;
  }
  return sampler;
}

} // namespace nanairo
