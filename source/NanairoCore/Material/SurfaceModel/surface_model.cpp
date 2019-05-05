/*!
  \file surface_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "surface_model.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
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
#include "NanairoCore/system.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"

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
zisc::UniqueMemoryPointer<SurfaceModel> SurfaceModel::makeSurface(
    System& system,
    const SettingNodeBase* settings,
    const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  zisc::UniqueMemoryPointer<SurfaceModel> surface;
  auto& data_resource = system.dataMemoryManager();
  switch (surface_settings->surfaceType()) {
   case SurfaceType::kSmoothDiffuse: {
    surface =
        zisc::UniqueMemoryPointer<SmoothDiffuseSurface>::make(&data_resource,
                                                              settings,
                                                              texture_list);
    break;
   }
   case SurfaceType::kSmoothDielectric: {
    surface =
        zisc::UniqueMemoryPointer<SmoothDielectricSurface>::make(&data_resource,
                                                                 settings,
                                                                 texture_list);
    break;
   }
   case SurfaceType::kSmoothConductor: {
    surface =
        zisc::UniqueMemoryPointer<SmoothConductorSurface>::make(&data_resource,
                                                                settings,
                                                                texture_list);
    break;
   }
   case SurfaceType::kRoughDielectric: {
    surface =
        zisc::UniqueMemoryPointer<RoughDielectricSurface>::make(&data_resource,
                                                                settings,
                                                                texture_list);
    break;
   }
   case SurfaceType::kRoughConductor: {
    surface =
        zisc::UniqueMemoryPointer<RoughConductorSurface>::make(&data_resource,
                                                               settings,
                                                               texture_list);
    break;
   }
   case SurfaceType::kLayeredDiffuse: {
    surface =
        zisc::UniqueMemoryPointer<LayeredDiffuseSurface>::make(&data_resource,
                                                               settings,
                                                               texture_list);
    break;
   }
   case SurfaceType::kCloth: {
    surface =
        zisc::UniqueMemoryPointer<ClothSurface>::make(&data_resource,
                                                      settings,
                                                      texture_list);
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
std::string_view SurfaceModel::name() const noexcept
{
#ifdef Z_DEBUG_MODE
  return std::string_view{name_};
#else // Z_DEBUG_MODE
  return std::string_view{"SurfaceModel"};
#endif // Z_DEBUG_MODE
}

/*!
  */
void SurfaceModel::setName(const std::string_view& name) noexcept
{
#ifdef Z_DEBUG_MODE
  name_ = name;
#endif // Z_DEBUG_MODE
  static_cast<void>(name);
}

} // namespace nanairo
