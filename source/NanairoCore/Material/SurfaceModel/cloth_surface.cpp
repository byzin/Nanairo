/*!
  \file cloth_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cloth_surface.hpp"
// Standard C++ library
#include <cstddef>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Bxdf/microcylinder_cloth_brdf.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ClothSurface::ClothSurface(
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  initialize(settings, texture_list);
}

/*!
  \details
  No detailed.
  */
auto ClothSurface::makeBxdf(
    const IntersectionInfo& info,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    zisc::MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto& uv = info.textureCoordinate();

  // Get the roughness
  const auto k_d = reflectance_->reflectiveValue(uv, wavelengths);

  // Make a microcylinder cloth BRDF
  using Brdf = MicrocylinderClothBrdf;
  auto chunk = memory_pool.allocate<Brdf>();
  auto ptr = makeUnique<Brdf>(chunk, this, k_d);
  return ptr;
}

/*!
  \details
  No detailed.
  */
SurfaceType ClothSurface::type() const noexcept
{
  return SurfaceType::kCloth;
}

/*!
  \details
  No detailed.
  */
void ClothSurface::initialize(
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  const auto& parameters = surface_settings->clothParameters();
  {
    const auto index = parameters.reflectance_index_;
    reflectance_ = texture_list[index];
  }
  {
    const auto& coefficients = parameters.coefficients_;

    eta_ = zisc::cast<Float>(coefficients[0]);
    ZISC_ASSERT(0.0 < eta_, "The eta is minus.");

    k_d_ = zisc::cast<Float>(coefficients[1]);
    ZISC_ASSERT(zisc::isInClosedBounds(k_d_, 0.0, 1.0),
                "The k_d is out of range [0, 1].");

    gamma_r_ = zisc::cast<Float>(coefficients[2]);
    ZISC_ASSERT(0.0 < gamma_r_, "The gamma_r is minus.");

    gamma_v_ = zisc::cast<Float>(coefficients[3]);
    ZISC_ASSERT(0.0 < gamma_v_, "The gamma_v is minus.");

    rho_ = zisc::cast<Float>(coefficients[4]);
    ZISC_ASSERT(0.0 < rho_, "The rho is minus.");
  }
}

} // namespace nanairo
