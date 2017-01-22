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
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/error.hpp"
// Qt
#include <QJsonObject>
#include <QString>
// Nanairo
#include "surface_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/Bxdf/lambert_brdf.hpp"
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
auto SmoothDiffuseSurface::makeBxdf(
    const Point2& texture_coordinate,
    const bool /* is_reverse_face */,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto reflectance = reflectance_->reflectiveValue(texture_coordinate,
                                                         wavelengths);
  ZISC_ASSERT(reflectance.isAllInClosedBounds(0.0, 1.0),
              "Reflectances aren't [0, 1].");

  using Brdf = LambertBrdf;
  auto brdf = memory_pool.allocate<Brdf>(reflectance);
  return ShaderPointer{brdf};
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
  
