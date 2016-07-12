/*!
  \file unicolor_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNICOLOR_TEXTURE_HPP
#define NANAIRO_UNICOLOR_TEXTURE_HPP

// Standard C++ library
#include <cstddef>
// Nanairo
#include "texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"

// Forward declaration
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
class UnicolorTexture : public Texture
{
 public:
  //! Create a unicolor texture
  UnicolorTexture(const System& system,
                  const SceneSettings& settings, 
                  const QString& prefix) noexcept;


  //! Evaluate a float value at the coordinate
  Float floatValue(const Point2& coordinate) const noexcept override;

  //! Evaluate spectra at the coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> unicolorSpectraValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Return the unicolor texture byte size
  std::size_t textureSize() const noexcept override;

  //! Return the unicolor texture type
  TextureType type() const noexcept override;

  //! Evaluate the value by the wavelength at the texture coordinate
  Float wavelengthValue(const Point2& coordinate, 
                        const uint16 wavelength) const noexcept override;
 private:
  //! Initialize
  void initialize(const System& system,
                  const SceneSettings& settings, 
                  const QString& prefix) noexcept;


  SpectralDistribution spectra_value_;
  Float float_value_;
};

//! \} Core

} // namespace nanairo

#include "unicolor_texture-inl.hpp"

#endif // NANAIRO_UNICOLOR_TEXTURE_HPP
