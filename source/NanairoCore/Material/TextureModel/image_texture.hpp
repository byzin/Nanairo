/*!
  \file image_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_IMAGE_TEXTURE_HPP
#define NANAIRO_IMAGE_TEXTURE_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"
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
  using SpectralDistributionPointer =
      SpectralDistribution::SpectralDistributionPointer;


  //! Create a image texture
  ImageTexture(System& system, const SettingNodeBase* settings) noexcept;


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
  //! Return the color index by the texture coordinate
  uint getColorIndex(const Point2& uv) const noexcept;

  //! Return the image pixel index by the texture coordinate
  uint getPixelIndex(const Point2& uv) const noexcept;

  //! Initialize
  void initialize(System& system, const SettingNodeBase* settings) noexcept;

  //! Set color
  void initializeTables(System& system,
                        const LdrImage& image,
                        zisc::pmr::memory_resource* work_resource) noexcept;


  zisc::pmr::vector<SpectralDistributionPointer> spectra_value_table_;
  zisc::pmr::vector<Float> emissive_scale_table_;
  zisc::pmr::vector<Float> gray_scale_table_;
  zisc::pmr::vector<uint> color_index_table_;
  Index2d resolution_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_IMAGE_TEXTURE_HPP
