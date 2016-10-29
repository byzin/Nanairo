/*!
  \file probabilistic_ppm.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PROBABILISTIC_PPM_HPP
#define NANAIRO_PROBABILISTIC_PPM_HPP

// Standard C++ library
#include <vector>
// Nanairo
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/DataStructure/knn_photon_list.hpp"
#include "NanairoCore/DataStructure/photon_map.hpp"

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
class World;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class ProbabilisticPpm : public RenderingMethod<kSampleSize>
{
 public:
  using Method = RenderingMethod<kSampleSize>;
  using Map = PhotonMap<kSampleSize>;
  using Photon = Ray;
  using Spectra = typename Method::Spectra;
  using Shader = ShaderModel<kSampleSize>;
  using ShaderPointer = UniquePointer<Shader>;
  using Wavelengths = typename Method::Wavelengths;


  //! Initialize probabilistic ppm method
  ProbabilisticPpm(const System& system, const QJsonObject& settings) noexcept;


  //! Render scene using probabilistic ppm method
  void render(System& system,
              Scene& scene,
              const Wavelengths& sampled_wavelengths) noexcept override;

 private:
  //! Calculate the photon weight using the smoothing kernel
  Float calcWeight(const Float t) const noexcept;

  //! Estimate the radiance
  Spectra estimateRadiance(const int thread_id,
                           const Vector3& vin,
                           const IntersectionInfo& intersection,
                           const ShaderPointer& bxdf,
                           const WavelengthSamples<kSampleSize>& wavelengths,
                           const bool wavelength_is_selected) noexcept;

  //! Evaluate implicit connection
  void evalImplicitConnection(const Ray& ray,
                              const IntersectionInfo& intersection,
                              const Spectra& camera_contribution,
                              const Spectra& ray_weight,
                              MemoryPool& memory_pool,
                              Spectra* contribution) const noexcept;

  //! Return the expected max reflection count
  static constexpr uint expectedMaxReflectionCount() noexcept;

  //! Generate a photon
  Photon generatePhoton(const World& world,
                        Spectra* light_contribution,
                        Sampler& sampler,
                        MemoryPool& memory_pool) const noexcept;

  //! Generate a camera ray
  Ray generateRay(const CameraModel& camera,
                  const uint x,
                  const uint y,
                  Sampler& sampler,
                  MemoryPool& memory_pool,
                  Spectra* weight) const noexcept;

  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;

  //! Sample next photon
  Ray sampleNextRay(const Ray& ray,
                    const ShaderPointer& bxdf,
                    const IntersectionInfo& intersection,
                    Sampler& sampler,
                    Spectra* photon_energy,
                    bool* wavelength_is_selected) const noexcept;

  //! Check if the surface has the photon map
  bool surfaceHasPhotonMap(const ShaderPointer& bxdf) const noexcept;

  //! Trace camera path
  void traceCameraPath(System& system,
                       Scene& scene,
                       const Wavelengths& wavelengths) noexcept;

  //! Trace camera path
  void traceCameraPath(System& system,
                       Scene& scene,
                       const Wavelengths& wavelengths,
                       const int thread_id,
                       const uint x,
                       const uint y) noexcept;

  //! Trace photons
  void tracePhoton(System& system,
                   Scene& scene,
                   const Wavelengths& sampled_wavelengths) noexcept;

  //! Trace photons
  void tracePhoton(System& system,
                   Scene& scene,
                   const Wavelengths& sampled_wavelengths,
                   const int thread_id) noexcept;

  //! Update the photon search radius
  void updateRadius() noexcept;


  Map photon_map_;
  std::vector<typename Map::PhotonList> thread_photon_list_;
  Float radius2_;
  Float inverse_radius_;
  Float inverse_estimation_area_;
  Float alpha_;
  Float photon_power_scale_;
  uint cycle_;
  uint num_of_thread_photons_;
};

//! \} Core

} // namespace nanairo

#include "probabilistic_ppm-inl.hpp"

#endif // NANAIRO_PROBABILISTIC_PPM_HPP
