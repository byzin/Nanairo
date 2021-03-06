/*!
  \file path_tracing.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PATH_TRACING_HPP
#define NANAIRO_PATH_TRACING_HPP

// Standard C++ library
#include <memory>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Sampling/LightSourceSampler/light_source_sampler.hpp"

namespace nanairo {

// Forward declaration
class CameraModel;
class IntersectionInfo;
class Material;
class PathState;
class Ray;
class Sampler;
class Scene;
class ShaderModel;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class PathTracing : public RenderingMethod
{
 public:
  using Method = RenderingMethod;
  using Spectra = typename Method::Spectra;
  using Shader = ShaderModel;
  using ShaderPointer = RenderingMethod::ShaderPointer;
  using Wavelengths = typename Method::Wavelengths;


  //! Initialize path tracing method
  PathTracing(System& system,
              const SettingNodeBase* settings,
              const Scene& scene) noexcept;


  //! Calculate the MIS weight
  static Float calcMisWeight(const Float pdf1, const Float inverse_pdf2) noexcept;

  //! Generate a camera ray
  static Ray generateRay(const CameraModel& camera,
                         const Index2d& pixel_index,
                         Sampler& sampler,
                         PathState& path_state,
                         zisc::pmr::memory_resource* mem_resource,
                         Spectra* ray_weight,
                         Float* inverse_direction_pdf) noexcept;

  //! Render scene using path tracing method
  void render(System& system,
              Scene& scene,
              const Wavelengths& sampled_wavelengths,
              const uint32 cycle) noexcept override;

 private:
  //! Evaluate the explicit connection
  void evalExplicitConnection(
      const World& world,
      const Ray& ray,
      const ShaderPointer& bxdf,
      const IntersectionInfo& intersection,
      const Spectra& camera_contribution,
      const Spectra& ray_weight,
      const bool emplicit_connection_is_enabled,
      const bool implicit_connection_is_enabled,
      Sampler& sampler,
      PathState& path_state,
      zisc::pmr::memory_resource* mem_resource,
      Spectra* contribution) const noexcept;

  //! Evaluate the implicit connection
  void evalImplicitConnection(
      const World& world,
      const Ray& ray,
      const Float inverse_direction_pdf,
      const IntersectionInfo& intersection,
      const Spectra& camera_contribution,
      const Spectra& ray_weight,
      const bool implicit_connection_is_enabled,
      const bool emplicit_connection_is_enabled,
      zisc::pmr::memory_resource* mem_resource,
      Spectra* contribution) const noexcept;

  //! Return the light source sampler for eye path
  const LightSourceSampler& eyePathLightSampler() const noexcept;

  //! Initialize
  void initialize(System& system,
                  const SettingNodeBase* settings,
                  const Scene& scene) noexcept;

  //! Parallelize path tracing
  void traceCameraPath(System& system,
                       Scene& scene,
                       const Wavelengths& sampled_wavelengths,
                       const uint32 cycle) noexcept;

  //! Trace the camera path
  void traceCameraPath(System& system,
                       Scene& scene,
                       const Wavelengths& sampled_wavelengths,
                       const uint32 cycle,
                       const uint thread_id,
                       const Index2d& pixel_index) noexcept;


  zisc::UniqueMemoryPointer<LightSourceSampler> eye_path_light_sampler_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_PATH_TRACING_HPP
