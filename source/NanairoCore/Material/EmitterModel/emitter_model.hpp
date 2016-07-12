/*!
  \file emitter_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_EMITTER_MODEL_HPP
#define NANAIRO_EMITTER_MODEL_HPP

// Standard C++ library
#include <cstddef>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class Sampler;
class SceneSettings;
template <uint> class ShaderModel;
class System;
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
  EmitterModel(const System& system,
               const SceneSettings& settings, 
               const QString& prefix) noexcept;

  //! Finalize the emitter model
  virtual ~EmitterModel() noexcept;


  //! Return the emitter model size
  std::size_t emitterSize() const noexcept {return sizeof(*this);}

  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeLight(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Return the spectral power distribution
  const SpectralDistribution& powerDistribution() const noexcept;

  //! Return the radiant exitance 
  Float radiantExitance() const noexcept;

  //! Return the emitter type
  virtual EmitterType type() const noexcept = 0;
 
 protected:
  //! Set the spectral power distribution
  void setPowerDistribution(const SpectralDistribution& distribution) noexcept;

 private:
  //! Initialize
  void initialize(const System& system,
                  const SceneSettings& settings, 
                  const QString& prefix) noexcept;


  SpectralDistribution power_distribution_;
  Float radiant_exitance_;
};

//! Make a non directional emitter
template <uint kSampleSize>
EmitterModel::ShaderPointer<kSampleSize> makeNonDirectionalLight(
    const EmitterModel* emitter,
    const Point2& texture_coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths,
    MemoryPool& memory_pool) noexcept;

//! Make a emitter model
UniquePointer<EmitterModel> makeEmitter(const System& system,
                                        const SceneSettings& settings,
                                        const QString& prefix) noexcept;

//! \} Core

} // namespace nanairo

#include "emitter_model-inl.hpp"

#endif // NANAIRO_EMITTER_MODEL_HPP
