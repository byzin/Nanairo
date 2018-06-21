/*!
  \file probabilistic_ppm.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "probabilistic_ppm.hpp"
// Standard C++ library
#include <atomic>
#include <cmath>
#include <future>
#include <limits>
#include <memory>
#include <thread>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_manager.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "path_tracing.hpp"
#include "rendering_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/Data/light_source_info.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"
#include "NanairoCore/DataStructure/photon_map.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Sampling/LightSourceSampler/light_source_sampler.hpp"
#include "NanairoCore/Setting/rendering_method_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ProbabilisticPpm::ProbabilisticPpm(System& system,
                                   const SettingNodeBase* settings,
                                   const Scene& scene) noexcept :
    RenderingMethod(system, settings),
    thread_photon_list_{
        decltype(thread_photon_list_)::allocator_type{&system.dataMemoryManager()}}
{
  initialize(system, settings, scene);
}

/*!
  \details
  No detailed.
  */
void ProbabilisticPpm::render(System& system,
                              Scene& scene,
                              const Wavelengths& sampled_wavelengths,
                              const uint64 cycle) noexcept
{
  photon_map_.initialize(system, num_of_photons_);
  tracePhoton(system, scene, sampled_wavelengths);
  photon_map_.construct(system);
  traceCameraPath(system, scene, sampled_wavelengths, cycle);
  photon_map_.reset();
}

/*!
  */
Float ProbabilisticPpm::calcPhotonSearchRadius(const uint64 cycle) const noexcept
{
  constexpr Float initial_radius = 0.2;
  constexpr Float e = -1.0 / 6.0;
  const Float r = initial_radius * std::pow(zisc::cast<Float>(cycle), e);
  return r;
}

/*!
  */
Float ProbabilisticPpm::evalKernel(const Float t) const noexcept
{
  Float k = 1.0 + zisc::power<3>(t) * (t * (-6.0 * t + 15.0) - 10.0);
  k = zisc::clamp(k, std::numeric_limits<Float>::epsilon(), 1.0);
  return k;
}

/*!
 */
void ProbabilisticPpm::estimateExplicitConnection(
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
    Spectra* contribution) const noexcept
{
  if (!explicit_connection_is_enabled)
    return;

  // Search photon caches
  {
    const Float radius2 = zisc::power<2>(search_radius);
    photon_list.clear();
    const bool is_frontside_culling = !bxdf->isReflective();
    const bool is_backside_culling = !bxdf->isTransmissive();
    photon_map_.search(intersection.point(), intersection.normal(), radius2,
                       is_frontside_culling, is_backside_culling, &photon_list);
  }

  // Estimate radiance
  const auto& wavelengths = ray_weight.wavelengths();
  const Float inv_radius = zisc::invert(search_radius);
  constexpr Float inv_pi = zisc::invert(zisc::kPi<Float>);
  const Float inv_acceptance_probability = (photon_list.size() == photon_list.k())
      ? inv_pi * zisc::invert(std::get<0>(photon_list[0]))
      : inv_pi * zisc::power<2>(inv_radius);
  Spectra radiance{wavelengths, 0.0};
  for (uint i = 0; i < photon_list.size(); ++i) {
    const auto& photon_point = photon_list[i];
    // Evaluate reflectance
    const auto photon_cache = std::get<1>(photon_point);
    const auto vout = -photon_cache->incidentDirection();
    const auto result = bxdf->evalRadianceAndPdf(&ray.direction(),
                                                 &vout,
                                                 wavelengths,
                                                 &intersection);
    const auto& f = std::get<0>(result);
    ZISC_ASSERT(!f.hasNegative(), "The f of BxDF has negative values.");

    // Evaluate the photon weight
    const Float distance = zisc::sqrt(std::get<0>(photon_point));
    const Float t = distance * inv_radius;
    ZISC_ASSERT(zisc::isInBounds(t, 0.0, 1.0), "The t is out of range [0, 1).");
    const Float kernel_weight = evalKernel(t);

    // Calc a MIS weight
    const auto& direction_pdf = std::get<1>(result);
    ZISC_ASSERT(0.0 <= direction_pdf, "The direction pdf isn't positive.");
    const Float inv_margin_pdf = photon_cache->inversePdf() *
                                 inv_acceptance_probability;
    const Float mis_weight = implicit_connection_is_enabled
        ? PathTracing::calcMisWeight(direction_pdf, inv_margin_pdf)
        : 1.0;

    // Calc a wavelength weight
    const Float wavelength_weight =
        (wavelength_is_selected && photon_cache->wavelengthIsSelected())
            ? zisc::invert(wavelengths.primaryInverseProbability())
            : 1.0;

    // Calc a contribution of a photon
    const auto c = (camera_contribution * ray_weight * f * photon_cache->energy()) *
                   (kernel_weight * inv_acceptance_probability *  mis_weight * wavelength_weight);
    ZISC_ASSERT(!c.hasNegative(), "The contribution has negative values.");
    radiance += c;
  }
  if (0 < photon_list.size())
    *contribution += radiance;
}

