/*!
  \file emitter_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_EMITTER_MODEL_INL_HPP
#define NANAIRO_EMITTER_MODEL_INL_HPP

#include "emitter_model.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "non_directional_emitter.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto EmitterModel::makeLight(
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer<kSampleSize>
{
  using zisc::cast;

  ShaderPointer<kSampleSize> light;
  const auto emitter_type = type();
  switch (emitter_type) {
   case EmitterType::NonDirectional: {
    light = makeNonDirectionalLight(texture_coordinate, wavelengths, memory_pool);
    break;
   }
   default: {
    zisc::raiseError("EmitterModelError: (type=", cast<int>(emitter_type),
                     ") makeLight method is not implemented.");
    break;
   }
  }
  return light;
}

/*!
  \details
  No detailed.
  */
inline
Float EmitterModel::radiantExitance() const noexcept
{
  return radiant_exitance_;
}

} // namespace nanairo

#endif // NANAIRO_EMITTER_MODEL_INL_HPP
