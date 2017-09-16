/*!
  \file spectral_distribution.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectral_distribution.hpp"
// Standard C++ library
#include <cmath>
#include <limits>
#include <memory>
#include <utility>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/linear_interp.hpp"
#include "zisc/math.hpp"
#include "zisc/matrix.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "color_conversion.hpp"
#include "color_space.hpp"
#include "rgb_color.hpp"
#include "spectral_transport.hpp"
#include "xyz_color.hpp"
#include "xyz_color_matching_function.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Setting/spectra_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
void SpectralDistribution::clampAll(const Float min, const Float max) noexcept
{
  distribution_.clampAll(min, max);
}

/*!
  \details
  No detailed.
  */
void SpectralDistribution::correctGamma(const Float gamma) noexcept
{
  for (uint index = 0; index < size(); ++index)
    distribution_[index] = zisc::pow(distribution_[index], gamma);
}

/*!
  */
bool SpectralDistribution::isRgbDistribution() const noexcept
{
  constexpr uint16 blue_w = CoreConfig::blueWavelength();
  constexpr uint16 green_w = CoreConfig::greenWavelength();
  constexpr uint16 red_w = CoreConfig::redWavelength();

  bool is_rgb = true;
  for (uint index = 0; is_rgb && (index < CoreConfig::spectraSize()); ++index) {
    const uint16 lambda = getWavelength(index);
    is_rgb = ((lambda == blue_w) || (lambda == green_w) || (lambda == red_w))
      ? (is_rgb && (0.0 < get(index)))
      : (is_rgb && (get(index) == 0.0));
  }
  return is_rgb;
}

/*!
  */
SpectralDistribution SpectralDistribution::computeSystemColor(const System& system)
    const noexcept
{
  SpectralDistribution distribution = *this;
  // RGB
  if (isRgbDistribution()) {
    if (system.colorMode() == RenderingColorMode::kSpectra)
      distribution = distribution.toSpectraColor(system);
  }
  // Spectra
  else {
    if (system.colorMode() == RenderingColorMode::kRgb)
      distribution = distribution.toRgbSpectraColor(system);
  }
  return distribution;
}

/*!
  */
std::unique_ptr<SpectralDistribution> SpectralDistribution::makeDistribution(
    const System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto spectra_settings = castNode<SpectraSettingNode>(settings);

  auto distribution = std::make_unique<SpectralDistribution>();
  switch (spectra_settings->representationType()) {
   case ColorRepresentationType::kRgb: {
    distribution->loadRgb(system, settings);
    break;
   }
   case ColorRepresentationType::kSpectra: {
    distribution->loadSpectra(settings);
    break;
   }
   default: {
    zisc::raiseError("ColorError: Unsupported representatino type is specified.");
    break;
   }
  }
  return distribution;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::toEmissiveColor() const noexcept
{
  return normalized();
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::toReflectiveColor() const noexcept
{
  auto distribution = *this;
  distribution.clampAll(0.0, 1.0);
  return distribution;
}

/*!
  \details
  No detailed.
  */
void SpectralDistribution::loadRgb(const System& system,
                                   const SettingNodeBase* settings) noexcept
{
  const auto spectra_settings = castNode<SpectraSettingNode>(settings);

  RgbColor rgb;
  {
    const auto& parameters = spectra_settings->rgbParameters();
    rgb[0] = zisc::cast<Float>(parameters.rgb_[0]);
    rgb[1] = zisc::cast<Float>(parameters.rgb_[1]);
    rgb[2] = zisc::cast<Float>(parameters.rgb_[2]);
    rgb.correctGamma(system.gamma());
    rgb.clampAll(std::numeric_limits<Float>::epsilon(), rgb.max());
  }
  {
    setByWavelength(CoreConfig::blueWavelength(), rgb.blue());
    setByWavelength(CoreConfig::greenWavelength(), rgb.green());
    setByWavelength(CoreConfig::redWavelength(), rgb.red());
  }
}

/*!
  */
void SpectralDistribution::loadSpectra(const SettingNodeBase* settings) noexcept
{
  const auto spectra_settings = castNode<SpectraSettingNode>(settings);

  zisc::LinearInterp<Float> spectra_data;
  {
    const auto& parameters = spectra_settings->spectraParameters();
    for (const auto& wavelength_data : parameters.spectra_) {
      spectra_data.add(zisc::cast<Float>(std::get<0>(wavelength_data)),
                       zisc::cast<Float>(std::get<1>(wavelength_data)));
    }
  }
  for (uint index = 0; index < CoreConfig::spectraSize(); ++index) {
    const Float lambda = zisc::cast<Float>(getWavelength(index));
    set(index, spectra_data(lambda));
  }
  clampAll(std::numeric_limits<Float>::epsilon(), max());
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::toSpectraColor(const System& system)
    const noexcept
{
  const RgbColor rgb{getByWavelength(CoreConfig::redWavelength()),
                     getByWavelength(CoreConfig::greenWavelength()),
                     getByWavelength(CoreConfig::blueWavelength())};
  const auto to_xyz_matrix = getRgbToXyzMatrix(system.colorSpace());
  const auto xyz = ColorConversion::toXyz(rgb, to_xyz_matrix);

  auto spectra = SpectralTransport::toSpectra(xyz);
  spectra.clampAll(std::numeric_limits<Float>::epsilon(), max());
  return spectra;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::toRgbSpectraColor(const System& system)
    const noexcept
{
  const auto xyz = toXyz<ColorType::kReflective>(system, *this);
  const auto to_rgb_matrix = getXyzToRgbMatrix(system.colorSpace());
  auto rgb = ColorConversion::toRgb(xyz, to_rgb_matrix);
  rgb.clampAll(std::numeric_limits<Float>::epsilon(), rgb.max());

  SpectralDistribution spectra;
  spectra.setByWavelength(CoreConfig::blueWavelength(), rgb.blue());
  spectra.setByWavelength(CoreConfig::greenWavelength(), rgb.green());
  spectra.setByWavelength(CoreConfig::redWavelength(), rgb.red());
  return spectra;
}

/*!
  */
XyzColor SpectralDistribution::toXyzForEmitter(const System& system) const noexcept
{
  return toXyz<ColorType::kEmissive>(system);
}

/*!
  */
XyzColor SpectralDistribution::toXyzForReflector(const System& system) const noexcept
{
  return toXyz<ColorType::kReflective>(system);
}

/*!
  \details
  No detailed.
  */
template <SpectralDistribution::ColorType type>
XyzColor SpectralDistribution::toXyz(const System& system,
                                     const SpectralDistribution& spectra) noexcept
{
  const auto& cmf = system.xyzColorMatchingFunction();
  const auto xyz = (type == ColorType::kEmissive)
      ? cmf.toXyzForEmitter(spectra)
      : cmf.toXyzForReflector(spectra);
  return xyz;
}

/*!
  \details
  No detailed.
  */
template <SpectralDistribution::ColorType type>
XyzColor SpectralDistribution::toXyz(const System& system) const noexcept
{
  XyzColor xyz;
  switch (system.colorMode()) {
   case RenderingColorMode::kRgb: {
    const RgbColor rgb{getByWavelength(CoreConfig::redWavelength()),
                       getByWavelength(CoreConfig::greenWavelength()),
                       getByWavelength(CoreConfig::blueWavelength())};
    xyz = ColorConversion::toXyz(rgb, getRgbToXyzMatrix(system.colorSpace()));
    break;
   }
   case RenderingColorMode::kSpectra: {
    xyz = toXyz<type>(system, *this);
    break;
   }
   default: {
    break;
   }
  }
  return xyz;
}

} // namespace nanairo
