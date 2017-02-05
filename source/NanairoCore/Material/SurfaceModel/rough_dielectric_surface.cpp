/*!
  \file rough_dielectric_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rough_dielectric_surface.hpp"
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
#include "zisc/math.hpp"
// Nanairo
#include "fresnel.hpp"
#include "surface_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Bxdf/ggx_dielectric_bsdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RoughDielectricSurface::RoughDielectricSurface(
    const QJsonObject& settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto RoughDielectricSurface::makeBxdf(
    const Point2& texture_coordinate,
    const bool is_reverse_face,
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

  // Evaluate the refractive index
  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];
  const Float n = (is_reverse_face)
      ? zisc::invert(eta_.getByWavelength(wavelength))
      : eta_.getByWavelength(wavelength);

  // Make GGX BSDF
  using Bsdf = GgxDielectricBsdf;
  auto bsdf = memory_pool.allocate<Bsdf>(roughness, n);
  return ShaderPointer{bsdf};
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

  eta_ = n2 / n1;
}

} // namespace nanairo
