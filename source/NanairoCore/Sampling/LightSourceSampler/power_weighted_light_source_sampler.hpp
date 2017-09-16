/*!
  \file power_weighted_light_source_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP
#define NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP

// Standard C++ library
#include <vector>
// Zisc
#include "zisc/cumulative_distribution_function.hpp"
// Nanairo
#include "light_source_sampler.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Sampler;
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
  using LightSourceCdf =
      zisc::CumulativeDistributionFunction<const LightSourceReference*, Float>;


  //! Create a light source sampler
  PowerWeightedLightSourceSampler(const World& world) noexcept;


  //! Return the light source reference by the light source
  const LightSourceReference& getReference(
      const IntersectionInfo& info,
      const Object* light_source) const noexcept override;

  //! Return the light source CDF
  const LightSourceCdf& lightSourceCdf() const noexcept;

  //! Return the light source CDF
  LightSourceCdf& lightSourceCdf() noexcept;

  //! Return the reference list of light source
  const std::vector<LightSourceReference>& referenceList() const noexcept;

  //! Return the reference list of light source
  std::vector<LightSourceReference>& referenceList() noexcept;

  //! Sample a light source
  const LightSourceReference& sample(Sampler& sampler) const noexcept;

  //! Sample a light source
  const LightSourceReference& sample(const IntersectionInfo& info,
                                     Sampler& sampler) const noexcept override;

 private:
  //! Initialize
  void initialize(const World& world) noexcept;


  std::vector<LightSourceReference> reference_list_;
  std::unique_ptr<LightSourceCdf> light_source_cdf_;
};

//! \} Core

} // namespace nanairo

#include "power_weighted_light_source_sampler-inl.hpp"

#endif // NANAIRO_POWER_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP
