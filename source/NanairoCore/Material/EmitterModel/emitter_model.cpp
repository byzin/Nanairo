/*!
  \file emitter_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "emitter_model.hpp"
// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_pool.hpp"
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
std::unique_ptr<EmitterModel> EmitterModel::makeEmitter(
    const SettingNodeBase* settings,
    const std::vector<TextureModel*>& texture_list) noexcept
{
  const auto emitter_settings = castNode<EmitterSettingNode>(settings);

  std::unique_ptr<EmitterModel> emitter;
  switch (emitter_settings->emitterType()) {
   case EmitterType::kNonDirectional: {
    emitter = std::make_unique<NonDirectionalEmitter>(settings, texture_list);
    break;
   }
   default: {
    zisc::raiseError("EmitterError: Unsupported type is specified.");
    break;
   }
  }
  return emitter;
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
