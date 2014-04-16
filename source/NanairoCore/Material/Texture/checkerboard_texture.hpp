/*!
  \file checkerboard_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_CHECKERBOARD_TEXTURE_HPP_
#define _NANAIRO_CHECKERBOARD_TEXTURE_HPP_

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
class CheckerboardTexture : public Texture
{
 public:
  //! Create a checkerboard texture
  CheckerboardTexture(const System& system,
                      const SceneSettings& settings, 
                      const QString& prefix);


  //! Evaluate a float value at the coordinate
  Float floatValue(const Point2& coordinate) const override;

  //! Evaluate spectra at the coordinate
  template <uint kSampleSize>
  SampledSpectra<kSampleSize> checkerboardSpectraValue(
      const Point2& coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths) const;

  //! Return the checkerboard texture byte size
  std::size_t textureSize() const override;

  //! Return the checkerboard texture type
  TextureType type() const override;

  //! Evaluate the value by the wavelength at the texture coordinate
  Float wavelengthValue(const Point2& coordinate, 
                        const uint16 wavelength) const override;
 private:
  //! Initialize
  void initialize(const System& system,
                  const SceneSettings& settings, 
                  const QString& prefix);


  SpectralDistribution spectra_value_[2];
  Float float_value_[2];
  Float width_,
        height_;
};

//! \} Core

} // namespace nanairo

#include "checkerboard_texture-inl.hpp"

#endif // _NANAIRO_CHECKERBOARD_TEXTURE_HPP_
