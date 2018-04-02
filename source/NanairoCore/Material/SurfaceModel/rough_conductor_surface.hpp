/*!
  \file rough_conductor_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP
#define NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Sampler;
class TextureModel;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class RoughConductorSurface : public SurfaceModel
{
 public:
  using ShaderPointer = SurfaceModel::ShaderPointer;


  //! Create a rough conductor surface
  RoughConductorSurface(
      const SettingNodeBase* settings,
      const std::vector<TextureModel*>& texture_list) noexcept;


  //! Make a GGX BRDF
  ShaderPointer makeBxdf(
      const IntersectionInfo& info,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      zisc::pmr::memory_resource* mem_resource) const noexcept override;

  //! Return the rough conductor surface type
  SurfaceType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const SettingNodeBase* settings,
                  const std::vector<TextureModel*>& texture_list) noexcept;


  const TextureModel* outer_refractive_index_;
  const TextureModel* inner_refractive_index_;
  const TextureModel* inner_extinction_;
  const TextureModel* roughness_x_;
  const TextureModel* roughness_y_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP
