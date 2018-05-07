/*!
  \file emitter_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_EMITTER_MODEL_HPP
#define NANAIRO_EMITTER_MODEL_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace zisc {

// Forward declaration
class MemoryPool;

} // namespace zisc

namespace nanairo {

// Forward declaration
class Sampler;
class ShaderModel;
class System;
class TextureModel;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
enum class EmitterType : uint32
{
  kNonDirectional              = zisc::Fnv1aHash32::hash("NonDirectional")
};

/*!
  \details
  No detailed.
  */
class EmitterModel
{
 public:
  using ShaderPointer = zisc::UniqueMemoryPointer<ShaderModel>;


  //! Create a emitter model
  EmitterModel(const SettingNodeBase* settings) noexcept;

  //! Finalize the emitter model
  virtual ~EmitterModel() noexcept;


  //! Make a emitter model
  static zisc::UniqueMemoryPointer<EmitterModel> makeEmitter(
      System& system,
      const SettingNodeBase* settings,
      const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept;

  //! Make a light shader model
  virtual ShaderPointer makeLight(const Point2& uv,
                                  const WavelengthSamples& wavelengths,
                                  zisc::pmr::memory_resource* mem_resource) const noexcept = 0;

  //! Return the emitter name 
  std::string_view name() const noexcept;

  //! Return the radiant exitance
  Float radiantExitance() const noexcept;

  //! Set the emitter name
  void setName(const std::string_view& name) noexcept;

  //! Return the emitter type
  virtual EmitterType type() const noexcept = 0;

 protected:
  //! Set the radiant exitance
  void setRadiantExitance(const Float radiant_exitance) noexcept;

 private:
  //! Initialize
  void initialize(const SettingNodeBase* settings) noexcept;


  Float radiant_exitance_;
#ifdef Z_DEBUG_MODE
  std::string name_;
#endif // Z_DEBUG_MODE
};

//! \} Core

} // namespace nanairo

#include "emitter_model-inl.hpp"

#endif // NANAIRO_EMITTER_MODEL_HPP
