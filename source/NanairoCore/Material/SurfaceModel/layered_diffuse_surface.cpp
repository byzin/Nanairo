/*!
  \file layered_diffuse_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "layered_diffuse_surface.hpp"
// Standard C++ library
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_pool.hpp"
// Nanairo
#include "fresnel.hpp"
#include "layered_diffuse.hpp"
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Material/Bxdf/interfaced_lambertian_brdf.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
LayeredDiffuseSurface::LayeredDiffuseSurface(
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto LayeredDiffuseSurface::makeBxdf(
    const IntersectionInfo& info,
    const WavelengthSamples& wavelengths,
    Sampler& sampler,
    zisc::MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto& uv = info.textureCoordinate();
  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];

  // Evaluate the reflectance
  const Float k_d = reflectance_->reflectiveValue(uv, wavelength);
  // Evaluate the roughness
  const Float roughness = evalRoughness(roughness_, uv);
  // Evaluate the refractive index
  const Float n = evalRefractiveIndex(outer_refractive_index_,
                                      inner_refractive_index_,
                                      uv,
                                      wavelength,
                                      info.isReverseFace());
  const Float ri = ri_.getByWavelength(wavelength);

  // Make a interfaced lambertian BRDF
  using Brdf = InterfacedLambertianBrdf;
  auto chunk = memory_pool.allocate<Brdf>();
  ShaderPointer ptr = makeUnique<Brdf>(chunk, k_d, roughness, n, ri, sampler);
  return ptr;
}


/*!
  \details
  No detailed.
  */
SurfaceType LayeredDiffuseSurface::type() const noexcept
{
  return SurfaceType::kLayeredDiffuse;
}

/*!
  */
void LayeredDiffuseSurface::calcInternalReflectance() noexcept
{
  for (uint i = 0; i < CoreConfig::spectraSize(); ++i) {
    const Point2 uv{0.0, 0.0};
    const auto wavelength = getWavelength(i);
    Float n = 0.0;
    {
      const auto n1 = outer_refractive_index_->spectraValue(uv, wavelength);
      const auto n2 = inner_refractive_index_->spectraValue(uv, wavelength);
      n = n2 / n1;
    }
    ri_[i] = LayeredDiffuse::calcRi(n);
  }
}

/*!
  \details
  No detailed.
  */
void LayeredDiffuseSurface::initialize(
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->layeredDiffuseParameters();
  {
    const auto index = parameters.outer_refractive_index_;
    outer_refractive_index_ = texture_list[index];
  }
  {
    const auto index = parameters.inner_refractive_index_;
    inner_refractive_index_ = texture_list[index];
  }
  {
    const auto index = parameters.reflectance_index_;
    reflectance_ = texture_list[index];
  }
  {
    const auto index = parameters.roughness_index_;
    roughness_ = texture_list[index];
  }
  calcInternalReflectance();
}

} // namespace nanairo
