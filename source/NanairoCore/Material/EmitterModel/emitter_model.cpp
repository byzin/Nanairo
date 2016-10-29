/*!
  \file emitter_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "emitter_model.hpp"
// Standard C++ library
#include <vector>
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

// Forward declaration
class TextureModel;

/*!
  */
EmitterModel::~EmitterModel() noexcept
{
}

/*!
  \details
  No detailed.
  */
EmitterModel::EmitterModel(const QJsonObject& settings) noexcept
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
UniquePointer<EmitterModel> EmitterModel::makeEmitter(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  using zisc::toHash32;

  EmitterModel* emitter = nullptr;
  const auto type = SceneValue::toString(settings, keyword::type);
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::nonDirectionalEmitter): {
    emitter = new NonDirectionalEmitter{settings, texture_list};
    break;
   }
   default: {
    zisc::raiseError("EmitterError: Unsupported type is specified.");
    break;
   }
  }
  return UniquePointer<EmitterModel>{emitter};
}

/*!
  \details
  No detailed.
  */
void EmitterModel::initialize(const QJsonObject& /* settings */) noexcept
{
}

/*!
  */
void EmitterModel::setRadiantExitance(const Float radiant_exitance) noexcept
{
  radiant_exitance_ = radiant_exitance;
}

} // namespace nanairo
