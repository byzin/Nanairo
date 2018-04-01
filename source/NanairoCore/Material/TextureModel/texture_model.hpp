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
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class SampledSpectra;
class SpectralDistribution;
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
  kValue                       = zisc::toHash32("Value"),
  kUnicolor                    = zisc::toHash32("Unicolor"),
  kCheckerboard                = zisc::toHash32("Checkerboard"),
  kImage                       = zisc::toHash32("Image")
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
  static std::unique_ptr<TextureModel> makeTexture(
      const System& system,
      const SettingNodeBase* settings) noexcept;

  //! Return the texture name
  const std::string* name() const noexcept;

  //! Evaluate the reflective value by the wavelength at the uv coordinate
  virtual Float reflectiveValue(
      const Point2& uv,
      const uint16 wavelength) const noexcept = 0;

  //! Evaluate the reflective spectra at the uv coordinate
  virtual SampledSpectra reflectiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept = 0;

  //! Set the name of the texture
  void setName(const std::string& name) noexcept;

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
