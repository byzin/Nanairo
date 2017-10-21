/*!
  \file rough_dielectric_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rough_dielectric_surface.hpp"
// Standard C++ library
#include <cstddef>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_pool.hpp"
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Bxdf/ggx_dielectric_bsdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RoughDielectricSurface::RoughDielectricSurface(
    const SettingNodeBase* settings,
    const std::vector<TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto RoughDielectricSurface::makeBxdf(
    const IntersectionInfo& info,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    zisc::MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];

  // Evaluate the roughness
  const Float roughness = evalRoughness(roughness_, info.uv());
  // Evaluate the refractive index
  const Float n = evalRefractiveIndex(outer_refractive_index_,
                                      inner_refractive_index_,
                                      info.uv(),
                                      wavelength,
                                      info.isBackFace());


  // Make GGX BSDF
  using Bsdf = GgxDielectricBsdf;
  auto chunk = memory_pool.allocate<Bsdf>();
  ShaderPointer ptr = makeUnique<Bsdf>(chunk, roughness, n);
  return ptr;
}

/*!
  \details
  No detailed.
  */
SurfaceType RoughDielectricSurface::type() const noexcept
{
  return SurfaceType::kRoughDielectric;
}

/*!
  \details
  No detailed.
  */
void RoughDielectricSurface::initialize(
    const SettingNodeBase* settings,
    const std::vector<TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->roughDielectricParameters();
  {
    const auto index = parameters.outer_refractive_index_;
    outer_refractive_index_ = texture_list[index];
  }
  {
    const auto index = parameters.inner_refractive_index_;
    inner_refractive_index_ = texture_list[index];
  }
  {
    const auto index = parameters.roughness_index_;
    roughness_ = texture_list[index];
  }
}

} // namespace nanairo
