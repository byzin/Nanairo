/*!
  \file layered_diffuse_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Material/Bxdf/interfaced_lambertian_brdf.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
LayeredDiffuseSurface::LayeredDiffuseSurface(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
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
    zisc::pmr::memory_resource* mem_resource) const noexcept -> ShaderPointer
{
  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];

  // Evaluate the reflectance
  const Float k_d = reflectance_->reflectiveValue(info.uv(), wavelength);

  // Evaluate the roughness
  const Float roughness_x = evalRoughness(roughness_x_, info.uv());
  const Float roughness_y = evalRoughness(roughness_y_, info.uv());

  // Evaluate the refractive index
  const Float n = evalRefractiveIndex(outer_refractive_index_,
                                      inner_refractive_index_,
                                      info.uv(),
                                      wavelength,
                                      info.isBackFace());


  // Make a interfaced lambertian BRDF
  using BxdfPointer = zisc::UniqueMemoryPointer<InterfacedLambertianBrdf>;
  auto ptr = BxdfPointer::make(mem_resource, k_d, roughness_x, roughness_y, n, sampler);
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
  \details
  No detailed.
  */
void LayeredDiffuseSurface::initialize(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->layeredDiffuseParameters();
  {
    const auto index = parameters.reflectance_index_;
    reflectance_ = texture_list[index];
  }
  {
    const auto index = parameters.outer_refractive_index_;
    outer_refractive_index_ = texture_list[index];
  }
  {
    const auto index = parameters.inner_refractive_index_;
    inner_refractive_index_ = texture_list[index];
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