void ProbabilisticPpm::evalImplicitConnection(
    const World& /* world */,
    const Ray& ray,
    const Float inverse_direction_pdf,
    const IntersectionInfo& intersection,
    const Spectra& camera_contribution,
    const Spectra& ray_weight,
    const Float search_radius,
    const bool implicit_connection_is_enabled,
    const bool explicit_connection_is_enabled,
    zisc::pmr::memory_resource* mem_resource,
    Spectra* contribution) const noexcept
{
  if (!implicit_connection_is_enabled)
    return;

  const auto object = intersection.object();
  const auto& material = object->material();
  if (!material.isLightSource() || intersection.isBackFace())
    return;

  const auto& wavelengths = ray_weight.wavelengths();
  const auto vout = -ray.direction();

  // Get the light
  const auto& emitter = material.emitter();
  const auto light = emitter.makeLight(intersection.uv(), wavelengths, mem_resource);

  // Evaluate the radiance
  const auto result = light->evalRadianceAndPdf(nullptr,
                                                &vout,
                                                wavelengths,
                                                &intersection);
  const auto& radiance = std::get<0>(result);
  const auto& light_dir_pdf = std::get<1>(result);

  // Calculate the MIS weight
  Float mis_weight = 1.0;
  if (explicit_connection_is_enabled) {
    const auto& light_sampler = lightPathLightSampler();
    const auto light_source_info = light_sampler.getInfo(intersection, object);
    const Float acceptance_probability = zisc::kPi<Float> * zisc::power<2>(search_radius);
    const Float margin_pdf = light_dir_pdf * zisc::cast<Float>(num_of_photons_) *
                             acceptance_probability *
                             zisc::invert(light_source_info.inverseWeight() *
                                          object->shape().surfaceArea());
    mis_weight = PathTracing::calcMisWeight(margin_pdf, inverse_direction_pdf);
  }

  // Calculate the contribution
  const auto c = (camera_contribution * ray_weight * radiance) * mis_weight;
  ZISC_ASSERT(!c.hasNegative(), "The contribution has negative values.");
  *contribution += c;
}

/*!
  \details
  No detailed.
  */
inline
auto ProbabilisticPpm::generatePhoton(
    Sampler& sampler,
    zisc::pmr::memory_resource* mem_resource,
    Spectra* weight,
    Float* inverse_sampling_pdf) const noexcept -> Photon
{
  const auto& wavelengths = weight->wavelengths();
  // Sample a light point
  const auto& light_sampler = lightPathLightSampler();
  const auto light_source_info = light_sampler.sample(sampler);
  const auto light_source = light_source_info.object();
  const auto light_point_info = light_source->shape().samplePoint(sampler);
  ZISC_ASSERT(0.0 < light_point_info.pdf(), "The point pdf is negative.");

  // Sample a direction
  const auto& emitter = light_source->material().emitter();
  const IntersectionInfo intersection{light_source, light_point_info};
  const auto light = emitter.makeLight(intersection.uv(), wavelengths, mem_resource);
  const auto result = light->sample(nullptr, wavelengths, sampler, &intersection);
  const auto& sampled_vout = std::get<0>(result);
  ZISC_ASSERT(0.0 < sampled_vout.pdf(), "The vout pdf is negative.");
  const auto& vout = sampled_vout.direction();
  const Float direction_pdf = light->evalPdf(nullptr,
                                             &vout,
                                             wavelengths,
                                             &intersection);

  *inverse_sampling_pdf =
      (light_source_info.inverseWeight() * light_point_info.inversePdf()) /
      zisc::cast<Float>(num_of_photons_);

  // Evaluate the light contribution
  const auto& w = std::get<1>(result);
  *weight = (*weight * w) * (*inverse_sampling_pdf);

  *inverse_sampling_pdf = *inverse_sampling_pdf / direction_pdf;
  ZISC_ASSERT(0.0 < *inverse_sampling_pdf, "The photon coefficient is negative.");

  const auto& normal = light_point_info.normal();
  const auto ray_epsilon = Method::rayCastEpsilon() * normal;
  ZISC_ASSERT(!isZeroVector(ray_epsilon), "Ray epsilon is zero vector.");
  return Photon::makeRay(light_point_info.point() + ray_epsilon, vout);
}

/*!
  \details
  No detailed.
  */
