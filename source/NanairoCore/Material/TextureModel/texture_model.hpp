/*!
  \file texture_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TEXTURE_MODEL_HPP
#define NANAIRO_TEXTURE_MODEL_HPP

// Standard C+ library
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class SampledSpectra;
class System;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
enum class TextureType : uint32
{
  kValue                       = zisc::Fnv1aHash32::hash("Value"),
  kUnicolor                    = zisc::Fnv1aHash32::hash("Unicolor"),
  kCheckerboard                = zisc::Fnv1aHash32::hash("Checkerboard"),
  kImage                       = zisc::Fnv1aHash32::hash("Image")
};

/*!
  \details
  No detailed.
  */
class TextureModel
{
 public:
  //! Dummy function
  virtual ~TextureModel() noexcept;


  //! Evaluate the spectra at the uv coordinate
  virtual SampledSpectra emissiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept = 0;

  //! Evaluate the gray scale value at the uv coordinate
  virtual Float grayScaleValue(const Point2& uv) const noexcept = 0;

  //! Make a texture
  static zisc::UniqueMemoryPointer<TextureModel> makeTexture(
      System& system,
      const SettingNodeBase* settings) noexcept;

  //! Return the texture name
  std::string_view name() const noexcept;

  //! Evaluate the reflective value by the wavelength at the uv coordinate
  virtual Float reflectiveValue(
      const Point2& uv,
      const uint16 wavelength) const noexcept = 0;

  //! Evaluate the reflective spectra at the uv coordinate
  virtual SampledSpectra reflectiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept = 0;

  //! Set the name of the texture
  void setName(const std::string_view& name) noexcept;

  //! Evaluate the spectra value by the wavelength at the uv coordinate
  virtual Float spectraValue(
      const Point2& uv,
      const uint16 wavelength) const noexcept = 0;

  //! Evaluate the spectra at the uv coordinate
  virtual SampledSpectra spectraValue(
      const Point2& uv,
      const WavelengthSamples& wavelength) const noexcept = 0;

  //! Return the texture type
  virtual TextureType type() const noexcept = 0;

 private:
#ifdef Z_DEBUG_MODE
  std::string name_;
#endif // Z_DEBUG_MODE
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_TEXTURE_MODEL_HPP
