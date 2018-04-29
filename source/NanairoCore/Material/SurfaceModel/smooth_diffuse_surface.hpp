/*!
  \file smooth_diffuse_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTH_DIFFUSE_SURFACE_HPP
#define NANAIRO_SMOOTH_DIFFUSE_SURFACE_HPP

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
class SmoothDiffuseSurface : public SurfaceModel
{
 public:
  using ShaderPointer = SurfaceModel::ShaderPointer;


  //! Create a smooth diffuse surface
  SmoothDiffuseSurface(
      const SettingNodeBase* settings,
      const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept;


  //! Make Lambert BRDF
  ShaderPointer makeBxdf(
      const IntersectionInfo& info,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      zisc::pmr::memory_resource* mem_resource) const noexcept override;

  //! Return the diffuse surface type
  SurfaceType type() const noexcept override;

 private:
  //! Initialize
  void initialize(
      const SettingNodeBase* settings,
      const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept;


  const TextureModel* reflectance_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SMOOTH_DIFFUSE_SURFACE_HPP
