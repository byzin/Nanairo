/*!
  \file non_directional_emitter.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "non_directional_emitter.hpp"
// Standard C++ library
#include <vector>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "emitter_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Light/non_directional_light.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
NonDirectionalEmitter::NonDirectionalEmitter(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
        : EmitterModel(settings)
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto NonDirectionalEmitter::makeLight(
    const Point2& texture_coordinate,
    const WavelengthSamples& wavelengths,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto color = color_->emissiveValue(texture_coordinate, wavelengths);
  const auto radiant_exitance = color * radiantExitance();

  using Light = NonDirectionalLight;
  auto light = memory_pool.allocate<Light>(radiant_exitance);
  return ShaderPointer{light};
}

/*!
  \details
  No detailed.
  */
EmitterType NonDirectionalEmitter::type() const noexcept
{
  return EmitterType::NonDirectional;
}

/*!
  \details
  No detailed.
  */
void NonDirectionalEmitter::initialize(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const Float radiant_exitance =
      SceneValue::toFloat<Float>(settings, keyword::radiantExitance);
  ZISC_ASSERT(0.0 < radiant_exitance, "Radiance exitance is negative.");
  setRadiantExitance(radiant_exitance);

  const uint color_index =
      SceneValue::toInt<uint>(settings, keyword::emissiveColorIndex);
  color_ = texture_list[color_index];
}

} // namespace nanairo
