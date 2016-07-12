/*!
  \file rough_dielectric_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rough_dielectric_surface.hpp"
// Standard C++ library
#include <cstddef>
#include <utility>
#include <vector>
// Qt
#include <QString>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "fresnel.hpp"
#include "surface_model.hpp"
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
RoughDielectricSurface::RoughDielectricSurface(
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
std::size_t RoughDielectricSurface::surfaceSize() const noexcept
{
  return sizeof(eta_);
}

/*!
  \details
  No detailed.
  */
SurfaceType RoughDielectricSurface::type() const noexcept
{
  return SurfaceType::RoughDielectric;
}

/*!
  \details
  No detailed.
  */
void RoughDielectricSurface::initialize(
    const SceneSettings& settings,
    const QString& prefix,
    const std::vector<const Texture*>& texture_list) noexcept
{
  const auto p = prefix + "/" + keyword::roughDielectricSurface;

  auto key = p + "/" + keyword::roughnessIndex;
  roughness_ = getTexture(settings, key, texture_list);

  key = p + "/" + keyword::outerRefractiveIndex;
  const auto n1 = makeSpectra(settings, key);
  ZISC_ASSERT(!hasZeroFloat(n1), "The n1 must not contain zero.");
  ZISC_ASSERT(!hasNegativeFloat(n1), "The n1 must not contain negative.");

  key = p + "/" + keyword::innerRefractiveIndex;
  const auto n2 = makeSpectra(settings, key);
  ZISC_ASSERT(!hasNegativeFloat(n2), "The n2 must not contain negative.");

  eta_ = n2 / n1;
}

} // namespace nanairo
