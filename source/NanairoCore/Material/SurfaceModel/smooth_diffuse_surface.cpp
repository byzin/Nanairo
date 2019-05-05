/*!
  \file smooth_diffuse_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_diffuse_surface.hpp"
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
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Material/Bxdf/lambert_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SmoothDiffuseSurface::SmoothDiffuseSurface(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto SmoothDiffuseSurface::makeBxdf(
    const IntersectionInfo& info,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    const PathState& /* path_state */,
    zisc::pmr::memory_resource* mem_resource) const noexcept -> ShaderPointer
{
  // Evaluate the reflectance
  const auto k_d = reflectance_->reflectiveValue(info.uv(), wavelengths);


  using BxdfPointer = zisc::UniqueMemoryPointer<LambertBrdf>;
  auto ptr = BxdfPointer::make(mem_resource, k_d);
  return std::move(ptr);
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothDiffuseSurface::type() const noexcept
{
  return SurfaceType::kSmoothDiffuse;
}

/*!
  \details
  No detailed.
  */
void SmoothDiffuseSurface::initialize(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->smoothDiffuseParameters();
  {
    const auto index = parameters.reflectance_index_;
    reflectance_ = texture_list[index];
  }
}

} // namespace nanairo
  
