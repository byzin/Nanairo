///*!
//  \file probabilistic_ppm-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef _NANAIRO_PROBABILISTIC_PPM_INL_HPP_
//#define _NANAIRO_PROBABILISTIC_PPM_INL_HPP_
//
//#include "probabilistic_ppm.hpp"
//// Standard C++ library
//#include <cmath>
//#include <functional>
//#include <future>
//#include <tuple>
//#include <utility>
//// Qt
//#include <QString>
//// Zisc
//#include "zisc/aligned_memory_pool.hpp"
//#include "zisc/algorithm.hpp"
//#include "zisc/error.hpp"
//#include "zisc/math.hpp"
//#include "zisc/thread_pool.hpp"
//#include "zisc/utility.hpp"
//// Nanairo
//#include "rendering_method.hpp"
//#include "NanairoCommon/keyword.hpp"
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/scene.hpp"
//#include "NanairoCore/system.hpp"
//#include "NanairoCore/CameraModel/camera_model.hpp"
//#include "NanairoCore/Data/light_source_reference.hpp"
//#include "NanairoCore/Data/intersection_info.hpp"
//#include "NanairoCore/Data/ray.hpp"
//#include "NanairoCore/Data/wavelength_samples.hpp"
//#include "NanairoCore/DataStructure/bvh.hpp"
//#include "NanairoCore/DataStructure/photon_map.hpp"
//#include "NanairoCore/Material/material.hpp"
//#include "NanairoCore/Material/shader_model.hpp"
//#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"
//#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
//#include "NanairoCore/Sampling/russian_roulette.hpp"
//#include "NanairoCore/Sampling/sampled_point.hpp"
//#include "NanairoCore/Sampling/sampled_direction.hpp"
//#include "NanairoCore/Sampling/sampled_spectra.hpp"
//#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
//#include "NanairoCore/Sampling/sampler.hpp"
//#include "NanairoCore/Utility/unique_pointer.hpp"
//#include "NanairoCore/Utility/scene_settings.hpp"
//
//namespace nanairo {
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::ProbabilisticPpm(
//    System& system,
//    const SceneSettings& settings) :
//        RenderingMethod<kSampleSize>(settings),
//        photon_map_{system},
//        pass_{0}
//{
//  initialize(system, settings);
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping>
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::render(
//    System& system,
//    Scene& scene,
//    const Samples& sampled_wavelengths)
//{
//  photon_map_.clear();
//  updateRadius();
//
//  tracePhoton(system, scene, sampled_wavelengths);
//  traceCameraPath(system, scene, sampled_wavelengths);
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//bool ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::bxdfHasPhotonMap(
//    const ShaderPointer& bxdf) const
//{
//  return (bxdf->type() == ShaderType::Diffuse) || 
//         (kEnableGlossyMapping && bxdf->type() == ShaderType::Glossy);
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping>
//auto ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::estimateRadiance(
//    const int thread_id,
//    const Vector3& vin,
//    const IntersectionInfo& intersection,
//    const ShaderPointer& bxdf,
//    const Wavelengths& wavelengths,
//    const bool wavelength_is_selected) -> Spectra
//{
//
//  // Search photon cache
//  auto& photon_list = thread_photon_list_[thread_id];
//  photon_list.clear();
//  photon_map_.search(intersection.point(), intersection.normal(),
//                     radius2_, photon_list);
//
//  // Estimate radiance
//  Spectra radiance{wavelengths};
//  for (uint i = 0; i < photon_list.size(); ++i) {
//    const auto& photon_point = photon_list[i];
//    // Evaluate reflectance
//    const auto photon_cache = std::get<1>(photon_point);
//    const auto vout = -photon_cache->incidentDirection();
//    const auto f = bxdf->evaluateRadiance(&vin, &vout, wavelengths);
//    NANAIRO_CHECK_POSITIVE_FLOAT(f);
//    if (f.isZero())
//      continue;
//
//    // Calculate contribution
//    const Float distance = zisc::sqrt(std::get<0>(photon_point));
//    const auto weight = kernel_(distance * inverse_radius_);
//    NANAIRO_CHECK_POSITIVE_FLOAT(weight);
//    const auto contribution = weight * f * photon_cache->energy();
//
//    if (!wavelength_is_selected & photon_cache->wavelengthIsSelected())
//      radiance += contribution * wavelengths.primaryInverseProbability();
//    else
//      radiance += contribution;
//  }
//  NANAIRO_CHECK_POSITIVE_FLOAT(radiance);
//  return radiance * inverse_estimation_area_;
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::evaluateImplicitConnection(
//    const Ray& ray,
//    const IntersectionInfo& intersection,
//    const Spectra& ray_weight,
//    Sampler& sampler,
//    MemoryPool& memory_pool,
//    Spectra* contribution) const
//{
//  const auto object = intersection.object();
//  const auto& material = object->material();
//  if (material.isLightSource() && !intersection.isReverseFace()) {
//    const auto& vin = ray.direction();
//    const auto& wavelengths = ray_weight.wavelengths();
//
//    // Get the light
//    const auto& emitter = material.emitter();
//    const auto light = emitter.makeLight(intersection, wavelengths, 
//                                         sampler, memory_pool);
//
//    // Evaluate the radiance
//    const auto radiance = light->evaluateRadiance(&vin, nullptr, wavelengths);
//
//    // Calculate the contribution
//    const auto c = ray_weight * radiance;
//    NANAIRO_CHECK_POSITIVE_FLOAT(c);
//    *contribution += c;
//  }
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//auto ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::generatePhoton(
//    const LightSourceReference& light_source,
//    const Wavelengths& wavelengths,
//    Sampler& sampler,
//    MemoryPool& memory_pool) const -> std::tuple<Photon, Spectra>
//{
//  // Generate a photon
//  const auto object = light_source.object();
//  const auto& geometry = object->geometry();
//  const auto light_point = geometry.samplePoint(sampler);
//  const auto& point = std::get<0>(light_point);
//  const auto& normal = std::get<1>(light_point);
//
//  const auto& emitter = object->material().emitter();
//  IntersectionInfo intersection{point.point(), normal, object, false};
//  const auto light = emitter.makeLight(intersection, wavelengths, 
//                                       sampler, memory_pool);
//  auto vout = light->sampleDirection(sampler);
//
//  const auto ray_epsilon = Method::rayCastEpsilon() * normal;
//  const Photon photon{point.point() + ray_epsilon, vout.direction()};
//
//  const auto k = point.inversePdf() * light_source.inverseWeight() * 
//                 photon_power_scale_;
//  const auto energy = k * light->evaluateWeight(nullptr, &vout, wavelengths);
//  return std::make_tuple(photon, energy);
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//Ray ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::generateRay(
//    const CameraModel& camera,
//    const uint x,
//    const uint y,
//    Sampler& sampler,
//    MemoryPool& memory_pool,
//    Spectra* weight) const
//{
//  const auto& wavelengths = weight->wavelengths();
//  auto sensor = camera.makeSensor(x, y, wavelengths, sampler, memory_pool);
//  const auto vout = sensor->sampleDirection(sampler);
//
//  const auto& lens_point = camera.sampledLensPoint();
//  *weight = *weight * sensor->evaluateWeight(nullptr, &vout, wavelengths);
//  return Ray{lens_point, vout.direction()};
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping>
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::initialize(
//    System& system,
//    const SceneSettings& settings)
//{
//  using zisc::cast;
//  auto& thread_pool = system.threadPool();
//  
//  const auto prefix = QString{keyword::renderingMethod} + "/" +
//                      keyword::probabilisticPpm;
//
//  auto key = prefix + "/" + keyword::numOfPhotons;
//  const auto num_of_photons = cast<uint>(settings.intValue(key));
//  thread_photons_ = num_of_photons / thread_pool.numOfThreads();
//  photon_power_scale_ = 1.0 / cast<Float>(num_of_photons);
//
//  key = prefix + "/" + keyword::radiusReductionRate;
//  alpha_ = settings.realValue(key);
//
//  key = prefix + "/" + keyword::photonSearchRadius;
//  const Float initial_radius = settings.realValue(key);
//  const auto radius2 = (initial_radius * initial_radius) / alpha_;
//  radius2_ = radius2;
//
//  thread_photon_list_.resize(thread_pool.numOfThreads());
//  key = prefix + "/" + keyword::kNearestNeighbor;
//  const uint k = cast<uint>(settings.intValue(key));
//  for (auto& photon_list : thread_photon_list_)
//    photon_list.setK(k);
//
//  initializeFilterKernel(settings);
//
//  std::function<void ()> clear_function{[this, radius2]()
//  {
//    pass_ = 0;
//    radius2_ = radius2;
//  }};
//  Method::setClearFunction(std::move(clear_function));
//
//  photon_map_.reserve(system, num_of_photons * kMaxReflectionCountPrediction);
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping>
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::initializeFilterKernel(
//    const SceneSettings& settings)
//{
//  using zisc::toHash32;
//
//  const auto prefix = QString{keyword::renderingMethod} + "/" +
//                      keyword::probabilisticPpm;
//
//  auto key = prefix + "/" + keyword::kernelType;
//  const auto kernel_type = settings.stringValue(key);
//  const auto kernel_hash = keyword::toHash32(kernel_type);
//  if (kernel_hash == toHash32(keyword::noKernel)) {
//    kernel_ = [](const Float /* t */)
//    {
//      return 1.0;
//    };
//  }
//  else if (kernel_hash == toHash32(keyword::coneKernel)) {
//    kernel_ = [](const Float t)
//    {
//      constexpr Float k = 1.1;
//      constexpr Float inverse_k = 1.0 / k;
//      constexpr Float normalizer = 1.0 / (1.0 - (2.0 / (3.0 * k)));
//      return normalizer * (1.0 - t * inverse_k);
//    };
//  }
//  else if (kernel_hash == toHash32(keyword::perlinKernel)) {
//    kernel_ = [](const Float t)
//    {
//      const Float t2 = t * t;
//      const Float t3 = t2 * t;
//      return 1.0 + t3 * (-6.0 * t2 + 15.0 * t - 10.0);
//    };
//  }
//  else {
//    zisc::raiseError("ProbabilisticPpmError: Unsupported kernel type is specified.");
//  }
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//auto ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::sampleNextPhoton(
//    const uint length,
//    const Photon& photon,
//    const ShaderPointer& bxdf,
//    const IntersectionInfo& intersection,
//    Sampler& sampler,
//    Spectra* photon_energy,
//    bool* wavelength_is_selected) const -> Photon
//{
//  // Sample next direction
//  const auto& vin = photon.direction();
//  const auto vout = bxdf->sampleDirection(sampler);
//  if (vout.inversePdf() == 0.0)
//    return Photon{};
//
//  // Evaluate BxDF weight
//  const auto& wavelengths = photon_energy->wavelengths();
//  const auto weight = bxdf->evaluateWeight(&vin, &vout, wavelengths);
//
//  // Play russian roulette
//  const auto roulette_result = Method::playRussianRoulette(length, weight, sampler);
//  if (!roulette_result.result())
//    return Photon{};
//
//  const Float inverse_probability = 1.0 / roulette_result.probability();
//  *wavelength_is_selected = *wavelength_is_selected || bxdf->wavelengthIsSelected();
//
//  const Float cos_theta = zisc::dot(intersection.normal(), vout.direction());
//  const auto ray_epsilon = (cos_theta > 0.0)
//      ? Method::rayCastEpsilon() * intersection.normal()
//      : -Method::rayCastEpsilon() * intersection.normal();
//
//  *photon_energy = *photon_energy * weight * inverse_probability;
//  return Photon{intersection.point() + ray_epsilon, vout.direction()};
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//Ray ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::sampleNextRay(
//    const Ray& ray,
//    const ShaderPointer& bxdf,
//    const IntersectionInfo& intersection,
//    Sampler& sampler,
//    Spectra* ray_weight,
//    bool* wavelength_is_selected) const
//{
//  // Sample next direction 
//  const auto& vin = ray.direction();
//  const auto vout = bxdf->sampleDirection(sampler);
//  if (vout.inversePdf() == 0.0)
//    return Ray{};
//
//  // Evaluate BxDF weight
//  const auto& wavelengths = ray_weight->wavelengths();
//  const auto weight = bxdf->evaluateWeight(&vin, &vout, wavelengths);
//
//  // Update ray weight
//  if (!(*wavelength_is_selected) && bxdf->wavelengthIsSelected()) {
//    *ray_weight = *ray_weight * wavelengths.primaryInverseProbability();
//    *wavelength_is_selected = true;
//  }
//  *ray_weight *= weight;
//
//  // Update ray
//  const Float cos_theta = zisc::dot(intersection.normal(), vout.direction());
//  const auto ray_epsilon = (cos_theta > 0.0)
//      ? Method::rayCastEpsilon() * intersection.normal()
//      : -Method::rayCastEpsilon() * intersection.normal();
//
//  return Ray{intersection.point() + ray_epsilon, vout.direction()};
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::tracePhoton(
//    System& system,
//    Scene& scene,
//    const Samples& sampled_wavelengths)
//{
//  auto& sampler = system.globalSampler();
//
//  const auto& light_source = scene.world().sampleLightSource(sampler);
//  std::function<void (const int, const uint)> trace_photon{
//  [this, &system, &scene, &sampled_wavelengths, &light_source]
//  (const int thread_id, const uint)
//  {
//    tracePhoton(system, scene, sampled_wavelengths, light_source, thread_id);
//  }};
//
//  auto& thread_pool = system.threadPool();
//  constexpr uint start = 0;
//  const uint end = thread_pool.numOfThreads();
//  auto result = thread_pool.loop(std::move(trace_photon), start, end);
//  result.get();
//
//  photon_map_.construct(system);
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::tracePhoton(
//    System& system,
//    Scene& scene,
//    const Samples& sampled_wavelengths,
//    const LightSourceReference& light_source,
//    const int thread_id)
//{
//  auto& sampler = system.threadSampler(thread_id);
//  auto& memory_pool = system.threadMemoryPool(thread_id);
//  const auto& world = scene.world();
//  const auto& wavelengths = sampled_wavelengths.wavelengths();
//
//  for (uint number = 0; number < thread_photons_; ++number) {
//    auto result = generatePhoton(light_source, wavelengths, sampler, memory_pool);
//    auto& photon = std::get<0>(result);
//    auto& photon_energy = std::get<1>(result);
//
//    uint path_length = 0;
//    bool wavelength_is_selected = false;
//
//    while(photon.isAlive()) {
//      ++path_length;
//
//      // Phton object intersection test
//      const auto intersection = Method::castRay(world, photon);
//      if (!intersection.isIntersected())
//        break;
//
//      // Evaluate the surface
//      const auto& surface = intersection.object()->material().surface();
//      const auto& vin = photon.direction();
//      const auto bxdf = surface.makeBxdf(&vin, intersection, wavelengths, 
//                                         sampler, memory_pool);
//
//      if (bxdfHasPhotonMap(bxdf)) {
//        photon_map_.store(thread_id, intersection.point(), photon.direction(), 
//                          photon_energy, wavelength_is_selected);
//      }
//
//      photon = sampleNextPhoton(path_length, photon, bxdf, intersection,
//                                sampler, &photon_energy, &wavelength_is_selected);
//
//      memory_pool.reset();
//    }
//    memory_pool.reset();
//  }
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::traceCameraPath(
//    System& system,
//    Scene& scene,
//    const Samples& sampled_wavelengths)
//{
//  auto& sampler = system.globalSampler();
//
//  // Set camera
//  auto& camera = scene.camera();
//  camera.sampleLensPoint(sampler);
//  camera.jitter(sampler);
//
//  std::function<void (const int, const uint)> trace_camera_path{
//  [this, &system, &scene, &sampled_wavelengths]
//  (const int thread_id, const uint y) 
//  {
//    traceCameraPath(system, scene, sampled_wavelengths, thread_id, y);
//  }};
//
//  auto& thread_pool = system.threadPool();
//  constexpr uint start = 0;
//  const uint end = camera.heightResolution();
//  auto result = thread_pool.loop(std::move(trace_camera_path), start, end);
//  result.get();
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> 
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::traceCameraPath(
//    System& system,
//    Scene& scene,
//    const Samples& sampled_wavelengths,
//    const int thread_id,
//    const uint y)
//{
//  const auto& wavelengths = sampled_wavelengths.wavelengths();
//  const auto wavelength_weight = makeSampledSpectra(sampled_wavelengths);
//
//  const auto& world = scene.world();
//  auto& camera = scene.camera();
//
//  auto& sampler = system.threadSampler(thread_id);
//  auto& memory_pool = system.threadMemoryPool(thread_id);
//
//  for (uint x = 0; x < camera.widthResolution(); ++x) {
//    Spectra contribution{wavelengths};
//    bool wavelength_is_selected = false;
//
//    // Generate a camera ray
//    auto ray_weight = wavelength_weight;
//    auto ray = generateRay(camera, x, y, sampler, memory_pool, &ray_weight);
//
//    while (ray.isAlive()) {
//      memory_pool.reset();
//
//      // Cast the ray
//      const auto intersection = Method::castRay(world, ray);
//      if (!intersection.isIntersected())
//        break;
//
//      // Evaluate material
//      const auto& material = intersection.object()->material();
//
//      evaluateImplicitConnection(ray, intersection, ray_weight,
//                                 sampler, memory_pool, &contribution);
//
//      // Evaluate surface
//      const auto& vin = ray.direction();
//      const auto& surface = material.surface();
//      const auto bxdf = surface.makeBxdf(&vin, intersection, wavelengths, 
//                                         sampler, memory_pool);
//
//      if (!bxdfHasPhotonMap(bxdf)) {
//        ray = sampleNextRay(ray, bxdf, intersection, sampler,
//                            &ray_weight, &wavelength_is_selected);
//      }
//      else {
//        // Evaluate explicit connection
//        const auto radiance = estimateRadiance(thread_id, vin, intersection, bxdf, 
//                                               wavelengths, wavelength_is_selected);
//        contribution += ray_weight * radiance;
//        break;
//      }
//    }
//    camera.addContribution(x, y, contribution);
//    memory_pool.reset();
//  }
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize, bool kEnableGlossyMapping> inline
//void ProbabilisticPpm<kSampleSize, kEnableGlossyMapping>::updateRadius()
//{
//  using zisc::cast;
//
//  ++pass_;
//  const Float scale_factor = (cast<Float>(pass_ - 1) + alpha_) / cast<Float>(pass_);
//  radius2_ = scale_factor * radius2_;
//  inverse_radius_ = zisc::invSqrt(radius2_);
//
//  constexpr Float k = 1.0 / zisc::kPi;
//  inverse_estimation_area_ = k / radius2_;
//}
//
//} // namespace nanairo
//
//#endif // _NANAIRO_PROBABILISTIC_PPM_INL_HPP_
