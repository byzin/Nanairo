/*!
  \file checkerboard_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CHECKERBOARD_TEXTURE_HPP
#define NANAIRO_CHECKERBOARD_TEXTURE_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"

// Forward declaration
class QJsonObject;
class QString;

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
  //! Create a checkerboard texture
  CheckerboardTexture(const System& system, const QJsonObject& settings) noexcept;


  //! Evaluate a float value at the coordinate
  Float floatValue(const Point2& coordinate) const noexcept override;

  //! Evaluate the emissive spectra at the coordinate
  SampledSpectra emissiveValue(
      const Point2& coordinate,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate the reflective value by the wavelength at the texture coordinate
  Float reflectiveValue(const Point2& coordinate, 
                        const uint16 wavelength) const noexcept override;

  //! Evaluate the reflective spectra at the coordinate
  SampledSpectra reflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Return the checkerboard texture type
  TextureType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;


  std::unique_ptr<SpectralDistribution> emissive_value_[2];
  std::unique_ptr<SpectralDistribution> reflective_value_[2];
  Float float_value_[2];
  Float width_,
        height_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_CHECKERBOARD_TEXTURE_HPP
