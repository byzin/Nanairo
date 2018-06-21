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
#include "NanairoCore/Sampling/LightSourceSampler/light_source_sampler.hpp"
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


  //! Render scene using probabilistic ppm method
  void render(System& system,
              Scene& scene,
              const Wavelengths& sampled_wavelengths,
              const uint64 cycle) noexcept override;

 private:
  //! Calculate a photon search radius
  Float calcPhotonSearchRadius(const uint64 cycle) const noexcept;

  //! Evaluate the perlin kernel
  Float evalKernel(const Float t) const noexcept;

  //!
  void estimateExplicitConnection(
      const Ray& ray,
      const ShaderPointer& bxdf,
      const IntersectionInfo& intersection,
      const Spectra& camera_contribution,
      const Spectra& ray_weight,
      const Float search_radius,
      const bool wavelength_is_selected,
      const bool explicit_connection_is_enabled,
      const bool implicit_connection_is_enabled,
      KnnPhotonList& photon_list,
      Spectra* contribution) const noexcept;

  //! Evaluate the implicit connection
  void evalImplicitConnection(
      const World& world,
      const Ray& ray,
      const Float inverse_direction_pdf,
      const IntersectionInfo& intersection,
      const Spectra& camera_contribution,
      const Spectra& ray_weight,
      const Float search_radius,
      const bool implicit_connection_is_enabled,
      const bool emplicit_connection_is_enabled,
      zisc::pmr::memory_resource* mem_resource,
      Spectra* contribution) const noexcept;

  //! Generate a photon
  Photon generatePhoton(Sampler& sampler,
                        zisc::pmr::memory_resource* mem_resource,
                        Spectra* weight,
                        Float* inverse_sampling_pdf) const noexcept;

  //! Initialize
  void initialize(System& system,
                  const SettingNodeBase* settings,
                  const Scene& scene) noexcept;

  //! Return the light sampler for light path
  const LightSourceSampler& lightPathLightSampler() const noexcept;

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
                       const Wavelengths& wavelengths,
                       const uint64 cycle) noexcept;

  //! Trace camera path
  void traceCameraPath(System& system,
                       Scene& scene,
                       const Wavelengths& wavelengths,
                       const uint64 cycle,
                       const int thread_id,
                       const Index2d& pixel_index) noexcept;

  //! Trace photons
  void tracePhoton(System& system,
                   Scene& scene,
                   const Wavelengths& sampled_wavelengths) noexcept;

  //! Trace photons
  void tracePhoton(System& system,
                   Scene& scene,
                   const Wavelengths& sampled_wavelengths,
                   const int thread_id) noexcept;


  PhotonMap photon_map_;
  zisc::pmr::vector<KnnPhotonList> thread_photon_list_;
  zisc::UniqueMemoryPointer<LightSourceSampler> light_path_light_sampler_;
  uint num_of_photons_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_PROBABILISTIC_PPM_HPP
