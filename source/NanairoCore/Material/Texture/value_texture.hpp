/*!
  \file value_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_VALUE_TEXTURE_HPP_
#define _NANAIRO_VALUE_TEXTURE_HPP_

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
class ValueTexture : public Texture
{
 public:
  //! Create a value texture
  ValueTexture(const System& system,
               const SceneSettings& settings, 
               const QString& prefix);

  //! Return the unique float value
  Float uniqueValue() const;

  //! Evaluate a float value at the coordinate
  Float floatValue(const Point2& coordinate) const override;

  //! Return the value texture byte size
  std::size_t textureSize() const override;

  //! Return the value texture type
  TextureType type() const override;
 
  //! Evaluate the value by the wavelength at the texture coordinate
  Float wavelengthValue(const Point2& coordinate, 
                        const uint16 wavelength) const override;
 private:
  //! Initialize
  void initialize(const System& system,
                  const SceneSettings& settings, 
                  const QString& prefix);


  Float value_;
};

//! \} Core

} // namespace nanairo

#include "value_texture-inl.hpp"

#endif // _NANAIRO_VALUE_TEXTURE_HPP_
