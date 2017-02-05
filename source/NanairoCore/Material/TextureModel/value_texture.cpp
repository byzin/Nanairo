/*!
  \file value_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "value_texture.hpp"
// Standard C++ library
#include <cstddef>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ValueTexture::ValueTexture(const System& system,
                           const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
Float ValueTexture::floatValue(const Point2& /* coordinate */) const noexcept
{
  return reflective_value_;
}

/*!
  \details
  No detailed.
  */
SampledSpectra ValueTexture::emissiveValue(
    const Point2& /* coordinate */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return SampledSpectra{wavelengths, emissive_value_};
}

/*!
  \details
  No detailed.
  */
Float ValueTexture::reflectiveValue(const Point2& /* coordinate */,
                                    const uint16 /* wavelength */) const noexcept
{
  return reflective_value_;
}

/*!
  \details
  No detailed.
  */
SampledSpectra ValueTexture::reflectiveValue(
    const Point2& /* coordinate */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return SampledSpectra{wavelengths, reflective_value_};
}

/*!
  \details
  No detailed.
  */
TextureType ValueTexture::type() const noexcept
{
  return TextureType::Value;
}

/*!
  \details
  No detailed.
  */
void ValueTexture::initialize(const System& system,
                              const QJsonObject& settings) noexcept
{
  // Emissive value
  emissive_value_ = system.isRgbRenderingMode()
      ? zisc::invert(3.0)
      : zisc::invert(zisc::cast<Float>(CoreConfig::spectraSize()));
  // Reflective value and float value
  reflective_value_ = SceneValue::toFloat<Float>(settings, keyword::value);
  ZISC_ASSERT(zisc::isInClosedBounds(reflective_value_, 0.0, 1.0),
              "Texture value isn't [0, 1].");
}

} // namespace nanairo
