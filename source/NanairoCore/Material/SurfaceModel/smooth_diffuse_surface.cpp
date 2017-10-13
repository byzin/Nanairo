/*!
  \file smooth_diffuse_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_diffuse_surface.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_pool.hpp"
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Material/Bxdf/lambert_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SmoothDiffuseSurface::SmoothDiffuseSurface(
    const SettingNodeBase* settings,
    const std::vector<TextureModel*>& texture_list) noexcept
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
    zisc::MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto& uv = info.textureCoordinate();

  // Evaluate the reflectance
  const auto k_d = reflectance_->reflectiveValue(uv, wavelengths);

  using Brdf = LambertBrdf;
  auto chunk = memory_pool.allocate<Brdf>();
  ShaderPointer ptr = makeUnique<Brdf>(chunk, k_d);
  return ptr;
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
    const std::vector<TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->smoothDiffuseParameters();
  {
    const auto index = parameters.reflectance_index_;
    reflectance_ = texture_list[index];
  }
}

} // namespace nanairo
  
