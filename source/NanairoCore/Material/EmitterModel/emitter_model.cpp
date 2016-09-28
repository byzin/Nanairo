/*!
  \file emitter_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "emitter_model.hpp"
// Qt
#include <QJsonObject>
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
#include "NanairoCore/Utility/scene_value.hpp"

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
                           const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void EmitterModel::initialize(const System& /* system */,
                              const QJsonObject& /* settings */) noexcept
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
                                        const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  EmitterModel* emitter = nullptr;

  const auto type = SceneValue::toString(settings, keyword::type);
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::nonDirectionalEmitter):
    emitter = new NonDirectionalEmitter{system, settings};
    break;
   default:
    zisc::raiseError("EmitterError: Unsupported type is specified.");
    break;
  }

  return UniquePointer<EmitterModel>{emitter};
}

} // namespace nanairo
