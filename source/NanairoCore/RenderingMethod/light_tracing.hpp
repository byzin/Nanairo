/*!
  \file light_tracing.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_TRACING_HPP
#define NANAIRO_LIGHT_TRACING_HPP

// Standard C++ library
#include <thread>
// Nanairo
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class CameraModel;
class IntersectionInfo;
class Material;
class Ray;
class Sampler;
class Scene;
template <uint> class ShaderModel;
class System;
template <typename> class UniquePointer;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class LightTracing : public RenderingMethod<kSampleSize>
{
 public:
  using Method = RenderingMethod<kSampleSize>;
  using Spectra = typename Method::Spectra;
  using Shader = ShaderModel<kSampleSize>;
  using ShaderPointer = UniquePointer<Shader>;
  using Wavelengths = typename Method::Wavelengths;


  //! Initialize light tracing method
  LightTracing(const System& system, const QJsonObject& settings) noexcept;


  //! Render scene using light tracing method
  void render(System& system,
              Scene& scene,
              const Wavelengths& sampled_wavelengths) noexcept override;

 private:
  //! Add a light contribution to buffer
  void addLightContribution(CameraModel& camera,
                            const uint x,
                            const uint y,
                            const Spectra& contribution) noexcept;

  //! Evaluate the explicit connection
  void evalExplicitConnection(const World& world,
                              const Vector3* vin,
                              const ShaderPointer& bxdf,
                              const IntersectionInfo& intersection,
                              const Spectra& light_contribution,
                              const Spectra& ray_weight,
                              CameraModel& camera,
                              MemoryPool& memory_pool) noexcept;

  //! Generate a light ray
  Ray generateRay(const World& world,
                  Spectra* light_contribution,
                  const Spectra& ray_weight,
                  CameraModel& camera,
                  Sampler& sampler,
                  MemoryPool& memory_pool) noexcept;

  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;

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
};

//! \} Core

} // namespace nanairo

#include "light_tracing-inl.hpp"

#endif // NANAIRO_LIGHT_TRACING_HPP
