///*!
//  \file probabilistic_ppm.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef _NANAIRO_PROBABILISTIC_PPM_HPP_
//#define _NANAIRO_PROBABILISTIC_PPM_HPP_
//
//// Standard C++ library
//#include <functional>
//#include <vector>
//// Nanairo
//#include "rendering_method.hpp"
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/DataStructure/knn_photon_list.hpp"
//#include "NanairoCore/DataStructure/photon_map.hpp"
//
//namespace nanairo {
//
//// Forward declaration
//class IntersectionInfo;
//class LightSourceReference;
//class Material;
//class Object;
//class Ray;
//template <uint> class SampledSpectra;
//template <uint> class SampledWavelengths;
//class Scene;
//class SceneSettings;
//template <uint> class ShaderModel;
//class System;
//template <typename> class UniquePointer;
//template <uint> class WavelengthSamples;
//
////! \addtogroup Core
////! \{
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping>
//class ProbabilisticPpm : public RenderingMethod<kSampleSize>
//{
// public:
//  using Method = RenderingMethod<kSampleSize>;
//  using Map = PhotonMap<kSampleSize>;
//  using Photon = Ray;
//  using Spectra = typename Method::Spectra;
//  using Samples = typename Method::Samples;
//  using Shader = ShaderModel<kSampleSize>;
//  using ShaderPointer = UniquePointer<Shader>;
//  using Wavelengths = WavelengthSamples<kSampleSize>;
//
//
//  //! Initialize probabilistic ppm method
//  ProbabilisticPpm(System& system, const SceneSettings& settings);
//
//
//  //! Render scene using probabilistic ppm method
//  void render(System& system,
//              Scene& scene,
//              const Samples& sampled_wavelengths) override;
//
// private:
//  //! Check if the BxDF has photon map
//  bool bxdfHasPhotonMap(const ShaderPointer& bxdf) const;
//
//  //! Estimate the radiance
//  Spectra estimateRadiance(const int thread_id,
//                           const Vector3& vin,
//                           const IntersectionInfo& intersection,
//                           const ShaderPointer& bxdf,
//                           const Wavelengths& wavelengths,
//                           const bool wavelength_is_selected);
//
//  //! Evaluate implicit connection
//  void evaluateImplicitConnection(const Ray& ray,
//                                  const IntersectionInfo& intersection,
//                                  const Spectra& ray_weight,
//                                  Sampler& sampler,
//                                  MemoryPool& memory_pool,
//                                  Spectra* contribution) const;
//
//  //! Generate a photon
//  std::tuple<Photon, Spectra> generatePhoton(const LightSourceReference& light_source,
//                                             const Wavelengths& wavelengths,
//                                             Sampler& sampler,
//                                             MemoryPool& memory_pool) const;
//
//  //! Generate a camera ray
//  Ray generateRay(const CameraModel& camera,
//                  const uint x,
//                  const uint y,
//                  Sampler& sampler,
//                  MemoryPool& memory_pool,
//                  Spectra* weight) const;
//
//  //! Initialize
//  void initialize(System& system, const SceneSettings& settings);
//
//  //! Initialize filter kernel
//  void initializeFilterKernel(const SceneSettings& settings);
//
//  //! Sample next photon
//  Photon sampleNextPhoton(const uint length,
//                          const Photon& photon,
//                          const ShaderPointer& bxdf,
//                          const IntersectionInfo& intersection,
//                          Sampler& sampler,
//                          Spectra* photon_energy,
//                          bool* wavelength_is_selected) const;
//
//  //! Sample next photon
//  Ray sampleNextRay(const Ray& ray,
//                    const ShaderPointer& bxdf,
//                    const IntersectionInfo& intersection,
//                    Sampler& sampler,
//                    Spectra* photon_energy,
//                    bool* wavelength_is_selected) const;
//
//  //! Trace camera path
//  void traceCameraPath(System& system, 
//                       Scene& scene,
//                       const Samples& sampled_wavelengths);
//
//  //! Trace camera path
//  void traceCameraPath(System& system, 
//                       Scene& scene,
//                       const Samples& sampled_wavelengths,
//                       const int thread_id,
//                       const uint y);
//
//  //! Trace photons
//  void tracePhoton(System& system, 
//                   Scene& scene,
//                   const Samples& sampled_wavelengths);
//
//  //! Trace photons
//  void tracePhoton(System& system,
//                   Scene& scene,
//                   const Samples& sampled_wavelengths,
//                   const LightSourceReference& light_source,
//                   const int thread_id);
//
//  //! Update the photon search radius
//  void updateRadius();
//
//
//  static constexpr uint kMaxReflectionCountPrediction = 15;
//
//  
//  Map photon_map_;
//  std::function<Float (const Float)> kernel_;
//  std::vector<typename Map::PhotonList> thread_photon_list_;
//  Float radius2_;
//  Float inverse_radius_;
//  Float inverse_estimation_area_;
//  Float alpha_;
//  Float photon_power_scale_;
//  uint pass_;
//  uint thread_photons_;
//};
//
////! \} Core
//
//} // namespace nanairo
//
//#include "probabilistic_ppm-inl.hpp"
//
//#endif // _NANAIRO_PROBABILISTIC_PPM_HPP_
