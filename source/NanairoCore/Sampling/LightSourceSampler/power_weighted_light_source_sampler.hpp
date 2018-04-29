/*!
  \file power_weighted_light_source_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP
#define NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP

// Standard C++ library
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/cumulative_distribution_function.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "light_source_sampler.hpp"
#include "NanairoCore/Data/light_source_info.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Sampler;
class System;
class World;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class PowerWeightedLightSourceSampler : public LightSourceSampler
{
 public:
  using LightSourceCdf = zisc::CumulativeDistributionFunction<const LightSourceInfo*,
                                                              Float>;


  //! Create a light source sampler
  PowerWeightedLightSourceSampler(
      System& system,
      const World& world,
      zisc::pmr::memory_resource* work_resource) noexcept;


  //! Return the light source info by the light source
  LightSourceInfo getInfo(const IntersectionInfo& info,
                          const Object* light_source) const noexcept override;

  //! Return the info list of light source
  zisc::pmr::vector<LightSourceInfo>& infoList() noexcept;

  //! Return the info list of light source
  const zisc::pmr::vector<LightSourceInfo>& infoList() const noexcept;

  //! Return the light source CDF
  LightSourceCdf& lightSourceCdf() noexcept;

  //! Return the light source CDF
  const LightSourceCdf& lightSourceCdf() const noexcept;

  //! Sample a light source for a light path tracer
  LightSourceInfo sample(Sampler& sampler) const noexcept override;

  //! Sample a light source for a eye path tracer
  LightSourceInfo sample(const IntersectionInfo& info,
                         Sampler& sampler) const noexcept override;

 private:
  //! Return the light source info by the light source
  LightSourceInfo getInfo(const Object* light_source) const noexcept;

  //! Initialize
  void initialize(System& system,
                  const World& world,
                  zisc::pmr::memory_resource* work_resource) noexcept;

  //! Sample a light source
  const LightSourceInfo& sampleInfo(Sampler& sampler) const noexcept;


  zisc::pmr::vector<LightSourceInfo> info_list_;
  zisc::UniqueMemoryPointer<LightSourceCdf> light_source_cdf_;
};

//! \} Core

} // namespace nanairo

#include "power_weighted_light_source_sampler-inl.hpp"

#endif // NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP
