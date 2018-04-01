/*!
  \file cloth_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CLOTH_SURFACE_HPP
#define NANAIRO_CLOTH_SURFACE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
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
class ClothSurface : public SurfaceModel
{
 public:
  using ShaderPointer = SurfaceModel::ShaderPointer;


  //! Create a rough dielectric surface
  ClothSurface(
      const SettingNodeBase* settings,
      const std::vector<TextureModel*>& texture_list) noexcept;



  //! Return the eta (n2 / n1)
  Float eta() const noexcept;

  //! Return the reflection gamma width
  Float gammaR() const noexcept;

  //! Return the volume gamma width
  Float gammaV() const noexcept;

  //! Return the kd
  Float kD() const noexcept;

  //! Make a Microcylinder cloth BRDF
  ShaderPointer makeBxdf(
      const IntersectionInfo& info,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      zisc::MemoryPool& memory_pool) const noexcept override;

  //! Return the bandwidth parameter
  Float rho() const noexcept;

  //! Return the rough dielectric surface type
  SurfaceType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const SettingNodeBase* settings,
                  const std::vector<TextureModel*>& texture_list) noexcept;


  const TextureModel* reflectance_;
  Float eta_;
  Float k_d_;
  Float gamma_r_,
        gamma_v_;
  Float rho_;
};

//! \} Core

} // namespace nanairo

#include "cloth_surface-inl.hpp"

#endif // NANAIRO_CLOTH_SURFACE_HPP
