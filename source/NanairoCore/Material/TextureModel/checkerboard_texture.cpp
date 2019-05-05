/*!
  \file checkerboard_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/xyz_color.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"
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
CheckerboardTexture::CheckerboardTexture(System& system,
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
  const Float scale = emissive_scale_[index];
  const auto e = sample(*color_value_[index], wavelengths) * scale;
  return e;
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
  auto r = color_value_[index]->getByWavelength(wavelength);
  r = zisc::clamp(r, 0.0, 1.0);
  return r;
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
  auto r = sample(*color_value_[index], wavelengths);
  r.clampAll(0.0, 1.0);
  return r;
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
  return data_value_[index]->getByWavelength(wavelength);
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
  return sample(*data_value_[index], wavelengths);
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
void CheckerboardTexture::initialize(System& system,
                                     const SettingNodeBase* settings) noexcept
{
  const auto texture_settings = castNode<TextureSettingNode>(settings);
  const auto& parameters = texture_settings->checkerboardTextureParameters();

  for (std::size_t i = 0; i < 2; ++i) {
    resolution_[i] = zisc::cast<Float>(parameters.resolution_[i]);
    resolution_[i] = std::nextafter(resolution_[i], 0.0);
  }
  for (std::size_t i = 0; i < 2; ++i) {
    {
      const auto color_settings = parameters.color_[i].get();
      auto data_resource = &system.dataMemoryManager();
      auto work_resource = settings->workResource();
      const auto d = SpectralDistribution::makeDistribution(system,
                                                            color_settings,
                                                            work_resource);
      data_value_[i] = SpectralDistribution::makeDistribution(system.colorMode(),
                                                              data_resource);
      data_value_[i]->setData(*d);
      color_value_[i] = SpectralDistribution::makeDistribution(system.colorMode(),
                                                               data_resource);
      color_value_[i]->setColor(system, *d, work_resource);
    }

    emissive_scale_[i] = zisc::invert(color_value_[i]->compensatedSum());
    gray_scale_value_[i] = color_value_[i]->toXyzForReflector(system).y();
    gray_scale_value_[i]= zisc::clamp(gray_scale_value_[i], 0.0, 1.0);
  }
}

} // namespace nanairo
