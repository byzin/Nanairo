/*!
  \file image_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_IMAGE_TEXTURE_HPP
#define NANAIRO_IMAGE_TEXTURE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"

// Forward declaration
class QImage;
class QString;

namespace nanairo {

// Forward declaration
template <uint> class SampledSpectra;
class SceneSettings;
class System;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class ImageTexture : public Texture
{
 public:
  //! Create a image texture
  ImageTexture(const System& system,
               const SceneSettings& settings, 
               const QString& prefix) noexcept;


  //! Evaluate a float value at the coordinate
  Float floatValue(const Point2& coordinate) const noexcept override;

  //! Evaluate spectra at the coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> imageSpectraValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelength) const noexcept;

  //! Return the image texture byte size
  std::size_t textureSize() const noexcept override;

  //! Return the image texture type
  TextureType type() const noexcept override;

  //! Evaluate the value by the wavelength at the texture coordinate
  Float wavelengthValue(const Point2& coordinate, 
                        const uint16 wavelength) const noexcept override;
 private:
  //! Initialize
  void initialize(const System& system,
                  const SceneSettings& settings, 
                  const QString& prefix) noexcept;

  //! Set color
  void setColor(const System& system,
                const QImage& image, 
                const Float gamma) noexcept;


  std::vector<SpectralDistribution> spectra_table_;
  std::vector<Float> float_table_;
  std::vector<uint> color_index_;
  Float width_,
        height_;
  uint width_resolution_;
};

//! \} Core

} // namespace nanairo

#include "image_texture-inl.hpp"

#endif // NANAIRO_IMAGE_TEXTURE_HPP
