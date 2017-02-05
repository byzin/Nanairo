/*!
  \file checkerboard_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "checkerboard_texture.hpp"
// Standard C++ library
#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <vector>
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
#include "NanairoCore/Color/xyz_color.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/value.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
CheckerboardTexture::CheckerboardTexture(const System& system,
                                         const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
Float CheckerboardTexture::floatValue(const Point2& coordinate) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return float_value_[index];
}

/*!
  \details
  No detailed.
  */
SampledSpectra CheckerboardTexture::emissiveValue(
    const Point2& coordinate,
    const WavelengthSamples& wavelengths) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return sample(*emissive_value_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
Float CheckerboardTexture::reflectiveValue(const Point2& coordinate,
                                           const uint16 wavelength) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return reflective_value_[index]->getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra CheckerboardTexture::reflectiveValue(
    const Point2& coordinate,
    const WavelengthSamples& wavelengths) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return sample(*reflective_value_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
TextureType CheckerboardTexture::type() const noexcept
{
  return TextureType::Checkerboard;
}

/*!
  \details
  No detailed.
  */
void CheckerboardTexture::initialize(const System& system,
                                     const QJsonObject& settings) noexcept
{
  using zisc::cast;

  {
    const auto resolution = SceneValue::toArray(settings, keyword::imageResolution);
    const auto width = SceneValue::toInt<int>(resolution[0]);
    width_ = cast<Float>(width) - std::numeric_limits<Float>::epsilon();
    const auto height = SceneValue::toInt<int>(resolution[1]);
    height_ = cast<Float>(height) - std::numeric_limits<Float>::epsilon();
  }

  const auto color1_settings = SceneValue::toObject(settings, keyword::color1);
  const auto color2_settings = SceneValue::toObject(settings, keyword::color2);
  // Emissive values
  emissive_value_[0] = std::make_unique<SpectralDistribution>(
      SpectralDistribution::makeEmissive(system, color1_settings));
  emissive_value_[1] = std::make_unique<SpectralDistribution>(
      SpectralDistribution::makeEmissive(system, color2_settings));
  // Reflective values
  reflective_value_[0] = std::make_unique<SpectralDistribution>(
      SpectralDistribution::makeReflective(system, color1_settings));
  reflective_value_[1] = std::make_unique<SpectralDistribution>(
      SpectralDistribution::makeReflective(system, color2_settings));
  // Float values
  float_value_[0] = reflective_value_[0]->toXyzForReflector(system).y();
  float_value_[0] = zisc::clamp(float_value_[0], 0.0, 1.0);
  float_value_[1] = reflective_value_[1]->toXyzForReflector(system).y();
  float_value_[1] = zisc::clamp(float_value_[1], 0.0, 1.0);
}

} // namespace nanairo
