/*!
  \file texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TEXTURE_HPP
#define NANAIRO_TEXTURE_HPP

// Standard C+ library
#include <cstddef>
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
template <uint> class SampledSpectra;
class SpectralDistribution;
template <uint> class WavelengthSamples;
class SceneSettings;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
enum class TextureType : int
{
  Value = 0,
  Unicolor,
  Checkerboard,
  Image
};

/*!
  \details
  No detailed.
  */
class Texture
{
 public:
  //! Dummy function
  virtual ~Texture() noexcept;


  //! Evaluate a float value at the texture coordinate
  virtual Float floatValue(const Point2& coordinate) const noexcept = 0;

  //! Evaluate spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> spectraValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Return the texture byte size
  virtual std::size_t textureSize() const noexcept = 0;

  //! Return the texture type
  virtual TextureType type() const noexcept = 0;

  //! Evaluate the value by the wavelength at the texture coordinate
  virtual Float wavelengthValue(const Point2& coordinate, 
                                const uint16 wavelength) const noexcept = 0;
};

//! Get the spectra of the texture
template <uint kSampleSize>
SampledSpectra<kSampleSize> getSpectraValue(
    const Texture* texture,
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) noexcept;

//! Get the spectra of the unicolor texture 
template <uint kSampleSize>
SampledSpectra<kSampleSize> getUnicolorSpectraValue(
    const Texture* texture,
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) noexcept;

//! Get the spectra of the checkerboard texture 
template <uint kSampleSize>
SampledSpectra<kSampleSize> getCheckerboardSpectraValue(
    const Texture* texture,
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) noexcept;

//! Get the spectra of the image texture
template <uint kSampleSize>
SampledSpectra<kSampleSize> getImageSpectraValue(
    const Texture* texture,
    const Point2& coordinate,
    const WavelengthSamples<kSampleSize>& wavelengths) noexcept;

//! Make a texture
UniquePointer<Texture> makeTexture(const System& system,
                                   const SceneSettings& settings,
                                   const QString& prefix) noexcept;

//! Get a texture
const Texture* getTexture(const SceneSettings& settings,
                          const QString& prefix,
                          const std::vector<const Texture*>& texture_list) noexcept;

//! \} Core

} // namespace nanairo

#include "texture-inl.hpp"

#endif // NANAIRO_TEXTURE_HPP
