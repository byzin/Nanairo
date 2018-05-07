/*!
  \file surface_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SURFACE_MODEL_HPP
#define NANAIRO_SURFACE_MODEL_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Sampler;
class ShaderModel;
class System;
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
  kSmoothDiffuse               = zisc::Fnv1aHash32::hash("SmoothDiffuse"),
  kSmoothDielectric            = zisc::Fnv1aHash32::hash("SmoothDielectric"),
  kSmoothConductor             = zisc::Fnv1aHash32::hash("SmoothConductor"),
  kRoughDielectric             = zisc::Fnv1aHash32::hash("RoughDielectric"),
  kRoughConductor              = zisc::Fnv1aHash32::hash("RoughConductor"),
  kLayeredDiffuse              = zisc::Fnv1aHash32::hash("LayeredDiffuse"),
  kCloth                       = zisc::Fnv1aHash32::hash("Cloth")
};

/*!
  \details
  No detailed.
  */
class SurfaceModel
{
 public:
  using ShaderPointer = zisc::UniqueMemoryPointer<ShaderModel>;


  //! Finalize the surface model
  virtual ~SurfaceModel() noexcept;


  //! Make BxDF
  virtual ShaderPointer makeBxdf(
      const IntersectionInfo& info,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      zisc::pmr::memory_resource* mem_resource) const noexcept = 0;

  //! Make a surface scattering model
  static zisc::UniqueMemoryPointer<SurfaceModel> makeSurface(
      System& system,
      const SettingNodeBase* settings,
      const zisc::pmr::vector<const TextureModel*>& texture_list) noexcept;

  //! Return the surface name
  std::string_view name() const noexcept;

  //! Set the surface name
  void setName(const std::string_view& name) noexcept;

  //! Return the surface type
  virtual SurfaceType type() const noexcept = 0;

 protected:
  // Evaluate the refractive index
  static Float evalRefractiveIndex(
      const TextureModel* outer_refractive_index_texture,
      const TextureModel* inner_refractive_index_texture,
      const Point2& uv,
      const uint16 wavelength,
      const bool is_back_face) noexcept;

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

 private:
#ifdef Z_DEBUG_MODE
  std::string name_;
#endif // Z_DEBUG_MODE
};

//! \} Core

} // namespace nanairo

#include "surface_model-inl.hpp"

#endif // NANAIRO_SURFACE_MODEL_HPP
