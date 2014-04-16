/*!
  \file smooth_dielectric_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_dielectric_surface.hpp"
// Qt
#include <QString>
// Zisc
#include "zisc/error.hpp"
// Nanairo
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
SmoothDielectricSurface::SmoothDielectricSurface(
    const SceneSettings& settings,
    const QString& prefix)
{
  initialize(settings, prefix);
}

/*!
  \details
  No detailed.
  */
std::size_t SmoothDielectricSurface::surfaceSize() const
{
  return sizeof(eta_);
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothDielectricSurface::type() const
{
  return SurfaceType::SmoothDielectric;
}

/*!
  \details
  No detailed.
  */
void SmoothDielectricSurface::initialize(const SceneSettings& settings,
                                         const QString& prefix)
{
  const auto p = prefix + "/" + keyword::smoothDielectricSurface;

  auto key = p + "/" + keyword::outerRefractiveIndex;
  const auto n1 = makeSpectra(settings, key);
  ZISC_ASSERT(!hasZeroFloat(n1), "The n1 must not contain zero.");
  ZISC_ASSERT(!hasNegativeFloat(n1), "The n1 must not contain negative.");

  key = p + "/" + keyword::innerRefractiveIndex;
  const auto n2 = makeSpectra(settings, key);
  ZISC_ASSERT(!hasNegativeFloat(n2), "The n2 must not contain negative.");

  eta_ = n2 / n1;
}

} // namespace nanairo
