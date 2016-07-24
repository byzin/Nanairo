/*!
  \file smooth_conductor_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_conductor_surface.hpp"
// Qt
#include <QJsonObject>
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
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SmoothConductorSurface::SmoothConductorSurface(const QJsonObject& settings) noexcept
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
std::size_t SmoothConductorSurface::surfaceSize() const noexcept
{
  return sizeof(reflectance_0deg_);
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothConductorSurface::type() const noexcept
{
  return SurfaceType::SmoothConductor;
}

/*!
  \details
  No detailed.
  */
void SmoothConductorSurface::initialize(const QJsonObject& settings) noexcept
{
  const auto outer_refractive_index_settings =
      stringValue(settings, keyword::outerRefractiveIndex);
  const auto n1 = makeSpectra(outer_refractive_index_settings);
  ZISC_ASSERT(!hasZeroFloat(n1), "The n1 contains zero value.");
  ZISC_ASSERT(!hasNegativeFloat(n1), "The n1 contains negative value.");

  const auto inner_refractive_index_settings =
      stringValue(settings, keyword::innerRefractiveIndex);
  const auto n2 = makeSpectra(inner_refractive_index_settings);
  ZISC_ASSERT(!hasNegativeFloat(n2), "The n2 contains negative value.");

  const auto inner_extinction_settings =
      stringValue(settings, keyword::innerExtinction);
  const auto k2 = makeSpectra(inner_extinction_settings);
  ZISC_ASSERT(!hasNegativeFloat(k2), "The k2 contains negative value.");

  const auto eta = n2 / n1;
  const auto eta_k = k2 / n1;
  reflectance_0deg_ = calculateFresnelConductorReflectance0(eta, eta_k);
}

} // namespace nanairo
