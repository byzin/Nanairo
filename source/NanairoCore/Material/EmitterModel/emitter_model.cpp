/*!
  \file emitter_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "emitter_model.hpp"
// Standard C++ library
#include <memory>
#include <string>
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
  emitter->setName(emitter_settings->name());
  return emitter;
}

/*!
  */
const std::string* EmitterModel::name() const noexcept
{
  const std::string* object_name = nullptr;
#ifdef Z_DEBUG_MODE
  object_name = &name_;
#endif // Z_DEBUG_MODE
  return object_name;
}

/*!
  */
void EmitterModel::setName(const std::string& name) noexcept
{
#ifdef Z_DEBUG_MODE
  name_ = name;
#endif // Z_DEBUG_MODE
  static_cast<void>(name);
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
