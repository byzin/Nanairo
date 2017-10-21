/*!
  \file non_directional_emitter.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace zisc {

// Forward declaration
class MemoryPool;

} // namespace zisc

namespace nanairo {

// Forward declaration
class Sampler;
class TextureModel;
class WavelengthSamples;

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
  NonDirectionalEmitter(const SettingNodeBase* settings,
                        const std::vector<TextureModel*>& texture_list) noexcept;


  //! Make non-directional light
  ShaderPointer makeLight(const Point2& uv,
                          const WavelengthSamples& wavelengths,
                          zisc::MemoryPool& memory_pool) const noexcept override;

  //! Return the non-directional emitter type
  EmitterType type() const noexcept override;

 private:
  //! Initialize the emitter
  void initialize(const SettingNodeBase* settings,
                  const std::vector<TextureModel*>& texture_list) noexcept;


  const TextureModel* color_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_NON_DIRECTIONAL_EMITTER_HPP
