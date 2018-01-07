/*!
  \file layered_diffuse_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LAYERED_DIFFUSE_SURFACE_HPP
#define NANAIRO_LAYERED_DIFFUSE_SURFACE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace zisc {

// Forward declaration
class MemoryPool;

} // namespace zisc

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
class LayeredDiffuseSurface : public SurfaceModel
{
 public:
  using ShaderPointer = SurfaceModel::ShaderPointer;


  //! Create a rough dielectric surface
  LayeredDiffuseSurface(
      const SettingNodeBase* settings,
      const std::vector<TextureModel*>& texture_list) noexcept;


  //! Make a interfaced lambertian BRDF
  ShaderPointer makeBxdf(
      const IntersectionInfo& info,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      zisc::MemoryPool& memory_pool) const noexcept override;

  //! Return the rough dielectric surface type
  SurfaceType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const SettingNodeBase* settings,
                  const std::vector<TextureModel*>& texture_list) noexcept;


  const TextureModel* reflectance_;
  const TextureModel* outer_refractive_index_;
  const TextureModel* inner_refractive_index_;
  const TextureModel* roughness_x_;
  const TextureModel* roughness_y_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_LAYERED_DIFFUSE_SURFACE_HPP
