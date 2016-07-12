/*!
  \file light_tracing.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_TRACING_HPP
#define NANAIRO_LIGHT_TRACING_HPP

// Standard C++ library
#include <bitset>
#include <vector>
// Nanairo
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class CameraModel;
class IntersectionInfo;
class Material;
class Ray;
class Sampler;
class SceneSettings;
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
  using Wavelengths = typename Method::Wavelengths;
  using Shader = ShaderModel<kSampleSize>;
  using ShaderPointer = UniquePointer<Shader>;


  //! Initialize light tracing method
  LightTracing(const System& system, const SceneSettings& settings) noexcept;


  //! Render scene using light tracing method
  void render(System& system,
              Scene& scene,
              const Wavelengths& sampled_wavelengths) noexcept override;

 private:
  //! Add the contributions
  void addContribution(const System& system, CameraModel& camera) noexcept;

  // Add the contribution to the light contribution buffer
  void addLightContribution(const CameraModel& camera,
                            const int thread_id, 
                            const uint x, 
                            const uint y,
                            const Spectra& contribution) noexcept;

  //! Clear the light contribution buffer
  void clearLightContribution() noexcept;

  //! Evaluate the explicit connection
  void evaluateExplicitConnection(const World& world,
                                  const CameraModel& camera,
                                  const int thread_id,
                                  const ShaderPointer& bxdf,
                                  const IntersectionInfo& intersection,
                                  const Vector3* vin,
                                  const Spectra& ray_weight,
                                  MemoryPool& memory_pool) noexcept;

  //! Generate a light ray
  Ray generateRay(const World& world,
                  const CameraModel& camera,
                  const int thread_id,
                  Sampler& sampler,
                  MemoryPool& memory_pool,
                  Spectra* weight) noexcept;

  //! Check if the buffer has contribution
  bool hasLightContribution(const uint index) const noexcept;

  //! Initialize
  void initialize(const System& system, const SceneSettings& settings) noexcept;

  //! Return the num of light rays per thread
  uint numOfThreadRays() const noexcept;

  //! Set the light contribution flag
  void setLightContributionFlag(const uint index, const bool flag) noexcept;

  //! Parallelize light tracing
  void traceLightPath(System& system,
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths) noexcept;

  //! Trace the light path
  void traceLightPath(System& system,
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths,
                      const int thread_id) noexcept;


  static constexpr uint kFlagBitsetSize = 64;

 
  std::vector<Spectra> light_contribution_buffer_;
  std::vector<std::bitset<kFlagBitsetSize>> light_contribution_flag_;
  Float ray_weight_;
  uint num_of_thread_rays_;


  //! Return the flag bitset size
  static constexpr uint flagBitsetSize() noexcept
  {
    return kFlagBitsetSize;
  }
};

//! \} Core

} // namespace nanairo

#include "light_tracing-inl.hpp"

#endif // NANAIRO_LIGHT_TRACING_HPP
