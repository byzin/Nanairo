/*!
  \file value_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_VALUE_TEXTURE_HPP
#define NANAIRO_VALUE_TEXTURE_HPP

// Standard C++ library
#include <cstddef>
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
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
class ValueTexture : public TextureModel
{
 public:
  //! Create a value texture
  ValueTexture(const System& system, const SettingNodeBase* settings) noexcept;


  //! Evaluate the emissive spectra at the uv coordinate
  SampledSpectra emissiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate the scale value at the uv coordinate
  Float grayScaleValue(const Point2& uv) const noexcept override;

  //! Evaluate the reflective value by the wavelength at the uv coordinate
  Float reflectiveValue(
      const Point2& uv,
      const uint16 wavelength) const noexcept override;

  //! Evaluate the reflective spectra at the uv coordinate
  SampledSpectra reflectiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate the spectra value by the wavelength at the uv coordinate
  Float spectraValue(
      const Point2& uv,
      const uint16 wavelength) const noexcept override;

  //! Evaluate the spectra value by the wavelength at the uv coordinate
  SampledSpectra spectraValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Return the value texture type
  TextureType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const System& system, const SettingNodeBase* settings) noexcept;


  Float spectra_value_;
  Float emissive_value_;
  Float reflective_value_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_VALUE_TEXTURE_HPP
