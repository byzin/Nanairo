/*!
  \file emitter_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "emitter_model.hpp"
// Standard C++ library
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "non_directional_emitter.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
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
zisc::UniqueMemoryPointer<EmitterModel> EmitterModel::makeEmitter(
    System& system,
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  const auto emitter_settings = castNode<EmitterSettingNode>(settings);

  zisc::UniqueMemoryPointer<EmitterModel> emitter;
  auto& data_resource = system.dataMemoryManager();
  switch (emitter_settings->emitterType()) {
   case EmitterType::kNonDirectional: {
    emitter = zisc::UniqueMemoryPointer<NonDirectionalEmitter>::make(&data_resource,
                                                                     settings,
                                                                     texture_list);
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
std::string_view EmitterModel::name() const noexcept
{
#ifdef Z_DEBUG_MODE
  return std::string_view{name_};
#else // Z_DEBUG_MODE
  return std::string_view{"EmitterModel"};
#endif // Z_DEBUG_MODE
}

/*!
  */
void EmitterModel::setName(const std::string_view& name) noexcept
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
