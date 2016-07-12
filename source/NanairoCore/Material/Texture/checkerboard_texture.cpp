/*!
  \file checkerboard_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "checkerboard_texture.hpp"
// Standard C++ library
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>
// Qt
#include <QString>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Color/xyz_color.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/Utility/value.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
CheckerboardTexture::CheckerboardTexture(const System& system,
                                         const SceneSettings& settings,
                                         const QString& prefix) noexcept
{
  initialize(system, settings, prefix);
}

/*!
  \details
  No detailed.
  */
Float CheckerboardTexture::floatValue(const Point2& coordinate) const noexcept
{
  using zisc::cast;

  const auto x = cast<uint>(coordinate[0] * width_);
  const auto y = cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return float_value_[index];
}

/*!
  \details
  No detailed.
  */
std::size_t CheckerboardTexture::textureSize() const noexcept
{
  return sizeof(SpectralDistribution) * 2 + sizeof(Float) * 4;
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
Float CheckerboardTexture::wavelengthValue(const Point2& coordinate, 
                                           const uint16 wavelength) const noexcept
{
  using zisc::cast;

  const auto x = cast<uint>(coordinate[0] * width_);
  const auto y = cast<uint>(coordinate[1] * height_);
  const uint index = (x ^ y) & 1;
  return spectra_value_[index].getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
void CheckerboardTexture::initialize(const System& system,
                                     const SceneSettings& settings,
                                     const QString& prefix) noexcept
{
  using zisc::cast;

  auto p = prefix + "/" + keyword::checkerboardTexture;

  const auto width = settings.intValue(p + "/" + keyword::width);
  width_ = cast<Float>(width) - std::numeric_limits<Float>::epsilon();

  const auto height = settings.intValue(p + "/" + keyword::height);
  height_ = cast<Float>(height) - std::numeric_limits<Float>::epsilon();

  auto key = p + "/" + keyword::textureColor + "1";
  spectra_value_[0] = makeReflectiveDistribution(system, settings, key);
  key = p + "/" + keyword::textureColor + "2";
  spectra_value_[1] = makeReflectiveDistribution(system, settings, key);

  float_value_[0] = spectra_value_[0].toReflectiveXyz(system).y();
  float_value_[0] = zisc::clamp(float_value_[0], 0.0, 1.0);
  float_value_[1] = spectra_value_[1].toReflectiveXyz(system).y();
  float_value_[1] = zisc::clamp(float_value_[1], 0.0, 1.0);
}

} // namespace nanairo
