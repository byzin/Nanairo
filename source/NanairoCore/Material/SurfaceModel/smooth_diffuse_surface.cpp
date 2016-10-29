/*!
  \file smooth_diffuse_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_diffuse_surface.hpp"
// Standard C++ library
#include <vector>
// Qt
#include <QJsonObject>
#include <QString>
// Nanairo
#include "surface_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

// Forward declaration
class SpectralDistribution;

/*!
  \details
  No detailed.
  */
SmoothDiffuseSurface::SmoothDiffuseSurface(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothDiffuseSurface::type() const noexcept
{
  return SurfaceType::SmoothDiffuse;
}

/*!
  \details
  No detailed.
  */
void SmoothDiffuseSurface::initialize(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto texture_index = SceneValue::toInt<uint>(settings,
                                                     keyword::reflectanceIndex);
  reflectance_ = texture_list[texture_index];
}

} // namespace nanairo
  
