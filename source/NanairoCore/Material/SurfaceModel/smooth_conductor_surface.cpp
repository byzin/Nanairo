/*!
  \file smooth_conductor_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_conductor_surface.hpp"
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
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SmoothConductorSurface::SmoothConductorSurface(const SceneSettings& settings,
                                               const QString& prefix)
{
  initialize(settings, prefix);
}

/*!
  \details
  No detailed.
  */
std::size_t SmoothConductorSurface::surfaceSize() const
{
  return sizeof(reflectance_0deg_);
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothConductorSurface::type() const
{
  return SurfaceType::SmoothConductor;
}

/*!
  \details
  No detailed.
  */
void SmoothConductorSurface::initialize(const SceneSettings& settings, 
                                        const QString& prefix)
{
  const auto p = prefix + "/" + keyword::smoothConductorSurface;

  QString key{p + "/" + keyword::outerRefractiveIndex};
  const auto n1 = makeSpectra(settings, key);
  ZISC_ASSERT(!hasZeroFloat(n1), "The n1 must not contain zero.");
  ZISC_ASSERT(!hasNegativeFloat(n1), "The n1 must not contain negative.");

  key = p + "/" + keyword::innerRefractiveIndex;
  const auto n2 = makeSpectra(settings, key);
  ZISC_ASSERT(!hasNegativeFloat(n2), "The n2 must not contain negative.");

  key = p + "/" + keyword::innerExtinction;
  const auto k2 = makeSpectra(settings, key);
  ZISC_ASSERT(!hasNegativeFloat(k2), "The k2 must not contain negative.");

  const auto eta = n2 / n1;
  const auto eta_k = k2 / n1;
  reflectance_0deg_ = calculateFresnelConductorReflectance0(eta, eta_k);
}

} // namespace nanairo
