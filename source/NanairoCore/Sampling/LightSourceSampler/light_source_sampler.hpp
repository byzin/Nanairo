/*!
  \file light_source_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_SOURCE_SAMPLER_HPP
#define NANAIRO_LIGHT_SOURCE_SAMPLER_HPP

// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
// Nanairo
#include "NanairoCore/Data/light_source_reference.hpp"
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
  kUniform                    = zisc::toHash32("UniformLightSampler"),
  kPowerWeighted              = zisc::toHash32("PowerWeightedLightSampler"),
  kContributionWeighted       = zisc::toHash32("ContributionWeightedLightSampler")
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

  virtual ~LightSourceSampler() noexcept;


  //! Return the light source reference by the light source
  virtual const LightSourceReference& getReference(
      const IntersectionInfo& info,
      const Object* light_source) const noexcept = 0;

  //! Make a light source sampler
  static std::unique_ptr<LightSourceSampler> makeSampler(
      const SettingNodeBase* settings,
      const World& world,
      System& system) noexcept;

  //! Sample a light source
  virtual const LightSourceReference& sample(const IntersectionInfo& info,
                                             Sampler& sampler) const noexcept = 0;

 protected:
  //! Return the light source reference by the light source
  const LightSourceReference& getReference(
      const std::vector<LightSourceReference>& reference_list,
      const Object* light_source) const noexcept;

 private:
  //! Initialize
  void initialize() noexcept;

  //! Return the invalid reference
  const LightSourceReference& invalidReference() const noexcept;


  LightSourceReference invalid_reference_;
};

//! \} Core

} // namespace nanairo

#include "light_source_sampler-inl.hpp"

#endif // NANAIRO_LIGHT_SOURCE_SAMPLER_HPP
