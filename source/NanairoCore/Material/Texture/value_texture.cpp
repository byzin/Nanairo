/*!
  \file value_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "value_texture.hpp"
// Standard C++ library
#include <cstddef>
// Qt
#include <QString>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "texture.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/floating_point.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ValueTexture::ValueTexture(const System& system,
                           const SceneSettings& settings,
                           const QString& prefix)
{
  initialize(system, settings, prefix);
}

/*!
  \details
  No detailed.
  */
Float ValueTexture::floatValue(const Point2& /* coordinate */) const
{
  return value_;
}

/*!
  \details
  No detailed.
  */
std::size_t ValueTexture::textureSize() const
{
  return sizeof(value_);
}

/*!
  \details
  No detailed.
  */
TextureType ValueTexture::type() const
{
  return TextureType::Value;
}


/*!
  \details
  No detailed.
  */
Float ValueTexture::wavelengthValue(const Point2& /* coordinate */, 
                                    const uint16 /* wavelength */) const
{
  return value_;
}

/*!
  \details
  No detailed.
  */
void ValueTexture::initialize(const System& /* system */,
                              const SceneSettings& settings,
                              const QString& prefix)
{
  auto p = prefix + "/" + keyword::valueTexture;
  value_ = settings.realValue(p + "/" + keyword::value);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(value_),
              "Texture value must be [0, 1].");
}

} // namespace nanairo
