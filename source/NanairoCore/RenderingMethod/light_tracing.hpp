/*!
  \file light_tracing.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_TRACING_HPP
#define NANAIRO_LIGHT_TRACING_HPP

// Standard C++ library
#include <memory>
#include <thread>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/LightSourceSampler/light_source_sampler.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

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
class LightTracing : public RenderingMethod
{
 public:
  using Method = RenderingMethod;
  using Spectra = typename Method::Spectra;
  using Shader = ShaderModel;
  using ShaderPointer = RenderingMethod::ShaderPointer;
  using Wavelengths = typename Method::Wavelengths;


  //! Initialize light tracing method
  LightTracing(System& system,
               const SettingNodeBase* settings,
               const Scene& scene) noexcept;


  //! Render scene using light tracing method
  void render(System& system,
              Scene& scene,
              const Wavelengths& sampled_wavelengths,
              const uint32 cycle) noexcept override;

 private:
  //! Add a light contribution to buffer
  void addLightContribution(CameraModel& camera,
                            const Index2d& index,
                            const Spectra& contribution) noexcept;

  //! Evaluate the explicit connection
  void evalExplicitConnection(const World& world,
                              const Vector3* vin,
                              const ShaderPointer& bxdf,
                              const IntersectionInfo& intersection,
                              const Spectra& light_contribution,
                              const Spectra& ray_weight,
                              CameraModel& camera,
                              zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Generate a light ray
  Ray generateRay(const World& world,
                  Spectra* light_contribution,
                  const Spectra& ray_weight,
                  CameraModel& camera,
                  Sampler& sampler,
                  PathState& path_state,
                  zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Initialize
  void initialize(System& system,
                  const SettingNodeBase* settings,
                  const Scene& scene) noexcept;

  //! Return the light sampler for light path
  const LightSourceSampler& lightPathLightSampler() const noexcept;

  //! Parallelize light tracing
  void traceLightPath(System& system,
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths,
                      const uint32 cycle) noexcept;

  //! Trace the light path
  void traceLightPath(System& system,
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths,
                      const uint32 cycle,
                      const uint thread_id,
                      const uint path_index) noexcept;


  std::mutex lock_;
  zisc::UniqueMemoryPointer<LightSourceSampler> light_path_light_sampler_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_LIGHT_TRACING_HPP
