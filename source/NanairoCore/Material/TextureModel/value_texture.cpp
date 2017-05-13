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
#include <limits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/texture_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ValueTexture::ValueTexture(const System& system,
                           const SettingNodeBase* settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
SampledSpectra ValueTexture::emissiveValue(
    const Point2& /* uv */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return SampledSpectra{wavelengths, emissive_value_};
}

/*!
  \details
  No detailed.
  */
Float ValueTexture::grayScaleValue(const Point2& /* uv */) const noexcept
{
  return reflective_value_;
}

/*!
  \details
  No detailed.
  */
Float ValueTexture::reflectiveValue(
    const Point2& /* uv */,
    const uint16 /* wavelength */) const noexcept
{
  return reflective_value_;
}

/*!
  \details
  No detailed.
  */
SampledSpectra ValueTexture::reflectiveValue(
    const Point2& /* uv */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return SampledSpectra{wavelengths, reflective_value_};
}

/*!
  */
Float ValueTexture::spectraValue(
    const Point2& /* uv */,
    const uint16 /* wavelength */) const noexcept
{
  return spectra_value_;
}

/*!
  \details
  No detailed.
  */
SampledSpectra ValueTexture::spectraValue(
    const Point2& /* uv */,
    const WavelengthSamples& wavelengths) const noexcept
{
  return SampledSpectra{wavelengths, spectra_value_};
}

/*!
  \details
  No detailed.
  */
TextureType ValueTexture::type() const noexcept
{
  return TextureType::kValue;
}

/*!
  \details
  No detailed.
  */
void ValueTexture::initialize(const System& system,
                              const SettingNodeBase* settings) noexcept
{
  const auto texture_settings = castNode<TextureSettingNode>(settings);

  const auto& parameters = texture_settings->valueTextureParameters();
  {
    const Float value = zisc::cast<Float>(parameters.value_);
    spectra_value_ = zisc::clamp(value, 0.0, std::numeric_limits<Float>::max());
  }
  // Emissive value
  {
    emissive_value_ = system.isRgbMode()
        ? zisc::invert(3.0)
        : zisc::invert(zisc::cast<Float>(CoreConfig::spectraSize()));
  }
  // Reflective value
  {
    reflective_value_ = zisc::clamp(spectra_value_, 0.0, 1.0);
  }
}

} // namespace nanairo
