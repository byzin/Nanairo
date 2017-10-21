/*!
  \file light_tracing.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_TRACING_HPP
#define NANAIRO_LIGHT_TRACING_HPP

// Standard C++ library
#include <memory>
#include <thread>
// Nanairo
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Sampling/LightSourceSampler/power_weighted_light_source_sampler.hpp"

namespace zisc {

// Forward declaration
class MemoryPool;

} // namespace zisc

namespace nanairo {

// Forward declaration
class CameraModel;
class IntersectionInfo;
class Material;
class Ray;
class Sampler;
class Scene;
class ShaderModel;
class System;
template <typename> class UniquePointer;

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
  using ShaderPointer = UniquePointer<Shader>;
  using Wavelengths = typename Method::Wavelengths;


  //! Initialize light tracing method
  LightTracing(const System& system,
               const SettingNodeBase* settings,
               const Scene& scene) noexcept;


  //! Render scene using light tracing method
  void render(System& system,
              Scene& scene,
              const Wavelengths& sampled_wavelengths) noexcept override;

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
                              zisc::MemoryPool& memory_pool) noexcept;

  //! Generate a light ray
  Ray generateRay(const World& world,
                  Spectra* light_contribution,
                  const Spectra& ray_weight,
                  CameraModel& camera,
                  Sampler& sampler,
                  zisc::MemoryPool& memory_pool) noexcept;

  //! Initialize
  void initialize(const System& system,
                  const SettingNodeBase* settings,
                  const Scene& scene) noexcept;

  //! Return the light sampler for light path
  const PowerWeightedLightSourceSampler& lightPathLightSampler() const noexcept;

  //! Parallelize light tracing
  void traceLightPath(System& system,
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths) noexcept;

  //! Trace the light path
  void traceLightPath(System& system,
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths,
                      const int thread_id) noexcept;


  std::mutex lock_;
  std::unique_ptr<PowerWeightedLightSourceSampler> light_path_light_sampler_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_LIGHT_TRACING_HPP
