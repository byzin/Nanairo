/*!
  \file color_matching_function.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "xyz_color_matching_function.hpp"
// Qt
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/arithmetic_array.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "spectral_distribution.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
XyzColorMatchingFunction::XyzColorMatchingFunction(const SceneSettings& settings)
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
void XyzColorMatchingFunction::initialize(const SceneSettings& settings)
{
  setStandardObserver(settings);
  setStandardIlluminant(settings);
  k_ = 1.0 / (y_bar_ * standard_illuminant_).sum();
}

/*!
  \details
  No detailed.
  */
void XyzColorMatchingFunction::setStandardIlluminant(const SceneSettings& settings)
{
  using zisc::toHash32;

  const auto key = QString(keyword::color) + "/" + keyword::standardIllumination;
  const auto standard_illuminant = settings.stringValue(key);

  switch (keyword::toHash32(standard_illuminant)) {
   case toHash32(keyword::cieD65):
    standard_illuminant_ = makeSpectra(":/spectrum/cie_si_d65.csv");
    break;
   case toHash32(keyword::cieA):
    standard_illuminant_ = makeSpectra(":/spectrum/cie_si_a.csv");
    break;
   default:
    zisc::raiseError("XyzColorMatchingFunctionError: Unsupported standard illuminant.");
    break;
  }
  standard_illuminant_ = standard_illuminant_.normalized();
}

/*!
  \details
  No detailed.
  */
void XyzColorMatchingFunction::setStandardObserver(const SceneSettings& settings)
{
  using zisc::toHash32;

  const auto key = QString{keyword::color} + "/" + keyword::standardObserver;
  const auto standard_observer = settings.stringValue(key);

  switch (keyword::toHash32(standard_observer)) {
   case toHash32(keyword::cie2Deg):
     x_bar_ = makeSpectra(":/spectrum/cie_sco_2degree_xbar.csv");
     y_bar_ = makeSpectra(":/spectrum/cie_sco_2degree_ybar.csv");
     z_bar_ = makeSpectra(":/spectrum/cie_sco_2degree_zbar.csv");
    break;
   case toHash32(keyword::cie10Deg):
     x_bar_ = makeSpectra(":/spectrum/cie_sco_10degree_xbar.csv");
     y_bar_ = makeSpectra(":/spectrum/cie_sco_10degree_ybar.csv");
     z_bar_ = makeSpectra(":/spectrum/cie_sco_10degree_zbar.csv");
    break;
   default:
    zisc::raiseError("XyzColorMatchingFunctionError: Unsupported standard observer.");
    break;
  }

//  constexpr Float k = 107.0;
//  x_bar_ = x_bar_.normalized() * k;
//  y_bar_ = y_bar_.normalized() * k;
//  z_bar_ = z_bar_.normalized() * k;
}

} // namespace nanairo
