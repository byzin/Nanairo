/*!
  \file surface_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SURFACE_MODEL_HPP
#define NANAIRO_SURFACE_MODEL_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace zisc {

class MemoryPool;

} // namespace zisc

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Sampler;
class ShaderModel;
class TextureModel;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
enum class SurfaceType : uint32
{
  kSmoothDiffuse               = zisc::toHash32("SmoothDiffuse"),
  kSmoothDielectric            = zisc::toHash32("SmoothDielectric"),
  kSmoothConductor             = zisc::toHash32("SmoothConductor"),
  kRoughDielectric             = zisc::toHash32("RoughDielectric"),
  kRoughConductor              = zisc::toHash32("RoughConductor"),
  kLayeredDiffuse              = zisc::toHash32("LayeredDiffuse"),
  kCloth                       = zisc::toHash32("Cloth")
};

/*!
  \details
  No detailed.
  */
class SurfaceModel
{
 public:
  using ShaderPointer = UniquePointer<ShaderModel>;


  //! Finalize the surface model
  virtual ~SurfaceModel() noexcept;


  //! Make BxDF
  virtual ShaderPointer makeBxdf(
      const IntersectionInfo& info,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      zisc::MemoryPool& memory_pool) const noexcept = 0;

  //! Make a surface scattering model
  static std::unique_ptr<SurfaceModel> makeSurface(
      const SettingNodeBase* settings,
      const std::vector<TextureModel*>& texture_list) noexcept;

  //! Return the surface type
  virtual SurfaceType type() const noexcept = 0;

 protected:
  // Evaluate the refractive index
  static Float evalRefractiveIndex(
      const TextureModel* outer_refractive_index_texture,
      const TextureModel* inner_refractive_index_texture,
      const Point2& uv,
      const uint16 wavelength,
      const bool is_reverse_face) noexcept;

  // Evaluate the refractive index
  static SampledSpectra evalRefractiveIndex(
      const TextureModel* outer_refractive_index_texture,
      const TextureModel* inner_refractive_index_texture,
      const Point2& uv,
      const WavelengthSamples& wavelengths) noexcept;

  //! Evaluate the roughness
  static Float evalRoughness(
      const TextureModel* roughness_texture,
      const Point2& uv) noexcept;
};

//! \} Core

} // namespace nanairo

#include "surface_model-inl.hpp"

#endif // NANAIRO_SURFACE_MODEL_HPP
