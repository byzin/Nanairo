/*!
  \file unicolor_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNICOLOR_TEXTURE_HPP
#define NANAIRO_UNICOLOR_TEXTURE_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
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
class UnicolorTexture : public TextureModel
{
 public:
  //! Create a unicolor texture
  UnicolorTexture(const System& system, const SettingNodeBase* settings) noexcept;


  //! Evaluate a emissive spectra at the uv coordinate
  SampledSpectra emissiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate a gray scale value at the uv coordinate
  Float grayScaleValue(const Point2& uv) const noexcept override;

  //! Evaluate the reflective value by the wavelength at the uv coordinate
  Float reflectiveValue(
      const Point2& uv,
      const uint16 wavelength) const noexcept override;

  //! Evaluate a reflective spectra at the uv coordinate
  SampledSpectra reflectiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate the spectra value by the wavelength at the uv coordinate
  Float spectraValue(
      const Point2& uv,
      const uint16 wavelength) const noexcept override;

  //! Evaluate the spectra  at the uv coordinate
  SampledSpectra spectraValue(
      const Point2& uv,
      const WavelengthSamples& wavelength) const noexcept override;

  //! Return the unicolor texture type
  TextureType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const System& system, const SettingNodeBase* settings) noexcept;


  SpectralDistribution spectra_value_;
  SpectralDistribution emissive_value_;
  SpectralDistribution reflective_value_;
  Float gray_scale_value_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_UNICOLOR_TEXTURE_HPP
