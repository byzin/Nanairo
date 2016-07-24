/*!
  \file texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "texture.hpp"
// Qt
#include <QJsonObject>
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
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  */
Texture::~Texture() noexcept
{
}

/*!
  \details
  No detailed.
  */
UniquePointer<Texture> makeTexture(const System& system,
                                   const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  Texture* texture = nullptr;

  const auto type = stringValue(settings, keyword::type);
  switch (keyword::toHash32(type)) {
    case toHash32(keyword::valueTexture): {
      texture = new ValueTexture{system, settings};
      break;
    }
    case toHash32(keyword::unicolorTexture): {
      texture = new UnicolorTexture{system, settings};
      break;
    }
    case toHash32(keyword::checkerboardTexture): {
      texture = new CheckerboardTexture{system, settings};
      break;
    }
    case toHash32(keyword::imageTexture): {
      texture = new ImageTexture{system, settings};
      break;
    }
    default: {
      zisc::raiseError("TextureError: Unsupported type is specified.");
      break;
    }
  }

  return UniquePointer<Texture>{texture};
}

} // namespace nanairo
