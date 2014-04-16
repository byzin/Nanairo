/*!
  \file texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "texture.hpp"
// Qt
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "checkerboard_texture.hpp"
#include "image_texture.hpp"
#include "unicolor_texture.hpp"
#include "value_texture.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
UniquePointer<Texture> makeTexture(const System& system,
                                   const SceneSettings& settings,
                                   const QString& prefix)
{
  using zisc::toHash32;

  Texture* texture = nullptr;

  const auto type = settings.stringValue(prefix + "/" + keyword::type);
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::valueTexture):
    texture = new ValueTexture{system, settings, prefix};
    break;
   case toHash32(keyword::unicolorTexture):
    texture = new UnicolorTexture{system, settings, prefix};
    break;
   case toHash32(keyword::checkerboardTexture):
    texture = new CheckerboardTexture{system, settings, prefix};
    break;
   case toHash32(keyword::imageTexture):
    texture = new ImageTexture{system, settings, prefix};
    break;
   default:
    zisc::raiseError("TextureError: Unsupported type is specified.");
    break;
  }

  return UniquePointer<Texture>{texture};
}

//! Get a texture
const Texture* getTexture(const SceneSettings& settings,
                          const QString& key,
                          const std::vector<const Texture*>& texture_list)
{
  using zisc::cast;

  const uint index = cast<uint>(settings.intValue(key));
  return texture_list[index];
}

} // namespace nanairo
