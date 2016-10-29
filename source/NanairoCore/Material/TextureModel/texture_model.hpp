/*!
  \file texture_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TEXTURE_MODEL_HPP
#define NANAIRO_TEXTURE_MODEL_HPP

// Standard C+ library
#include <cstddef>
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
template <uint> class SampledSpectra;
class SpectralDistribution;
class System;
template <uint> class WavelengthSamples;

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
class TextureModel
{
 public:
  //! Dummy function
  virtual ~TextureModel() noexcept;


  //! Evaluate a float value at the texture coordinate
  virtual Float floatValue(const Point2& coordinate) const noexcept = 0;

  //! Evaluate emissive spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> emissiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Make a texture
  static UniquePointer<TextureModel> makeTexture(
      const System& system,
      const QJsonObject& settings) noexcept;

  //! Evaluate the reflective value by the wavelength at the texture coordinate
  virtual Float reflectiveValue(const Point2& coordinate,
                                const uint16 wavelength) const noexcept = 0;

  //! Evaluate reflective spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> reflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Return the texture type
  virtual TextureType type() const noexcept = 0;

 protected:
   //! Evaluate emissive spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> getValueEmissiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Evaluate reflective spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> getValueReflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

   //! Evaluate emissive spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> getUnicolorEmissiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Evaluate reflective spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> getUnicolorReflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

   //! Evaluate emissive spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> getCheckerboardEmissiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Evaluate reflective spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> getCheckerboardReflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

   //! Evaluate emissive spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> getImageEmissiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Evaluate reflective spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> getImageReflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;
};

//! \} Core

} // namespace nanairo

#include "texture_model-inl.hpp"

#endif // NANAIRO_TEXTURE_MODEL_HPP
