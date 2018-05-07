/*!
  \file probabilistic_ppm.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PROBABILISTIC_PPM_HPP
#define NANAIRO_PROBABILISTIC_PPM_HPP

// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/DataStructure/knn_photon_list.hpp"
#include "NanairoCore/DataStructure/photon_map.hpp"
#include "NanairoCore/Sampling/LightSourceSampler/power_weighted_light_source_sampler.hpp"
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
class World;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class ProbabilisticPpm : public RenderingMethod
{
 public:
  using Method = RenderingMethod;
  using Photon = Ray;
  using Spectra = typename Method::Spectra;
  using Shader = ShaderModel;
  using ShaderPointer = RenderingMethod::ShaderPointer;
  using Wavelengths = typename Method::Wavelengths;


  //! Initialize probabilistic ppm method
  ProbabilisticPpm(System& system,
                   const SettingNodeBase* settings,
                   const Scene& scene) noexcept;


  //! Initialize the rendering method
  void initMethod() noexcept override;

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
                           const WavelengthSamples& wavelengths,
                           const bool wavelength_is_selected) noexcept;

  //! Evaluate implicit connection
  void evalImplicitConnection(const Ray& ray,
                              const IntersectionInfo& intersection,
                              const Spectra& camera_contribution,
                              const Spectra& ray_weight,
                              zisc::pmr::memory_resource* mem_resource,
                              Spectra* contribution) const noexcept;

  //! Return the estimated max reflection count
  static constexpr uint estimatedMaxReflectionCount() noexcept;

  //! Generate a photon
  Photon generatePhoton(Spectra* light_contribution,
                        Sampler& sampler,
                        zisc::pmr::memory_resource* mem_resource) const noexcept;

  //! Generate a camera ray
  Ray generateRay(const CameraModel& camera,
                  const uint x,
                  const uint y,
                  Sampler& sampler,
                  zisc::pmr::memory_resource* mem_resource,
                  Spectra* weight) const noexcept;

  //! Initialize
  void initialize(System& system,
                  const SettingNodeBase* settings,
                  const Scene& scene) noexcept;

  //! Return the light sampler for light path
  const PowerWeightedLightSourceSampler& lightPathLightSampler() const noexcept;

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


  PhotonMap photon_map_;
  zisc::pmr::vector<KnnPhotonList> thread_photon_list_;
  Float init_radius2_;
  Float radius2_;
  Float inverse_radius_;
  Float inverse_estimation_area_;
  Float alpha_;
  Float photon_power_scale_;
  zisc::UniqueMemoryPointer<PowerWeightedLightSourceSampler> light_path_light_sampler_;
  uint cycle_;
  uint num_of_thread_photons_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_PROBABILISTIC_PPM_HPP
