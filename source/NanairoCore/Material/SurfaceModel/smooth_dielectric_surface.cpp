/*!
  \file smooth_dielectric_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_dielectric_surface.hpp"
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/floating_point.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SmoothDielectricSurface::SmoothDielectricSurface(const QJsonObject& settings) noexcept
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
std::size_t SmoothDielectricSurface::surfaceSize() const noexcept
{
  return sizeof(eta_);
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothDielectricSurface::type() const noexcept
{
  return SurfaceType::SmoothDielectric;
}

/*!
  \details
  No detailed.
  */
void SmoothDielectricSurface::initialize(const QJsonObject& settings) noexcept
{
  const auto outer_refractive_index_settings =
      SceneValue::toString(settings, keyword::outerRefractiveIndex);
  const auto n1 = makeSpectra(outer_refractive_index_settings);
  ZISC_ASSERT(!hasZeroFloat(n1), "The n1 contains zero value.");
  ZISC_ASSERT(!hasNegativeFloat(n1), "The n1 contains negative value.");

  const auto inner_refractive_index_settings =
      SceneValue::toString(settings, keyword::innerRefractiveIndex);
  const auto n2 = makeSpectra(inner_refractive_index_settings);
  ZISC_ASSERT(!hasNegativeFloat(n2), "The n2 contains negative value.");

  eta_ = n2 / n1;
}

} // namespace nanairo