void ProbabilisticPpm::initialize(System& system,
                                  const SettingNodeBase* settings,
                                  const Scene& scene) noexcept
{
  const auto method_settings = castNode<RenderingMethodSettingNode>(settings);

  using zisc::cast;
  auto& threads = system.threadManager();

  const auto& parameters = method_settings->probabilisticPpmParameters();
  {
    num_of_photons_ = parameters.num_of_photons_;
  }

  {
    thread_photon_list_.reserve(threads.numOfThreads());
    for (uint i = 0; i < threads.numOfThreads(); ++i) {
      const uint k = parameters.k_nearest_neighbor_;
      thread_photon_list_.emplace_back(&system.dataMemoryManager());
      thread_photon_list_.back().setK(k);
    }
  }

  {
    const auto sampler_type = parameters.light_path_light_sampler_type_;
    light_path_light_sampler_ = LightSourceSampler::makeSampler(
            system,
            sampler_type,
            scene.world(),
            settings->workResource());
  }
}

/*!
  */
inline
const LightSourceSampler& ProbabilisticPpm::lightPathLightSampler() const noexcept
{
  return *light_path_light_sampler_;
}

/*!
  \details
  No detailed.
  */
inline
bool ProbabilisticPpm::surfaceHasPhotonMap(
    const ShaderPointer& bxdf) const noexcept
{
  return (bxdf->type() == ShaderType::Diffuse) ||
         (bxdf->type() == ShaderType::Glossy);
}

/*!
  \details
  No detailed.
  */
inline
void ProbabilisticPpm::traceCameraPath(
    System& system,
    Scene& scene,
    const Wavelengths& sampled_wavelengths,
    const uint64 cycle) noexcept
{
  auto& sampler = system.globalSampler();

  // Init camera
  {
    auto& camera = scene.camera();
    camera.sampleLensPoint(sampler);
    camera.jitter(sampler);
  }

  std::atomic<uint> tile_count{0};

  auto trace_camera_path =
  [this, &system, &scene, &sampled_wavelengths, cycle, &tile_count]
  (const int thread_id, const uint)
  {
    const auto& camera = scene.camera();
    const uint num_of_tiles = 
        RenderingMethod::calcNumOfTiles(camera.imageResolution());

    for (uint index = tile_count++; index < num_of_tiles; index = tile_count++) {
      auto tile = RenderingMethod::getRenderingTile(camera.imageResolution(), index);
      for (uint i = 0; i < tile.numOfPixels(); ++i) {
        const auto& pixel_index = tile.current();
        traceCameraPath(system, scene, sampled_wavelengths, cycle,
                        thread_id, pixel_index);
        tile.next();
      }
    }
  };

  {
    auto& threads = system.threadManager();
    auto& work_resource = system.globalMemoryManager();
    constexpr uint start = 0;
    const uint end = threads.numOfThreads();
    auto result = threads.enqueueLoop(trace_camera_path, start, end, &work_resource);
    result.get();
  }
}

/*!
  \details
  No detailed.
  */
void ProbabilisticPpm::traceCameraPath(
    System& system,
    Scene& scene,
    const Wavelengths& sampled_wavelengths,
    const uint64 cycle,
    const int thread_id,
    const Index2d& pixel_index) noexcept
{
  // System
  auto& sampler = system.threadSampler(thread_id);
  auto& memory_manager = system.threadMemoryManager(thread_id);
  // Scene
  const auto& world = scene.world();
  auto& camera = scene.camera();
  // Trace info
  const auto& wavelengths = sampled_wavelengths.wavelengths();
  auto camera_contribution = makeSampledSpectra(sampled_wavelengths);
  Spectra contribution{wavelengths};
  uint path_length = 1;
  bool wavelength_is_selected = false;

  const bool implicit_connection_is_enabled =
      CoreConfig::pathTracingImplicitConnectionIsEnabled();
  bool explicit_connection_is_enabled = false; // Explicit camera-light connection isn't performed

  // Generate a camera ray
  Float inverse_direction_pdf;
  Spectra ray_weight{wavelengths, 1.0};
  auto ray = PathTracing::generateRay(camera, pixel_index, sampler, &memory_manager,
                                      &camera_contribution, &inverse_direction_pdf);

  const Float photon_search_radius = calcPhotonSearchRadius(cycle);

  while (ray.isAlive()) {
    // Cast the ray
    const auto intersection = Method::castRay(world, ray);
    if (!intersection.isIntersected())
      break;

    evalImplicitConnection(world, ray, inverse_direction_pdf, intersection,
                           camera_contribution, ray_weight, photon_search_radius,
                           implicit_connection_is_enabled,
                           explicit_connection_is_enabled && (1 < path_length),
                           &memory_manager, &contribution);

    // Evaluate material
    const auto& material = intersection.object()->material();
    const auto& surface = material.surface();
    const auto bxdf = surface.makeBxdf(intersection,
                                       wavelengths,
                                       sampler,
                                       &memory_manager);
    RenderingMethod::updateSelectedWavelengthInfo(bxdf,
                                                  &ray_weight,
                                                  &wavelength_is_selected);

    // Sample next ray
    auto next_ray_weight = ray_weight;
    const auto next_ray = Method::sampleNextRay(path_length, ray, bxdf, intersection,
                                                &ray_weight, &next_ray_weight,
                                                sampler, &inverse_direction_pdf);
    if (!next_ray.isAlive())
      break;
    ++path_length;

    explicit_connection_is_enabled = surfaceHasPhotonMap(bxdf) &&
        CoreConfig::pathTracingExplicitConnectionIsEnabled();

    auto& photon_list = thread_photon_list_[thread_id];
    estimateExplicitConnection(ray, bxdf, intersection, camera_contribution,
                               ray_weight, photon_search_radius,
                               wavelength_is_selected,
                               explicit_connection_is_enabled,
                               implicit_connection_is_enabled,
                               photon_list, &contribution);

    // Update ray
    ray = next_ray;
    ray_weight = next_ray_weight;
    // Clear memory
    memory_manager.reset();
  }
  camera.addContribution(pixel_index, contribution);
  // Clear memory
  memory_manager.reset();
}

