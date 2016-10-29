/*!
  \file value_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
template <uint> class SampledSpectra;
class System;
template <uint> class WavelengthSamples;

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
  ValueTexture(const System& system, const QJsonObject& settings) noexcept;

  //! Evaluate a float value at the coordinate
  Float floatValue(const Point2& coordinate) const noexcept override;

  //! Evaluate a emissive spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> emissiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Evaluate the reflective value by the wavelength at the texture coordinate
  Float reflectiveValue(const Point2& coordinate,
                        const uint16 wavelength) const noexcept override;

  //! Evaluate a reflective spectra at the texture coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> reflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Return the value texture type
  TextureType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;


  Float emissive_value_;
  Float reflective_value_;
};

//! \} Core

} // namespace nanairo

#include "value_texture-inl.hpp"

#endif // NANAIRO_VALUE_TEXTURE_HPP
