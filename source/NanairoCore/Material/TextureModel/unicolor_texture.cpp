/*!
  \file unicolor_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "unicolor_texture.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/texture_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
UnicolorTexture::UnicolorTexture(System& system,
                                 const SettingNodeBase* settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
SampledSpectra UnicolorTexture::emissiveValue(
    const Point2& /* uv */,
    const WavelengthSamples& wavelengths) const noexcept
{
  const auto e = sample(*color_value_, wavelengths) * emissive_scale_;
  return e;
}

/*!
  \details
  No detailed.
  */
Float UnicolorTexture::grayScaleValue(const Point2& /* uv */) const noexcept
{
  return gray_scale_value_;
}

/*!
  \details
  No detailed.
  */
Float UnicolorTexture::reflectiveValue(
    const Point2& /* uv */,
    const uint16 wavelength) const noexcept
{
  Float r = color_value_->getByWavelength(wavelength);
  r = zisc::clamp(r, 0.0, 1.0);
  return r;
}

/*!
  \details
  No detailed.
  */
SampledSpectra UnicolorTexture::reflectiveValue(
    const Point2& /* uv */,
    const WavelengthSamples& wavelengths) const noexcept
{
  auto r = sample(*color_value_, wavelengths);
  r.clampAll(0.0, 1.0);
  return r;
}

/*!
  \details
  No detailed.
  */
Float UnicolorTexture::spectraValue(
    const Point2& /* uv */,
    const uint16 wavelength) const noexcept
{
  return data_value_->getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra UnicolorTexture::spectraValue(
    const Point2& /* uv */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return sample(*data_value_, wavelengths);
}

/*!
  \details
  No detailed.
  */
TextureType UnicolorTexture::type() const noexcept
{
  return TextureType::kUnicolor;
}

/*!
  \details
  No detailed.
  */
void UnicolorTexture::initialize(System& system,
                                 const SettingNodeBase* settings) noexcept
{
  const auto texture_settings = castNode<TextureSettingNode>(settings);
  const auto& parameters = texture_settings->unicolorTextureParameters();

  {
    const auto color_setttings = parameters.color_.get();
    auto data_resource = &system.dataMemoryManager();
    auto work_resource = settings->workResource();

    const auto d = SpectralDistribution::makeDistribution(system,
                                                          color_setttings,
                                                          work_resource);
    data_value_ = SpectralDistribution::makeDistribution(system.colorMode(),
                                                         data_resource);
    color_value_ = SpectralDistribution::makeDistribution(system.colorMode(),
                                                          data_resource);
    data_value_->setData(*d);
    color_value_->setColor(system, *d, work_resource);
  }

  emissive_scale_ = zisc::invert(color_value_->compensatedSum());
  gray_scale_value_ = color_value_->toXyzForReflector(system).y();
  gray_scale_value_ = zisc::clamp(gray_scale_value_, 0.0, 1.0);
}

} // namespace nanairo