/*!
  \details
  No detailed.
  */
void ProbabilisticPpm::tracePhoton(
    System& system,
    Scene& scene,
    const Wavelengths& sampled_wavelengths) noexcept
{
  std::atomic<uint> photon_set_index{0};

  auto trace_photon =
  [this, &system, &scene, &sampled_wavelengths, &photon_set_index]
  (const int thread_id, const uint)
  {
    bool flag = true;
    for (uint index = photon_set_index++; flag; index = photon_set_index++) {
      constexpr uint photon_set_size =
          zisc::power<2>(CoreConfig::sizeOfRenderingTileSide());
      for (uint i = 0; i < photon_set_size; ++i) {
        const uint photon_index = index * photon_set_size + i;
        if (num_of_photons_ <= photon_index) {
          flag = false;
          break;
        }
        tracePhoton(system, scene, sampled_wavelengths, thread_id);
      }
    }
  };

  {
    auto& threads = system.threadManager();
    auto& work_resource = system.globalMemoryManager();
    constexpr uint start = 0;
    const uint end = threads.numOfThreads();
    auto result = threads.enqueueLoop(trace_photon, start, end, &work_resource);
    result.get();
  }
}

/*!
  \details
  No detailed.
  */
void ProbabilisticPpm::tracePhoton(
    System& system,
    Scene& scene,
    const Wavelengths& sampled_wavelengths,
    const int thread_id) noexcept
{
  // System
  auto& sampler = system.threadSampler(thread_id);
  auto& memory_manager = system.threadMemoryManager(thread_id);
  // Scene
  const auto& world = scene.world();
  // Trace info
  const auto& wavelengths = sampled_wavelengths.wavelengths();
  Spectra light_contribution{wavelengths, 1.0};
  uint path_length = 2;
  bool wavelength_is_selected = false;

  // Generate a photon
  Float inverse_sampling_pdf;
  auto photon_weight = light_contribution;
  auto photon = generatePhoton(sampler, &memory_manager,
                               &light_contribution, &inverse_sampling_pdf);

  while(true) {
    // Phton object intersection test
    const auto intersection = Method::castRay(world, photon);
    if (!intersection.isIntersected())
      break;

    // Evaluate the surface
    const auto& surface = intersection.object()->material().surface();
    const auto bxdf = surface.makeBxdf(intersection,
                                       wavelengths,
                                       sampler,
                                       &memory_manager);

    if (surfaceHasPhotonMap(bxdf)) {
      const auto photon_energy = light_contribution * photon_weight;
      photon_map_.store(intersection.point(), photon.direction(), photon_energy,
                        inverse_sampling_pdf, wavelength_is_selected);
      break;
    }

    RenderingMethod::updateSelectedWavelengthInfo(bxdf,
                                                  &photon_weight,
                                                  &wavelength_is_selected);

    Float inverse_direction_pdf;
    auto next_photon_weight = photon_weight;
    photon = Method::sampleNextRay(path_length, photon, bxdf,
                                   intersection, &photon_weight,
                                   &next_photon_weight, sampler,
                                   &inverse_direction_pdf);
    if (!photon.isAlive())
      break;
    ++path_length;

    // Update the photon
    photon_weight = next_photon_weight;
    ZISC_ASSERT(inverse_direction_pdf == 1.0,
                "The direction pdf isn't 1: ", inverse_direction_pdf);
    // Clear memory
    memory_manager.reset();
  }
  memory_manager.reset();
}

} // namespace nanairo
