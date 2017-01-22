/*!
  \file rough_conductor_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rough_conductor_surface.hpp"
// Standard C++ library
#include <cstddef>
#include <utility>
#include <vector>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/error.hpp"
// Reflect
#include "fresnel.hpp"
#include "surface_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Bxdf/ggx_conductor_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RoughConductorSurface::RoughConductorSurface(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

 /*!
  \details
  No detailed.
  */
auto RoughConductorSurface::makeBxdf(
    const Point2& texture_coordinate,
    const bool /* is_reverse_face */,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  // Get the roughness
  constexpr Float min_roughness = 0.001;
  Float roughness = roughness_->floatValue(texture_coordinate);
  roughness = (min_roughness < roughness)
      ? roughness * roughness
      : min_roughness * min_roughness;
  ZISC_ASSERT(zisc::isInClosedBounds(roughness, 0.0, 1.0),
              "The roughness is out of the range [0, 1].");

  const auto fresnel_0deg = sample(fresnel_0deg_, wavelengths);

  // Make GGX BRDF
  using Brdf = GgxConductorBrdf;
  auto brdf = memory_pool.allocate<Brdf>(roughness, fresnel_0deg);
  return ShaderPointer{brdf};
}

/*!
  \details
  No detailed.
  */
SurfaceType RoughConductorSurface::type() const noexcept
{
  return SurfaceType::RoughConductor;
}

/*!
  \details
  No detailed.
  */
void RoughConductorSurface::initialize(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto texture_index = SceneValue::toInt<uint>(settings,
                                                     keyword::roughnessIndex);
  roughness_ = texture_list[texture_index];

  const auto outer_refractive_index_settings =
      SceneValue::toString(settings, keyword::outerRefractiveIndex);
  const auto n1 = SpectralDistribution::makeSpectra(outer_refractive_index_settings);
  ZISC_ASSERT(!n1.hasValue(0.0), "The n1 contains zero value.");
  ZISC_ASSERT(!n1.hasNegative(), "The n1 contains negative value.");

  const auto inner_refractive_index_settings =
      SceneValue::toString(settings, keyword::innerRefractiveIndex);
  const auto n2 = SpectralDistribution::makeSpectra(inner_refractive_index_settings);
  ZISC_ASSERT(!n2.hasNegative(), "The n2 contains negative value.");

  const auto inner_extinction_settings =
      SceneValue::toString(settings, keyword::innerExtinction);
  const auto k2 = SpectralDistribution::makeSpectra(inner_extinction_settings);
  ZISC_ASSERT(!k2.hasNegative(), "The k2 contains negative value.");

  const auto eta = n2 / n1;
  const auto eta_k = k2 / n1;
  fresnel_0deg_ = Fresnel::evalFresnel0(eta, eta_k);
}

} // namespace nanairo
