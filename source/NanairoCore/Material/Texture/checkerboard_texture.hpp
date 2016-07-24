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
// Nanairo
#include "texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"

// Forward declaration
class QJsonObject;
class QString;

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
class CheckerboardTexture : public Texture
{
 public:
  //! Create a checkerboard texture
  CheckerboardTexture(const System& system, const QJsonObject& settings) noexcept;


  //! Evaluate a float value at the coordinate
  Float floatValue(const Point2& coordinate) const noexcept override;

  //! Evaluate spectra at the coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> checkerboardSpectraValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const noexcept;

  //! Return the checkerboard texture byte size
  std::size_t textureSize() const noexcept override;

  //! Return the checkerboard texture type
  TextureType type() const noexcept override;

  //! Evaluate the value by the wavelength at the texture coordinate
  Float wavelengthValue(const Point2& coordinate, 
                        const uint16 wavelength) const noexcept override;
 private:
  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;


  SpectralDistribution spectra_value_[2];
  Float float_value_[2];
  Float width_,
        height_;
};

//! \} Core

} // namespace nanairo

#include "checkerboard_texture-inl.hpp"

#endif // NANAIRO_CHECKERBOARD_TEXTURE_HPP
