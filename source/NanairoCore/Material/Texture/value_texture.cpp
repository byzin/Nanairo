/*!
  \file value_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "value_texture.hpp"
// Standard C++ library
#include <cstddef>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "texture.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/floating_point.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace {

template <typename FloatingPoint>
FloatingPoint fValue(const QJsonObject& object, const QString key) noexcept;

//!
template <typename FloatingPoint>
FloatingPoint fValue(const QJsonObject& object, const QString key) noexcept
{
  return nanairo::floatValue<FloatingPoint>(object, key);
}

} // namespace


namespace nanairo {

/*!
  \details
  No detailed.
  */
ValueTexture::ValueTexture(const System& system,
                           const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
Float ValueTexture::floatValue(const Point2& /* coordinate */) const noexcept
{
  return value_;
}

/*!
  \details
  No detailed.
  */
std::size_t ValueTexture::textureSize() const noexcept
{
  return sizeof(value_);
}

/*!
  \details
  No detailed.
  */
TextureType ValueTexture::type() const noexcept
{
  return TextureType::Value;
}


/*!
  \details
  No detailed.
  */
Float ValueTexture::wavelengthValue(const Point2& /* coordinate */, 
                                    const uint16 /* wavelength */) const noexcept
{
  return value_;
}

/*!
  \details
  No detailed.
  */
void ValueTexture::initialize(const System& /* system */,
                              const QJsonObject& settings) noexcept
{
  value_ = ::fValue<Float>(settings, keyword::value);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(value_), "Texture value isn't [0, 1].");
}

} // namespace nanairo
