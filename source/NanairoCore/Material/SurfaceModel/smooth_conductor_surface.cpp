/*!
  \file smooth_conductor_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_conductor_surface.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Bxdf/specular_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SmoothConductorSurface::SmoothConductorSurface(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto SmoothConductorSurface::makeBxdf(
    const IntersectionInfo& info,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    zisc::pmr::memory_resource* mem_resource) const noexcept -> ShaderPointer
{
  // Evaluate the refractive index
  const auto n = evalRefractiveIndex(outer_refractive_index_,
                                     inner_refractive_index_,
                                     info.uv(),
                                     wavelengths);
  const auto eta = evalRefractiveIndex(outer_refractive_index_,
                                       inner_extinction_,
                                       info.uv(),
                                       wavelengths);


  using BxdfPointer = zisc::UniqueMemoryPointer<SpecularBrdf>;
  auto ptr = BxdfPointer::make(mem_resource, n, eta);
  return ptr;
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothConductorSurface::type() const noexcept
{
  return SurfaceType::kSmoothConductor;
}

/*!
  \details
  No detailed.
  */
void SmoothConductorSurface::initialize(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->smoothConductorParameters();
  {
    const auto index = parameters.outer_refractive_index_;
    outer_refractive_index_ =texture_list[index];
  }
  {
    const auto index = parameters.inner_refractive_index_;
    inner_refractive_index_ =texture_list[index];
  }
  {
    const auto index = parameters.inner_extinction_index_;
    inner_extinction_ = texture_list[index];
  }
}

} // namespace nanairo
