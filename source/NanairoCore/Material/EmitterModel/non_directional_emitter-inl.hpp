/*!
  \file non_directional_light-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_NON_DIRECTIONA_LIGHT_INL_HPP
#define NANAIRO_NON_DIRECTIONA_LIGHT_INL_HPP

#include "non_directional_emitter.hpp"
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "emitter_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Light/non_directional_light.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto NonDirectionalEmitter::makeNonDirectionalLight(
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer<kSampleSize>
{
  const auto color = color_->emissiveValue(texture_coordinate, wavelengths);
  const auto radiant_exitance = color * radiantExitance();

  using Light = NonDirectionalLight<kSampleSize>;
  auto light = memory_pool.allocate<Light>(radiant_exitance);
  return ShaderPointer<kSampleSize>{light};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
EmitterModel::ShaderPointer<kSampleSize> makeNonDirectionalLight(
    const EmitterModel* emitter,
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) noexcept
{
  using zisc::cast;

  auto e = cast<const NonDirectionalEmitter*>(emitter);
  return e->makeNonDirectionalLight(texture_coordinate, wavelengths, memory_pool);
}

} // namespace nanairo

#endif // NANAIRO_NON_DIRECTIONA_LIGHT_INL_HPP
