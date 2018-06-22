/*!
  \file spectral_distribution_spectra.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectral_distribution_spectra.hpp"
// Standard C++ library
#include <array>
#include <utility>
// Zisc
#include "zisc/linear_interp.hpp"
#include "zisc/math.hpp"
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
SpectralDistributionSpectra<kCompensated>::SpectralDistributionSpectra() noexcept
{
  distribution_.fill(DataType{0.0});
}

/*!
  */
template <bool kCompensated>
SpectralDistributionSpectra<kCompensated>::SpectralDistributionSpectra(
    const DistributionType& property) noexcept :
        distribution_{property}
{
}

template <bool kCompensated>
void SpectralDistributionSpectra<kCompensated>::add(const uint index,
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
Float SpectralDistributionSpectra<kCompensated>::get(const uint index) const noexcept
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
uint SpectralDistributionSpectra<kCompensated>::getIndex(const uint16 wavelength)
    const noexcept
{
  const uint index =
      zisc::cast<uint>(wavelength - CoreConfig::shortestWavelength()) /
      CoreConfig::wavelengthResolution();
  return index;
}

/*!
  */
template <bool kCompensated>
uint16 SpectralDistributionSpectra<kCompensated>::getWavelength(const uint index)
    const noexcept
{
  const uint16 wavelength =
      CoreConfig::shortestWavelength() +
      zisc::cast<uint16>(index * CoreConfig::wavelengthResolution());
  return wavelength;
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionSpectra<kCompensated>::load(
    const System& /* system */,
    const SettingNodeBase* settings) noexcept
{
  auto work_resource = settings->workResource();
  const auto spectra_settings = castNode<SpectraSettingNode>(settings);

  ZISC_ASSERT(representationType() == spectra_settings->representationType(),
              "The representation types don't match");

  zisc::LinearInterp<Float> spectra_data{work_resource};
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
}

/*!
  */
template <bool kCompensated>
auto SpectralDistributionSpectra<kCompensated>::representationType() const noexcept
    -> RepresentationType
{
  return RepresentationType::kSpectra;
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionSpectra<kCompensated>::set(const uint index,
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
void SpectralDistributionSpectra<kCompensated>::setColor(
    const System& system,
    const SpectralDistribution& other,
    zisc::pmr::memory_resource* work_resource) noexcept
{
  switch (other.representationType()) {
   case RepresentationType::kRgb: {
    setAsSpectraColor(system, other, work_resource);
    break;
   }
   case RepresentationType::kSpectra: {
    for (uint i = 0; i < size(); ++i)
      set(i, other[i]);
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
void SpectralDistributionSpectra<kCompensated>::setData(
    const SpectralDistribution& other) noexcept
{
  switch (other.representationType()) {
   case RepresentationType::kRgb: {
    setAsSpectraData(other);
    break;
   }
   case RepresentationType::kSpectra: {
    for (uint i = 0; i < CoreConfig::spectraSize(); ++i)
      set(i, other[i]);
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
uint SpectralDistributionSpectra<kCompensated>::size() const noexcept
{
  return CoreConfig::spectraSize();
}

/*!
  */
template <bool kCompensated>
XyzColor SpectralDistributionSpectra<kCompensated>::toXyzForEmitter(
    const System& system) const noexcept
{
  const auto& cmf = system.xyzColorMatchingFunction();
  const auto xyz = cmf.toXyzForEmitter(*this);
  return xyz;
}

/*!
  */
template <bool kCompensated>
XyzColor SpectralDistributionSpectra<kCompensated>::toXyzForReflector(
    const System& system) const noexcept
{
  const auto& cmf = system.xyzColorMatchingFunction();
  const auto xyz = cmf.toXyzForReflector(*this);
  return xyz;
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionSpectra<kCompensated>::setAsSpectraColor(
    const System& system,
    const SpectralDistribution& other,
    zisc::pmr::memory_resource* work_resource) noexcept
{
  XyzColor xyz;
  {
    const RgbColor rgb{other.getByWavelength(CoreConfig::redWavelength()),
                       other.getByWavelength(CoreConfig::greenWavelength()),
                       other.getByWavelength(CoreConfig::blueWavelength())};
    const auto to_xyz_matrix = getRgbToXyzMatrix(system.colorSpace());
    xyz = ColorConversion::toXyz(rgb, to_xyz_matrix);
  }
  SpectralTransport::toSpectra(xyz, this, work_resource);
}

/*!
  */
template <bool kCompensated>
void SpectralDistributionSpectra<kCompensated>::setAsSpectraData(
    const SpectralDistribution& other) noexcept
{
  Float d = 0.0;
  for (uint i = 0; i < other.size(); ++i)
    d = other[i];
  d = d * zisc::invert(zisc::cast<Float>(other.size()));
  for (uint i = 0; i < size(); ++i)
    set(i, d);
}

// Instantiation
template class SpectralDistributionSpectra<false>;
template class SpectralDistributionSpectra<true>;

} // namespace nanairo
