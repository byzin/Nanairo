/*!
  \file non_directional_emitter.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "non_directional_emitter.hpp"
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "emitter_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
NonDirectionalEmitter::NonDirectionalEmitter(const System& system,
                                             const QJsonObject& settings) noexcept
    : EmitterModel(system, settings)
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
EmitterType NonDirectionalEmitter::type() const noexcept
{
  return EmitterType::NonDirectional;
}

/*!
  \details
  No detailed.
  */
void NonDirectionalEmitter::initialize(const System& system,
                                       const QJsonObject& settings) noexcept
{
  const Float radiant_exitance = floatValue<Float>(settings,
                                                   keyword::radiantExitance);
  ZISC_ASSERT(0.0 < radiant_exitance, "Radiance exitance is negative.");

  const auto color_setting = objectValue(settings, keyword::color);
  auto power_distribution = makeEmissiveDistribution(system, color_setting);

  const Float k = radiant_exitance / power_distribution.sum(); 
  power_distribution = power_distribution * k;
  setPowerDistribution(power_distribution);
}

} // namespace nanairo
