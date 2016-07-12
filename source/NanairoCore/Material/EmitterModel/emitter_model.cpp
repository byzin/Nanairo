/*!
  \file emitter_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "emitter_model.hpp"
// Qt
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "non_directional_emitter.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  */
EmitterModel::~EmitterModel() noexcept
{
}

/*!
  \details
  No detailed.
  */
EmitterModel::EmitterModel(const System& system,
                           const SceneSettings& settings, 
                           const QString& prefix) noexcept
{
  initialize(system, settings, prefix);
}

/*!
  \details
  No detailed.
  */
void EmitterModel::initialize(const System& /* system */,
                              const SceneSettings& /* settings */, 
                              const QString& /* prefix */) noexcept
{
}

/*!
  \details
  No detailed.
  */
void EmitterModel::setPowerDistribution(
    const SpectralDistribution& distribution) noexcept
{
  radiant_exitance_ = distribution.sum();
  power_distribution_ = distribution;
}

/*!
  \details
  No detailed.
  */
UniquePointer<EmitterModel> makeEmitter(const System& system,
                                        const SceneSettings& settings,
                                        const QString& prefix) noexcept
{
  using zisc::toHash32;

  EmitterModel* emitter = nullptr;

  const auto type = settings.stringValue(prefix + "/" + keyword::type);
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::nonDirectionalEmitter):
    emitter = new NonDirectionalEmitter{system, settings, prefix};
    break;
   default:
    zisc::raiseError("EmitterError: Unsupported type is specified.");
    break;
  }

  return UniquePointer<EmitterModel>{emitter};
}

} // namespace nanairo
