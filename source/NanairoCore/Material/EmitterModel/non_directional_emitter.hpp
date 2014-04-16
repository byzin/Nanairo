/*!
  \file non_directional_emitter.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_NON_DIRECTIONAL_EMITTER_HPP_
#define _NANAIRO_NON_DIRECTIONAL_EMITTER_HPP_

// Nanairo
#include "emitter_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class SceneSettings;
class QString;

namespace nanairo {

// Forward declaration
class Sampler;
class System;
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
  NonDirectionalEmitter(const System& system,
                        const SceneSettings& settings, 
                        const QString& prefix);


  //! Make non-directional light
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeNonDirectionalLight(
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const;

  //! Return the non-directional emitter type
  EmitterType type() const override;

 private:
  //! Initialize the emitter
  void initialize(const System& system,
                  const SceneSettings& settings,
                  const QString& prefix);
};

//! \} Core

} // namespace nanairo

#include "non_directional_emitter-inl.hpp"

#endif // _NANAIRO_NON_DIRECTIONAL_EMITTER_HPP_
