/*!
  \file image_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_IMAGE_TEXTURE_HPP
#define NANAIRO_IMAGE_TEXTURE_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <vector>
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class LdrImage;
class SampledSpectra;
class System;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class ImageTexture : public TextureModel
{
 public:
  //! Create a image texture
  ImageTexture(const System& system, const SettingNodeBase* settings) noexcept;


  //! Evaluate the reflective spectra at the uv coordinate
  SampledSpectra emissiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelength) const noexcept override;

  //! Evaluate a float value at the uv coordinate
  Float grayScaleValue(const Point2& uv) const noexcept override;

  //! Evaluate the reflective value by the wavelength at the uv coordinate
  Float reflectiveValue(
      const Point2& uv, 
      const uint16 wavelength) const noexcept override;

  //! Evaluate the reflective spectra at the uv coordinate
  SampledSpectra reflectiveValue(
      const Point2& uv,
      const WavelengthSamples& wavelength) const noexcept override;

  //! Evaluate the spectra value by the wavelength at the uv coordinate
  Float spectraValue(
      const Point2& uv, 
      const uint16 wavelength) const noexcept override;

  //! Evaluate the spectra at the uv coordinate
  SampledSpectra spectraValue(
      const Point2& uv,
      const WavelengthSamples& wavelength) const noexcept override;

  //! Return the image texture type
  TextureType type() const noexcept override;

 private:
  //! Return the image pixel index by the texture coordinate
  uint getPixelIndex(const Point2& coordinate) const noexcept;

  //! Initialize
  void initialize(const System& system, const SettingNodeBase* settings) noexcept;

  //! Set color
  void initializeTables(const System& system, const LdrImage& image) noexcept;


  std::vector<SpectralDistribution> emissive_value_table_;
  std::vector<SpectralDistribution> reflective_value_table_;
  std::vector<Float> gray_scale_table_;
  std::vector<uint> color_index_table_;
  std::array<uint, 2> resolution_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_IMAGE_TEXTURE_HPP
