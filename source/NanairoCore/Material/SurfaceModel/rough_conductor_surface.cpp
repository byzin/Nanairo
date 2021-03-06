/*!
  \file rough_conductor_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Reflect
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Material/Bxdf/ggx_conductor_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RoughConductorSurface::RoughConductorSurface(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
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
    const PathState& /* path_state */,
    zisc::pmr::memory_resource* mem_resource) const noexcept -> ShaderPointer
{
  // Evaluate the roughness
  const Float roughness_x = evalRoughness(roughness_x_, info.uv());
  const Float roughness_y = evalRoughness(roughness_y_, info.uv());

  // Evaluate the refractive index
  const auto n = evalRefractiveIndex(outer_refractive_index_,
                                     inner_refractive_index_,
                                     info.uv(),
                                     wavelengths);
  const auto eta = evalRefractiveIndex(outer_refractive_index_,
                                       inner_extinction_,
                                       info.uv(),
                                       wavelengths);


  // Make GGX BRDF
  using BxdfPointer = zisc::UniqueMemoryPointer<GgxConductorBrdf>;
  auto ptr = BxdfPointer::make(mem_resource, roughness_x, roughness_y, n, eta);
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
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
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
    const auto index = parameters.roughness_x_index_;
    roughness_x_ = texture_list[index];
  }
  {
    const auto index = (parameters.anisotropic_ == kTrue)
        ? parameters.roughness_y_index_
        : parameters.roughness_x_index_;
    roughness_y_ = texture_list[index];
  }
}

} // namespace nanairo
