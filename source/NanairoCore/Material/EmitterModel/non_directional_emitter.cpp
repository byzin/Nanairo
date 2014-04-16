/*!
  \file non_directional_emitter.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "non_directional_emitter.hpp"
// Qt
#include <QString>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "emitter_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
NonDirectionalEmitter::NonDirectionalEmitter(const System& system,
                                             const SceneSettings& settings,
                                             const QString& prefix) :
    EmitterModel(system, settings, prefix)
{
  initialize(system, settings, prefix);
}

/*!
  \details
  No detailed.
  */
EmitterType NonDirectionalEmitter::type() const
{
  return EmitterType::NonDirectional;
}

/*!
  \details
  No detailed.
  */
void NonDirectionalEmitter::initialize(const System& system,
                                       const SceneSettings& settings,
                                       const QString& prefix)
{
  auto p = prefix + "/" + keyword::nonDirectionalEmitter;
  auto key = p + "/" + keyword::radiantExitance;
  const Float radiant_exitance = settings.realValue(key);
  ZISC_ASSERT(0.0 < radiant_exitance, 
              "Radiance exitance must be positive.");

  key = p + "/" + keyword::color;
  auto power_distribution = 
      makeEmissiveDistribution(system, settings, key);

  const Float k = radiant_exitance / power_distribution.sum(); 
  power_distribution = power_distribution * k;
  setPowerDistribution(power_distribution);
}

} // namespace nanairo
