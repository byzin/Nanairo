/*!
  \file smooth_diffuse_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_diffuse_surface.hpp"
// Standard C++ library
#include <vector>
// Qt
#include <QString>
// Nanairo
#include "surface_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/Texture/texture.hpp"

namespace nanairo {

// Forward declaration
class SpectralDistribution;

/*!
  \details
  No detailed.
  */
SmoothDiffuseSurface::SmoothDiffuseSurface(
    const SceneSettings& settings,
    const QString& prefix,
    const std::vector<const Texture*>& texture_list) noexcept
{
  initialize(settings, prefix, texture_list);
}

/*!
  \details
  No detailed.
  */
std::size_t SmoothDiffuseSurface::surfaceSize() const noexcept
{
  return 0;
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
    const SceneSettings& settings,
    const QString& prefix,
    const std::vector<const Texture*>& texture_list) noexcept
{
  const auto p = prefix + "/" + keyword::smoothDiffuseSurface;
  const auto key = p + "/" + keyword::reflectanceIndex;
  reflectance_ = getTexture(settings, key, texture_list);
}

} // namespace nanairo
  
