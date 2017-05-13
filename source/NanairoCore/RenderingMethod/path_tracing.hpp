/*!
  \file path_tracing.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PATH_TRACING_HPP
#define NANAIRO_PATH_TRACING_HPP

// Nanairo
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

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
class PathTracing : public RenderingMethod
{
 public:
  using Method = RenderingMethod;
  using Spectra = typename Method::Spectra;
  using Shader = ShaderModel;
  using ShaderPointer = UniquePointer<Shader>;
  using Wavelengths = typename Method::Wavelengths;


  //! Initialize path tracing method
  PathTracing(const System& system, const SettingNodeBase* settings) noexcept;


  //! Render scene using path tracing method
  void render(System& system,
              Scene& scene,
              const Wavelengths& sampled_wavelengths) noexcept override;

 private:
  //! Evaluate the explicit connection
  void evalExplicitConnection(const World& world,
                              const Ray& ray,
                              const ShaderPointer& bxdf,
                              const IntersectionInfo& intersection,
                              const Spectra& camera_contribution,
                              const Spectra& ray_weight,
                              Sampler& sampler,
                              MemoryPool& memory_pool,
                              Spectra* contribution) const noexcept;

  //! Evaluate the implicit connection
  void evalImplicitConnection(const World& world,
                              const Ray& ray,
                              const Float inverse_direction_pdf,
                              const IntersectionInfo& intersection,
                              const Spectra& camera_contribution,
                              const Spectra& ray_weight,
                              const bool mis,
                              MemoryPool& memory_pool,
                              Spectra* contribution) const noexcept;

  //! Check if the explicit connection is enabled
  static constexpr bool explicitConnectionIsEnabled() noexcept;

  //! Check if the implicit connection is enabled
  static constexpr bool implicitConnectionIsEnabled() noexcept;

  //! Generate a camera ray
  Ray generateRay(const CameraModel& camera,
                  const uint x,
                  const uint y,
                  Sampler& sampler,
                  MemoryPool& memory_pool,
                  Spectra* ray_weight,
                  Float* inverse_direction_pdf) const noexcept;

  //! Initialize
  void initialize(const SettingNodeBase* settings) noexcept;

  //! Calculate the MIS weight
  Float calcMisWeight(const Float pdf1, const Float inverse_pdf2) const noexcept;

  //! Parallelize path tracing
  void traceCameraPath(System& system,
                       Scene& scene,
                       const Wavelengths& sampled_wavelengths) noexcept;

  //! Trace the camera path
  void traceCameraPath(System& system,
                       Scene& scene,
                       const Wavelengths& sampled_wavelengths,
                       const int thread_id,
                       const uint x,
                       const uint y) noexcept;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_PATH_TRACING_HPP
