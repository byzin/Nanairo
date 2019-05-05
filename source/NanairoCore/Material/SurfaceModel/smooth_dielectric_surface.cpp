/*!
  \file smooth_dielectric_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_dielectric_surface.hpp"
// Standard C++ library
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
#include "NanairoCore/Material/Bxdf/specular_bsdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SmoothDielectricSurface::SmoothDielectricSurface(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto SmoothDielectricSurface::makeBxdf(
    const IntersectionInfo& info,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    const PathState& /* path_state */,
    zisc::pmr::memory_resource* mem_resource) const noexcept -> ShaderPointer
{
  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];

  // Evaluate the refractive index
  const Float n = evalRefractiveIndex(outer_refractive_index_,
                                      inner_refractive_index_,
                                      info.uv(),
                                      wavelength,
                                      info.isBackFace());


  using BxdfPointer = zisc::UniqueMemoryPointer<SpecularBsdf>;
  auto ptr = BxdfPointer::make(mem_resource, n);
  return std::move(ptr);
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothDielectricSurface::type() const noexcept
{
  return SurfaceType::kSmoothDielectric;
}

/*!
  \details
  No detailed.
  */
void SmoothDielectricSurface::initialize(
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->smoothDielectricParameters();
  {
    const auto index = parameters.outer_refractive_index_;
    outer_refractive_index_ = texture_list[index];
  }
  {
    const auto index = parameters.inner_refractive_index_;
    inner_refractive_index_ = texture_list[index];
  }
}

} // namespace nanairo
