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
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/texture_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
UnicolorTexture::UnicolorTexture(const System& system,
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
  return sample(*emissive_value_, wavelengths);
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
  return reflective_value_->getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra UnicolorTexture::reflectiveValue(
    const Point2& /* uv */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return sample(*reflective_value_, wavelengths);
}

/*!
  \details
  No detailed.
  */
Float UnicolorTexture::spectraValue(
    const Point2& /* uv */,
    const uint16 wavelength) const noexcept
{
  return spectra_value_->getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra UnicolorTexture::spectraValue(
    const Point2& /* uv */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return sample(*spectra_value_, wavelengths);
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
void UnicolorTexture::initialize(const System& system,
                                 const SettingNodeBase* settings) noexcept
{
  const auto texture_settings = castNode<TextureSettingNode>(settings);

  {
    const auto& parameters = texture_settings->unicolorTextureParameters();
    spectra_value_ = SpectralDistribution::makeDistribution(system,
                                                            parameters.color_.get());
    *spectra_value_ = spectra_value_->computeSystemColor(system);
  }
  {
    emissive_value_ = std::make_unique<SpectralDistribution>(
                                             spectra_value_->toEmissiveColor());
    reflective_value_ = std::make_unique<SpectralDistribution>(
                                           spectra_value_->toReflectiveColor());

    gray_scale_value_ = reflective_value_->toXyzForReflector(system).y();
    gray_scale_value_ = zisc::clamp(gray_scale_value_, 0.0, 1.0);
  }
}

} // namespace nanairo
