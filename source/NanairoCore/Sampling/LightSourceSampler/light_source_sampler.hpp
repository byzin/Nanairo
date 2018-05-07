/*!
  \file light_source_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_SOURCE_SAMPLER_HPP
#define NANAIRO_LIGHT_SOURCE_SAMPLER_HPP

// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/Data/light_source_info.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Sampler;
class System;
class World;

//! \addtogroup Core
//! \{

enum class LightSourceSamplerType : uint32
{
  kUniform                    = zisc::Fnv1aHash32::hash("UniformLightSampler"),
  kPowerWeighted              = zisc::Fnv1aHash32::hash("PowerWeightedLightSampler"),
  kContributionWeighted       = zisc::Fnv1aHash32::hash("ContributionWeightedLightSampler")
};

/*!
  \details
  No detailed.
  */
class LightSourceSampler
{
 public:
  //! Create a light source sampler
  LightSourceSampler() noexcept;

  //!
  virtual ~LightSourceSampler() noexcept;


  //! Return the light source info by the light source
  virtual LightSourceInfo getInfo(const IntersectionInfo& info,
                                  const Object* light_source) const noexcept = 0;

  //! Make a light source sampler
  static zisc::UniqueMemoryPointer<LightSourceSampler> makeSampler(
      System& system,
      const LightSourceSamplerType sampler_type,
      const World& world,
      zisc::pmr::memory_resource* work_resource) noexcept;

  //! Sample a light source for a light path tracer
  virtual LightSourceInfo sample(Sampler& sampler) const noexcept = 0;

  //! Sample a light source for a eye path tracer
  virtual LightSourceInfo sample(const IntersectionInfo& info,
                                 Sampler& sampler) const noexcept = 0;

 private:
  //! Initialize
  void initialize() noexcept;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_LIGHT_SOURCE_SAMPLER_HPP
