/*!
  \file unicolor_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "unicolor_texture.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
UnicolorTexture::UnicolorTexture(const System& system,
                                 const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
Float UnicolorTexture::floatValue(const Point2& /* coordinate */) const noexcept
{
  return float_value_;
}

/*!
  \details
  No detailed.
  */
SampledSpectra UnicolorTexture::emissiveValue(
    const Point2& /* coordinate */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return sample(*emissive_value_, wavelengths);
}

/*!
  \details
  No detailed.
  */
Float UnicolorTexture::reflectiveValue(const Point2& /* coordinate */,
                                       const uint16 wavelength) const noexcept
{
  return reflective_value_->getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra UnicolorTexture::reflectiveValue(
    const Point2& /* coordinate */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return sample(*reflective_value_, wavelengths);
}

/*!
  \details
  No detailed.
  */
TextureType UnicolorTexture::type() const noexcept
{
  return TextureType::Unicolor;
}

/*!
  \details
  No detailed.
  */
void UnicolorTexture::initialize(const System& system,
                                 const QJsonObject& settings) noexcept
{
  const auto color_settings = SceneValue::toObject(settings, keyword::color);
  // Emissive value
  emissive_value_ = std::make_unique<SpectralDistribution>(
      SpectralDistribution::makeEmissive(system, color_settings));
  // Reflective value
  reflective_value_ = std::make_unique<SpectralDistribution>(
      SpectralDistribution::makeReflective(system, color_settings));
  // Float value
  float_value_ = reflective_value_->toXyzForReflector(system).y();
  float_value_ = zisc::clamp(float_value_, 0.0, 1.0);
}

} // namespace nanairo
