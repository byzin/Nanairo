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
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Bxdf/specular_bsdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
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
auto SmoothDielectricSurface::makeBxdf(
    const Point2& /* texture_coordinate */,
    const bool is_reverse_face,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];
  const Float n = (is_reverse_face)
      ? zisc::invert(eta_.getByWavelength(wavelength))
      : eta_.getByWavelength(wavelength);

  using Bsdf = SpecularBsdf;
  auto bsdf = memory_pool.allocate<Bsdf>(n);
  return ShaderPointer{bsdf};
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
