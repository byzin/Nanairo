/*!
  \file texture_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "texture_model.hpp"
// Standard C++ library
#include <memory>
#include <string>
#include <string_view>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "checkerboard_texture.hpp"
#include "image_texture.hpp"
#include "unicolor_texture.hpp"
#include "value_texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/texture_setting_node.hpp"

namespace nanairo {

/*!
  */
TextureModel::~TextureModel() noexcept
{
}

/*!
  \details
  No detailed.
  */
zisc::UniqueMemoryPointer<TextureModel> TextureModel::makeTexture(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto texture_settings = castNode<TextureSettingNode>(settings);

  zisc::UniqueMemoryPointer<TextureModel> texture;
  auto& data_resource = system.dataMemoryManager();
  switch (texture_settings->textureType()) {
    case TextureType::kValue: {
      texture = zisc::UniqueMemoryPointer<ValueTexture>::make(&data_resource,
                                                              system,
                                                              settings);
      break;
    }
    case TextureType::kUnicolor: {
      texture = zisc::UniqueMemoryPointer<UnicolorTexture>::make(&data_resource,
                                                                 system,
                                                                 settings);
      break;
    }
    case TextureType::kCheckerboard: {
      texture = zisc::UniqueMemoryPointer<CheckerboardTexture>::make(&data_resource,
                                                                     system,
                                                                     settings);
      break;
    }
    case TextureType::kImage: {
      texture = zisc::UniqueMemoryPointer<ImageTexture>::make(&data_resource,
                                                              system,
                                                              settings);
      break;
    }
    default: {
      zisc::raiseError("TextureError: Unsupported type is specified.");
      break;
    }
  }
  texture->setName(texture_settings->name());
  return texture;
}

/*!
  */
std::string_view TextureModel::name() const noexcept
{
#ifdef Z_DEBUG_MODE
  return std::string_view{name_};
#else // Z_DEBUG_MODE
  return std::string_view{"TextureModel"};
#endif // Z_DEBUG_MODE
}

/*!
  */
void TextureModel::setName(const std::string_view& name) noexcept
{
#ifdef Z_DEBUG_MODE
  name_ = name;
#endif // Z_DEBUG_MODE
  static_cast<void>(name);
}

} // namespace nanairo
