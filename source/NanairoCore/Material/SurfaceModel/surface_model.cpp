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
UniquePointer<SurfaceModel> SurfaceModel::makeSurface(
    const SettingNodeBase* settings,
    const std::vector<const TextureModel*>& texture_list) noexcept
{
  const auto surface_settings = castNode<SurfaceSettingNode>(settings);

  SurfaceModel* surface = nullptr;
  switch (surface_settings->surfaceType()) {
   case SurfaceType::kSmoothDiffuse: {
    surface = new SmoothDiffuseSurface{settings, texture_list};
    break;
   }
   case SurfaceType::kSmoothDielectric: {
    surface = new SmoothDielectricSurface{settings, texture_list};
    break;
   }
   case SurfaceType::kSmoothConductor: {
    surface = new SmoothConductorSurface{settings, texture_list};
    break;
   }
   case SurfaceType::kRoughDielectric: {
    surface = new RoughDielectricSurface{settings, texture_list};
    break;
   }
   case SurfaceType::kRoughConductor: {
    surface = new RoughConductorSurface{settings, texture_list};
    break;
   }
   case SurfaceType::kLayeredDiffuse: {
    surface = new LayeredDiffuseSurface{settings, texture_list};
    break;
   }
   case SurfaceType::kCloth: {
    surface = new ClothSurface{settings, texture_list};
    break;
   }
   default: {
    zisc::raiseError("SurfaceModelError: Unsupported type is specified.");
    break;
   }
  }
  return UniquePointer<SurfaceModel>{surface};
}

} // namespace nanairo
