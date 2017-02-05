/*!
  \file xyz_color_matching_function.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "xyz_color_matching_function.hpp"
// Standard C++ library
#include <memory>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/arithmetic_array.hpp"
#include "zisc/math.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "spectral_distribution.hpp"
#include "spectral_transport.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
XyzColorMatchingFunction::XyzColorMatchingFunction(const QJsonObject& settings) noexcept :
    illuminant_{std::make_unique<SpectralDistribution>()},
    bar_{{std::make_unique<SpectralDistribution>(),
          std::make_unique<SpectralDistribution>(),
          std::make_unique<SpectralDistribution>()}}
{
  initialize(settings);
}

/*!
  */
XyzColor XyzColorMatchingFunction::toXyzForReflector(
    const SpectralDistribution& spectra) const noexcept
{
  XyzColor xyz;
  const auto& illum = illuminant();
  const Float k = 1.0 / (illum * yBar()).sum();
  for (uint color = 0; color < 3; ++color) {
    const auto& bar = *bar_[color];
    zisc::CompensatedSummation<Float> s{0.0};
    for (uint i = 0; i < CoreConfig::spectraSize(); ++i)
      s.add(illum[i] * bar[i] * spectra[i]);
    xyz[color] = k * s.get();
  }
  return xyz;
}

/*!
  \details
  No detailed.
  */
void XyzColorMatchingFunction::initialize(const QJsonObject& settings) noexcept
{
  loadStandardIlluminant(settings);
  loadStandardObserver(settings);
}

/*!
  \details
  No detailed.
  */
void XyzColorMatchingFunction::loadStandardIlluminant(
    const QJsonObject& settings) const noexcept
{
  using zisc::toHash32;

  const auto illuminant_name = SceneValue::toString(settings,
                                                    keyword::standardIllumination);
  switch (keyword::toHash32(illuminant_name)) {
   case toHash32(keyword::cieD65): {
    *illuminant_ = SpectralDistribution::makeSpectra(":/spectrum/illuminant/cie_si_d65.csv");
    break;
   }
   case toHash32(keyword::cieA): {
    *illuminant_ = SpectralDistribution::makeSpectra(":/spectrum/illuminant/cie_si_a.csv");
    break;
   }
   default: {
    zisc::raiseError("XyzColorMatchingFunctionError: Unsupported standard illuminant.");
    break;
   }
  }
}

/*!
  \details
  No detailed.
  */
void XyzColorMatchingFunction::loadStandardObserver(
    const QJsonObject& settings) const noexcept
{
  using zisc::toHash32;

  const auto observer_name = SceneValue::toString(settings,
                                                  keyword::standardObserver);
  switch (keyword::toHash32(observer_name)) {
   case toHash32(keyword::cie2Deg): {
    *bar_[0] = SpectralDistribution::makeSpectra(":/spectrum/observer/cie_sco_2degree_xbar.csv");
    *bar_[1] = SpectralDistribution::makeSpectra(":/spectrum/observer/cie_sco_2degree_ybar.csv");
    *bar_[2] = SpectralDistribution::makeSpectra(":/spectrum/observer/cie_sco_2degree_zbar.csv");
    break;
   }
   case toHash32(keyword::cie10Deg): {
    *bar_[0] = SpectralDistribution::makeSpectra(":/spectrum/observer/cie_sco_10degree_xbar.csv");
    *bar_[1] = SpectralDistribution::makeSpectra(":/spectrum/observer/cie_sco_10degree_ybar.csv");
    *bar_[2] = SpectralDistribution::makeSpectra(":/spectrum/observer/cie_sco_10degree_zbar.csv");
    break;
   }
   default: {
    zisc::raiseError("XyzColorMatchingFunctionError: Unsupported standard observer.");
    break;
   }
  }
}

} // namespace nanairo
