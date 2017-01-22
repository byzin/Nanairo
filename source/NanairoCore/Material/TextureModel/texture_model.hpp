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
  virtual SampledSpectra emissiveValue(
      const Point2& coordinate,
      const WavelengthSamples& wavelengths) const noexcept = 0;

  //! Make a texture
  static UniquePointer<TextureModel> makeTexture(
      const System& system,
      const QJsonObject& settings) noexcept;

  //! Evaluate the reflective value by the wavelength at the texture coordinate
  virtual Float reflectiveValue(const Point2& coordinate,
                                const uint16 wavelength) const noexcept = 0;

  //! Evaluate reflective spectra at the texture coordinate
  virtual SampledSpectra reflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples& wavelengths) const noexcept = 0;

  //! Return the texture type
  virtual TextureType type() const noexcept = 0;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_TEXTURE_MODEL_HPP
