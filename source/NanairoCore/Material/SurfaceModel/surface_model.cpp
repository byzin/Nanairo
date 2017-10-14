/*!
  \file surface_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "surface_model.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "cloth_surface.hpp"
#include "layered_diffuse_surface.hpp"
#include "rough_dielectric_surface.hpp"
#include "rough_conductor_surface.hpp"
#include "smooth_conductor_surface.hpp"
#include "smooth_dielectric_surface.hpp"
#include "smooth_diffuse_surface.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class TextureModel;

/*!
  */
SurfaceModel::~SurfaceModel() noexcept
{
}

/*!
  \details
  No detailed.
  */
std::unique_ptr<SurfaceModel> SurfaceModel::makeSurface(
    const SettingNodeBase* settings,
    const std::vector<TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  std::unique_ptr<SurfaceModel> surface;
  switch (surface_settings->surfaceType()) {
   case SurfaceType::kSmoothDiffuse: {
    surface = std::make_unique<SmoothDiffuseSurface>(settings, texture_list);
    break;
   }
   case SurfaceType::kSmoothDielectric: {
    surface = std::make_unique<SmoothDielectricSurface>(settings, texture_list);
    break;
   }
   case SurfaceType::kSmoothConductor: {
    surface = std::make_unique<SmoothConductorSurface>(settings, texture_list);
    break;
   }
   case SurfaceType::kRoughDielectric: {
    surface = std::make_unique<RoughDielectricSurface>(settings, texture_list);
    break;
   }
   case SurfaceType::kRoughConductor: {
    surface = std::make_unique<RoughConductorSurface>(settings, texture_list);
    break;
   }
   case SurfaceType::kLayeredDiffuse: {
    surface = std::make_unique<LayeredDiffuseSurface>(settings, texture_list);
    break;
   }
   case SurfaceType::kCloth: {
    surface = std::make_unique<ClothSurface>(settings, texture_list);
    break;
   }
   default: {
    zisc::raiseError("SurfaceModelError: Unsupported type is specified.");
    break;
   }
  }
  surface->setName(surface_settings->name());
  return surface;
}

/*!
  */
const std::string* SurfaceModel::name() const noexcept
{
  const std::string* object_name = nullptr;
#ifdef Z_DEBUG_MODE
  object_name = &name_;
#endif // Z_DEBUG_MODE
  return object_name;
}

/*!
  */
void SurfaceModel::setName(const std::string& name) noexcept
{
#ifdef Z_DEBUG_MODE
  name_ = name;
#endif // Z_DEBUG_MODE
  static_cast<void>(name);
}

} // namespace nanairo
