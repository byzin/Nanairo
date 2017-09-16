/*!
  \file emitter_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "emitter_model.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "non_directional_emitter.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Setting/emitter_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

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
EmitterModel::EmitterModel(const SettingNodeBase* settings) noexcept
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
UniquePointer<EmitterModel> EmitterModel::makeEmitter(
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  ZISC_ASSERT(settings != nullptr, "The setting node is null.");
  ZISC_ASSERT(settings->type() == SettingNodeType::kEmitter,
              "Wrong setting node is specified.");
  const auto emitter_settings = zisc::cast<const EmitterSettingNode*>(settings);

  EmitterModel* emitter = nullptr;
  switch (emitter_settings->emitterType()) {
   case EmitterType::kNonDirectional: {
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
void EmitterModel::initialize(const SettingNodeBase* /* settings */) noexcept
{
}

/*!
  */
void EmitterModel::setRadiantExitance(const Float radiant_exitance) noexcept
{
  radiant_exitance_ = radiant_exitance;
}

} // namespace nanairo
