/*!
  \file cloth_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cloth_surface.hpp"
// Standard C++ library
#include <cstddef>
#include <utility>
#include <vector>
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
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ClothSurface::ClothSurface(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
SurfaceType ClothSurface::type() const noexcept
{
  return SurfaceType::Cloth;
}

/*!
  \details
  No detailed.
  */
void ClothSurface::initialize(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto texture_index = SceneValue::toInt<uint>(settings,
                                                     keyword::reflectanceIndex);
  reflectance_ = texture_list[texture_index];

  eta_ = SceneValue::toFloat<Float>(settings, keyword::fabricRefractiveIndex);
  ZISC_ASSERT(0.0 < eta_, "The eta is minus.");

  k_d_ = SceneValue::toFloat<Float>(settings,
                                    keyword::isotropicScatteringCoefficient);
  ZISC_ASSERT(zisc::isInClosedBounds(k_d_, 0.0, 1.0),
              "The k_d is out of range [0, 1].");

  gamma_r_ = SceneValue::toFloat<Float>(settings,
                                        keyword::surfaceReflectanceGaussianWidth);
  ZISC_ASSERT(0.0 < gamma_r_, "The gamma_r is minus.");

  gamma_v_ = SceneValue::toFloat<Float>(settings,
                                        keyword::volumeReflectanceGaussianWidth);
  ZISC_ASSERT(0.0 < gamma_v_, "The gamma_v is minus.");

  rho_ = SceneValue::toFloat<Float>(settings, keyword::bandwidthParameter);
  ZISC_ASSERT(0.0 < rho_, "The rho is minus.");
}

} // namespace nanairo
