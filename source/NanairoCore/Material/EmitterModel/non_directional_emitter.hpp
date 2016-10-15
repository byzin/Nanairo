/*!
  \file non_directional_emitter.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_NON_DIRECTIONAL_EMITTER_HPP
#define NANAIRO_NON_DIRECTIONAL_EMITTER_HPP

// Standard C++ library
#include <vector>
// Nanairo
#include "emitter_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class Sampler;
class TextureModel;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class NonDirectionalEmitter : public EmitterModel
{
 public:
  //! Create a non directional emitter
  NonDirectionalEmitter(
      const QJsonObject& settings,
      const std::vector<const TextureModel*>& texture_list) noexcept;


  //! Make non-directional light
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeNonDirectionalLight(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Return the non-directional emitter type
  EmitterType type() const noexcept override;

 private:
  //! Initialize the emitter
  void initialize(const QJsonObject& settings,
                  const std::vector<const TextureModel*>& texture_list) noexcept;


  const TextureModel* color_;
};

//! \} Core

} // namespace nanairo

#include "non_directional_emitter-inl.hpp"

#endif // NANAIRO_NON_DIRECTIONAL_EMITTER_HPP
