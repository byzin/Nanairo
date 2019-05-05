/*!
  \file spectral_distribution_rgb.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectral_distribution_rgb.hpp"
// Standard C++ library
#include <array>
#include <utility>
// Zisc
#include "zisc/linear_interp.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_conversion.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Color/rgb_color.hpp"
#include "NanairoCore/Color/spectral_transport.hpp"
#include "NanairoCore/Color/xyz_color.hpp"
#include "NanairoCore/Color/xyz_color_matching_function.hpp"
#include "NanairoCore/Setting/spectra_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  */
template <bool kCompensated>
SpectralDistributionRgb<kCompensated>::SpectralDistributionRgb() noexcept
{
  distribution_.fill(DataType{0.0});
}

/*!
  */
template <bool kCompensated>
SpectralDistributionRgb<kCompensated>::SpectralDistributionRgb(
    const DistributionType& property) noexcept :
        distribution_{property}
{
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionRgb<kCompensated>::add(const uint index,
                                                const Float intensity) noexcept
{
  if constexpr (kCompensated) {
    distribution_[index].add(intensity);
  }
  else {
    distribution_[index] += intensity;
  }
}

/*!
  */
template <bool kCompensated>
Float SpectralDistributionRgb<kCompensated>::get(const uint index) const noexcept
{
  if constexpr (kCompensated) {
    return distribution_[index].get();
  }
  else {
    return distribution_[index];
  }
}

/*!
  */
template <bool kCompensated>
uint SpectralDistributionRgb<kCompensated>::getIndex(const uint16 wavelength)
    const noexcept
{
  const uint index = (wavelength == CoreConfig::blueWavelength()) ? 0 :
                     (wavelength == CoreConfig::greenWavelength()) ? 1
                                                                   : 2;
  return index;
}

/*!
  */
template <bool kCompensated>
uint16 SpectralDistributionRgb<kCompensated>::getWavelength(const uint index)
    const noexcept
{
  constexpr std::array<uint16, 3> rgb_wavelengths{{CoreConfig::blueWavelength(),
                                                   CoreConfig::greenWavelength(),
                                                   CoreConfig::redWavelength()}};
  const uint16 wavelength = rgb_wavelengths[index];
  return wavelength;
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionRgb<kCompensated>::load(
    const System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto spectra_settings = castNode<SpectraSettingNode>(settings);

  ZISC_ASSERT(representationType() == spectra_settings->representationType(),
              "The representation types don't match");

  RgbColor rgb;
  {
    const auto& parameters = spectra_settings->rgbParameters();
    rgb[0] = zisc::cast<Float>(parameters.rgb_[0]);
    rgb[1] = zisc::cast<Float>(parameters.rgb_[1]);
    rgb[2] = zisc::cast<Float>(parameters.rgb_[2]);
    rgb.correctGamma(system.gamma());
  }
  {
    setByWavelength(CoreConfig::blueWavelength(), rgb.blue());
    setByWavelength(CoreConfig::greenWavelength(), rgb.green());
    setByWavelength(CoreConfig::redWavelength(), rgb.red());
  }
}

/*!
  */
template <bool kCompensated>
auto SpectralDistributionRgb<kCompensated>::representationType() const noexcept
    -> RepresentationType
{
  return RepresentationType::kRgb;
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionRgb<kCompensated>::set(const uint index,
                                                const Float intensity) noexcept
{
  if constexpr (kCompensated) {
    distribution_[index].set(intensity);
  }
  else {
    distribution_[index] = intensity;
  }
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionRgb<kCompensated>::setColor(
    const System& system,
    const SpectralDistribution& other,
    zisc::pmr::memory_resource* /* work_resource */) noexcept
{
  switch (other.representationType()) {
   case RepresentationType::kRgb: {
    for (uint i = 0; i < size(); ++i)
      set(i, other[i]);
    break;
   }
   case RepresentationType::kSpectra: {
    setAsRgbColor(system, other);
    break;
   }
   default: {
    zisc::raiseError("SpectralDistributionError: Unsupported representation type.");
    break;
   }
  }
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionRgb<kCompensated>::setData(
    const SpectralDistribution& other) noexcept
{
  switch (other.representationType()) {
   case RepresentationType::kRgb: {
    for (uint i = 0; i < size(); ++i)
      set(i, other[i]);
    break;
   }
   case RepresentationType::kSpectra: {
    setAsRgbData(other);
    break;
   }
   default: {
    zisc::raiseError("SpectralDistributionError: Unsupported representation type.");
    break;
   }
  }
}

/*!
  */
template <bool kCompensated>
uint SpectralDistributionRgb<kCompensated>::size() const noexcept
{
  return 3;
}

/*!
  */
template <bool kCompensated>
XyzColor SpectralDistributionRgb<kCompensated>::toXyzForEmitter(
    const System& system) const noexcept
{
  const RgbColor rgb{getByWavelength(CoreConfig::redWavelength()),
                     getByWavelength(CoreConfig::greenWavelength()),
                     getByWavelength(CoreConfig::blueWavelength())};
  const auto xyz = ColorConversion::toXyz(rgb,
                                          getRgbToXyzMatrix(system.colorSpace()));
  return xyz;
}

/*!
  */
template <bool kCompensated>
XyzColor SpectralDistributionRgb<kCompensated>::toXyzForReflector(
    const System& system) const noexcept
{
  return toXyzForEmitter(system);
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionRgb<kCompensated>::setAsRgbColor(
    const System& system,
    const SpectralDistribution& other) noexcept
{
  RgbColor rgb;
  {
    const auto& cmf = system.xyzColorMatchingFunction();
    const auto xyz = cmf.toXyzForReflector(other);
    rgb = ColorConversion::toRgb(xyz, getXyzToRgbMatrix(system.colorSpace()));
  }
  setByWavelength(CoreConfig::redWavelength(), rgb.red());
  setByWavelength(CoreConfig::greenWavelength(), rgb.green());
  setByWavelength(CoreConfig::blueWavelength(), rgb.blue());
}

template <bool kCompensated>
void SpectralDistributionRgb<kCompensated>::setAsRgbData(
    const SpectralDistribution& other) noexcept
{
  constexpr std::array<uint16, 3> rgb_wavelengths{{CoreConfig::blueWavelength(),
                                                   CoreConfig::greenWavelength(),
                                                   CoreConfig::redWavelength()}};
  for (const auto wavelength : rgb_wavelengths)
    setByWavelength(wavelength, other.getByWavelength(wavelength));
}

// Instantiation
template class SpectralDistributionRgb<false>;
template class SpectralDistributionRgb<true>;

} // namespace nanairo
