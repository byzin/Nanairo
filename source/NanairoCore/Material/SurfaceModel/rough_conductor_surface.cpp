/*!
  \file rough_conductor_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rough_conductor_surface.hpp"
// Standard C++ library
#include <cstddef>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_pool.hpp"
#include "zisc/utility.hpp"
// Reflect
#include "fresnel.hpp"
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Bxdf/ggx_conductor_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RoughConductorSurface::RoughConductorSurface(
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

 /*!
  \details
  No detailed.
  */
auto RoughConductorSurface::makeBxdf(
    const IntersectionInfo& info,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    zisc::MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto& uv = info.textureCoordinate();

  // Evaluate the roughness
  const Float roughness = evalRoughness(roughness_, uv);
  // Evaluate the refractive index
  const auto n = evalRefractiveIndex(outer_refractive_index_,
                                     inner_refractive_index_,
                                     uv,
                                     wavelengths);
  const auto eta = evalRefractiveIndex(outer_refractive_index_,
                                       inner_extinction_,
                                       uv,
                                       wavelengths);

  // Make GGX BRDF
  using Brdf = GgxConductorBrdf;
  auto chunk = memory_pool.allocate<Brdf>();
  auto ptr = makeUnique<Brdf>(chunk, roughness, n, eta);
  return ptr;
}

/*!
  \details
  No detailed.
  */
SurfaceType RoughConductorSurface::type() const noexcept
{
  return SurfaceType::kRoughConductor;
}

/*!
  \details
  No detailed.
  */
void RoughConductorSurface::initialize(
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->roughConductorParameters();
  {
    const auto index = parameters.outer_refractive_index_;
    outer_refractive_index_ = texture_list[index];
  }
  {
    const auto index = parameters.inner_refractive_index_;
    inner_refractive_index_ = texture_list[index];
  }
  {
    const auto index = parameters.inner_extinction_index_;
    inner_extinction_ = texture_list[index];
  }
  {
    const auto index = parameters.roughness_index_;
    roughness_ = texture_list[index];
  }
}

} // namespace nanairo
