/*!
  \file non_directional_emitter.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "non_directional_emitter.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/utility.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "emitter_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/Light/non_directional_light.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/emitter_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
NonDirectionalEmitter::NonDirectionalEmitter(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
        : EmitterModel(settings)
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto NonDirectionalEmitter::makeLight(
    const Point2& uv,
    const WavelengthSamples& wavelengths,
    zisc::pmr::memory_resource* mem_resource) const noexcept -> ShaderPointer
{
  const auto color = color_->emissiveValue(uv, wavelengths);
  const auto radiant_exitance = color * radiantExitance();

  using LightPointer = zisc::UniqueMemoryPointer<NonDirectionalLight>;
  auto ptr = LightPointer::make(mem_resource, radiant_exitance);
  return ptr;
}

/*!
  \details
  No detailed.
  */
EmitterType NonDirectionalEmitter::type() const noexcept
{
  return EmitterType::kNonDirectional;
}

/*!
  \details
  No detailed.
  */
void NonDirectionalEmitter::initialize(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  const auto emitter_settings = castNode<EmitterSettingNode>(settings);

  const auto& parameters = emitter_settings->nonDirectionalEmitterParameters();
  {
    const Float radiant_exitance = zisc::cast<Float>(parameters.radiant_exitance_);
    ZISC_ASSERT(0.0 < radiant_exitance, "Radiance exitance is negative.");
    setRadiantExitance(radiant_exitance);
  }
  {
    const uint color_index = parameters.color_index_;
    color_ = texture_list[color_index];
  }
}

} // namespace nanairo
