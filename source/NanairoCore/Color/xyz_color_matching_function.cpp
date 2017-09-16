/*!
  \file xyz_color_matching_function.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "xyz_color_matching_function.hpp"
// Standard C++ library
#include <array>
#include <memory>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/math.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "spectral_distribution.hpp"
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
XyzColorMatchingFunction::XyzColorMatchingFunction() noexcept :
    bar_{{std::make_unique<SpectralDistribution>(),
          std::make_unique<SpectralDistribution>(),
          std::make_unique<SpectralDistribution>()}}
{
  initialize();
}

/*!
  \details
  No detailed.
  */
XyzColor XyzColorMatchingFunction::toXyzForEmitter(
    const SpectralDistribution& spectra) const noexcept
{
  XyzColor xyz;
  for (uint color = 0; color < 3; ++color) {
    const auto& bar = *bar_[color];
    zisc::CompensatedSummation<Float> s{0.0};
    for (uint i = 0; i < CoreConfig::spectraSize(); ++i)
      s.add(bar[i] * spectra[i]);
    xyz[color] = s.get();
  }
  return xyz;
}

/*!
  */
XyzColor XyzColorMatchingFunction::toXyzForReflector(
    const SpectralDistribution& spectra) const noexcept
{
  XyzColor xyz;
  const Float k = zisc::invert(yBar().sum());
  for (uint color = 0; color < 3; ++color) {
    const auto& bar = *bar_[color];
    zisc::CompensatedSummation<Float> s{0.0};
    for (uint i = 0; i < CoreConfig::spectraSize(); ++i)
      s.add(bar[i] * spectra[i]);
    xyz[color] = k * s.get();
  }
  return xyz;
}

/*!
  \details
  No detailed.
  */
void XyzColorMatchingFunction::initialize() noexcept
{
  {
    auto& x_bar = xBar();
    for (uint i = 0; i < CoreConfig::spectraSize(); ++i) {
      const Float x = calcX(getWavelength(i));
      x_bar.set(i, x);
    }
  }
  {
    auto& y_bar = yBar();
    for (uint i = 0; i < CoreConfig::spectraSize(); ++i) {
      const Float y = calcY(getWavelength(i));
      y_bar.set(i, y);
    }
  }
  {
    auto& z_bar = zBar();
    for (uint i = 0; i < CoreConfig::spectraSize(); ++i) {
      const Float z = calcZ(getWavelength(i));
      z_bar.set(i, z);
    }
  }
}

/*!
  */
Float XyzColorMatchingFunction::calcValue(const Float wavelength,
                                          const Float alpha,
                                          const Float beta,
                                          const Float gamma,
                                          const Float delta) const noexcept
{
  const Float w_diff = wavelength - beta;
  const Float t = w_diff * ((w_diff < 0.0) ? gamma : delta);
  return alpha * zisc::exp(-0.5 * zisc::power<2>(t));
}

/*!
  */
Float XyzColorMatchingFunction::calcX(const Float wavelength) const noexcept
{
  constexpr uint n = 3;
  constexpr Float alpha[n] = {0.362, 1.056, -0.065};
  constexpr Float beta[n] = {442.0, 599.8, 501.1};
  constexpr Float gamma[n] = {0.0624, 0.0264, 0.0490};
  constexpr Float delta[n] = {0.0374, 0.0323, 0.0382};
  Float x = 0.0;
  for (uint i = 0; i < n; ++i)
    x += calcValue(wavelength, alpha[i], beta[i], gamma[i], delta[i]);
  return x;
}

/*!
  */
Float XyzColorMatchingFunction::calcY(const Float wavelength) const noexcept
{
  constexpr uint n = 2;
  constexpr Float alpha[n] = {0.821, 0.286};
  constexpr Float beta[n] = {568.8, 530.9};
  constexpr Float gamma[n] = {0.0213, 0.0613};
  constexpr Float delta[n] = {0.0247, 0.0322};
  Float y = 0.0;
  for (uint i = 0; i < n; ++i)
    y += calcValue(wavelength, alpha[i], beta[i], gamma[i], delta[i]);
  return y;
}

/*!
  */
Float XyzColorMatchingFunction::calcZ(const Float wavelength) const noexcept
{
  constexpr uint n = 2;
  constexpr Float alpha[n] = {1.217, 0.681};
  constexpr Float beta[n] = {437.0, 459.0};
  constexpr Float gamma[n] = {0.0845, 0.0385};
  constexpr Float delta[n] = {0.0278, 0.0725};
  Float z = 0.0;
  for (uint i = 0; i < n; ++i)
    z += calcValue(wavelength, alpha[i], beta[i], gamma[i], delta[i]);
  return z;
}

} // namespace nanairo
