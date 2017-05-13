/*!
  \file checkerboard_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "checkerboard_texture.hpp"
// Standard C++ library
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <vector>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Color/xyz_color.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/texture_setting_node.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
CheckerboardTexture::CheckerboardTexture(const System& system,
                                         const SettingNodeBase* settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
SampledSpectra CheckerboardTexture::emissiveValue(
    const Point2& uv,
    const WavelengthSamples& wavelengths) const noexcept
{
  const uint index = getIndex(uv);
  return sample(*emissive_value_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
Float CheckerboardTexture::grayScaleValue(const Point2& uv) const noexcept
{
  const uint index = getIndex(uv);
  return gray_scale_value_[index];
}

/*!
  \details
  No detailed.
  */
Float CheckerboardTexture::reflectiveValue(
    const Point2& uv,
    const uint16 wavelength) const noexcept
{
  const uint index = getIndex(uv);
  return reflective_value_[index]->getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra CheckerboardTexture::reflectiveValue(
    const Point2& uv,
    const WavelengthSamples& wavelengths) const noexcept
{
  const uint index = getIndex(uv);
  return sample(*reflective_value_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
Float CheckerboardTexture::spectraValue(
    const Point2& uv,
    const uint16 wavelength) const noexcept
{
  const uint index = getIndex(uv);
  return spectra_value_[index]->getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra CheckerboardTexture::spectraValue(
    const Point2& uv,
    const WavelengthSamples& wavelengths) const noexcept
{
  const uint index = getIndex(uv);
  return sample(*spectra_value_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
TextureType CheckerboardTexture::type() const noexcept
{
  return TextureType::kCheckerboard;
}

/*!
  */
inline
uint CheckerboardTexture::getIndex(const Point2& uv) const noexcept
{
  const auto x = zisc::cast<uint>(uv[0] * resolution_[0]);
  const auto y = zisc::cast<uint>(uv[1] * resolution_[1]);
  const uint index = (x ^ y) & 1;
  return index;
}

/*!
  \details
  No detailed.
  */
void CheckerboardTexture::initialize(const System& system,
                                     const SettingNodeBase* settings) noexcept
{
  const auto texture_settings = castNode<TextureSettingNode>(settings);

  const auto& parameters = texture_settings->checkerboardTextureParameters();
  {
    resolution_[0] = zisc::cast<Float>(parameters.resolution_[0]) -
                     std::numeric_limits<Float>::epsilon();
    resolution_[1] = zisc::cast<Float>(parameters.resolution_[1]) -
                     std::numeric_limits<Float>::epsilon();
  }
  for (uint i = 0; i < 2; ++i) {
    spectra_value_[i] = SpectralDistribution::makeDistribution(system,
                                                    parameters.color_[i].get());
    *spectra_value_[i] = spectra_value_[i]->computeSystemColor(system);

    emissive_value_[i] = std::make_unique<SpectralDistribution>(
                                          spectra_value_[i]->toEmissiveColor());
    reflective_value_[i] = std::make_unique<SpectralDistribution>(
                                        spectra_value_[i]->toReflectiveColor());

    gray_scale_value_[i]= reflective_value_[i]->toXyzForReflector(system).y();
    gray_scale_value_[i]= zisc::clamp(gray_scale_value_[i], 0.0, 1.0);
  }
}

} // namespace nanairo
