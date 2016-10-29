/*!
  \file emitter_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_EMITTER_MODEL_HPP
#define NANAIRO_EMITTER_MODEL_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class Sampler;
template <uint> class ShaderModel;
class System;
class TextureModel;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
enum class EmitterType : int
{
  NonDirectional = 0,
};

/*!
  \details
  No detailed.
  */
class EmitterModel
{
 public:
  template <uint kSampleSize>
  using ShaderPointer = UniquePointer<ShaderModel<kSampleSize>>;


  //! Create a emitter model
  EmitterModel(const QJsonObject& settings) noexcept;

  //! Finalize the emitter model
  virtual ~EmitterModel() noexcept;


  //! Make a emitter model
  static UniquePointer<EmitterModel> makeEmitter(
      const QJsonObject& settings,
      const std::vector<const TextureModel*>& texture_list) noexcept;

  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeLight(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Return the radiant exitance
  Float radiantExitance() const noexcept;

  //! Return the emitter type
  virtual EmitterType type() const noexcept = 0;

 protected:
  //! Set the radiant exitance
  void setRadiantExitance(const Float radiant_exitance) noexcept;

 private:
  //! Initialize
  void initialize(const QJsonObject& settings) noexcept;

  //! Make a non directional emitter
  template <uint kSampleSize>
  EmitterModel::ShaderPointer<kSampleSize> makeNonDirectionalLight(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;


  Float radiant_exitance_;
};

//! \} Core

} // namespace nanairo

#include "emitter_model-inl.hpp"

#endif // NANAIRO_EMITTER_MODEL_HPP
