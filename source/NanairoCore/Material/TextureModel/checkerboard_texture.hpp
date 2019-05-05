/*!
  \file checkerboard_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CHECKERBOARD_TEXTURE_HPP
#define NANAIRO_CHECKERBOARD_TEXTURE_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"
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
class CheckerboardTexture : public TextureModel
{
 public:
  using SpectralDistributionPointer =
      SpectralDistribution::SpectralDistributionPointer;


  //! Create a checkerboard texture
  CheckerboardTexture(System& system,
                      const SettingNodeBase* settings) noexcept;


  //! Evaluate the emissive spectra at the uv coordinate
  SampledSpectra emissiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate a gray scale value at the uv coordinate
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

  //! Evaluate the spectra at the uv coordinate
  SampledSpectra spectraValue(
      const Point2& uv,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Return the checkerboard texture type
  TextureType type() const noexcept override;

 private:
  //! Get the checkerboard index
  uint getIndex(const Point2& uv) const noexcept;

  //! Initialize
  void initialize(System& system,
                  const SettingNodeBase* settings) noexcept;


  std::array<SpectralDistributionPointer, 2> data_value_;
  std::array<SpectralDistributionPointer, 2> color_value_;
  std::array<Float, 2> emissive_scale_;
  std::array<Float, 2> gray_scale_value_;
  std::array<Float, 2> resolution_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_CHECKERBOARD_TEXTURE_HPP
