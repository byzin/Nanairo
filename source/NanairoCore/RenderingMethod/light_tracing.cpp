/*!
  \file light_tracing.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "light_tracing.hpp"
// Standard C++ library
#include <future>
#include <memory>
#include <thread>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_pool.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/light_source_info.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Sampling/LightSourceSampler/power_weighted_light_source_sampler.hpp"
#include "NanairoCore/Setting/rendering_method_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
LightTracing::LightTracing(System& system,
                           const SettingNodeBase* settings,
                           const Scene& scene) noexcept :
    RenderingMethod(system, settings)
{
  initialize(system, settings, scene);
}

/*!
  \details
  No detailed.
  */
void LightTracing::render(System& system,
                          Scene& scene,
                          const Wavelengths& sampled_wavelengths) noexcept
{
  traceLightPath(system, scene, sampled_wavelengths);
}

/*!
  \details
  No detailed.
  */
void LightTracing::evalExplicitConnection(
    const World& world,
    const Vector3* vin,
    const ShaderPointer& bxdf,
    const IntersectionInfo& intersection,
    const Spectra& light_contribution,
    const Spectra& ray_weight,
    CameraModel& camera,
    zisc::MemoryPool& memory_pool) noexcept
{
  if (bxdf->type() == ShaderType::Specular)
    return;

  // Check if the camera is in front or back of the surface
  const bool is_in_front = 0.0 < zisc::dot(intersection.normal(),
                                           camera.sampledLensPoint() - intersection.point());
  if (!(is_in_front ? bxdf->isReflective() : bxdf->isTransmissive()))
    return;

  // Make a shadow ray
  const auto shadow_ray = Method::makeShadowRay(intersection.point(),
                                                camera.sampledLensPoint(),
                                                intersection.normal(),
                                                is_in_front);
  const Float cos_no = (is_in_front)
      ? zisc::dot(intersection.normal(), shadow_ray.direction())
      : -zisc::dot(intersection.normal(), shadow_ray.direction());
  if (cos_no <= 0.0)
    return;

  // Check the visibility of the camera
  const auto diff2 = (camera.sampledLensPoint() - shadow_ray.origin()).squareNorm();
  ZISC_ASSERT(0.0 < diff2, "Diff^2 isn't greater than 0.");
  const Float max_shadow_ray_distance = zisc::sqrt(diff2);
  const bool expect_no_hit = true;
  const auto shadow_intersection = Method::castRay(world,
                                                   shadow_ray,
                                                   max_shadow_ray_distance,
                                                   expect_no_hit);
  if (shadow_intersection.isIntersected())
    return;

  // Get the pixel location
  Index2d pixel_index;
  const bool ray_hits_film = camera.calcPixelLocation(shadow_ray.direction(),
                                                      &pixel_index);
  if (!ray_hits_film)
    return;

  // Evaluate the surface reflectance
  const auto& wavelengths = ray_weight.wavelengths();
  //! \todo fix normal
  const auto f = bxdf->evalRadiance(vin,
                                    &shadow_ray.direction(),
                                    wavelengths,
                                    &intersection);
  ZISC_ASSERT(!f.hasNegative(), "The f of BxDF has negative values.");

  // Evaluate the camera importance
  const auto sensor = camera.makeSensor(pixel_index, wavelengths, memory_pool);
  const auto camera_dir = -shadow_ray.direction();
  const auto importance = sensor->evalRadiance(nullptr, &camera_dir, wavelengths);
  ZISC_ASSERT(!importance.hasNegative(), "The importance has negative values.");

  // Calculate the geometry term
  const auto camera_normal = camera.getNormal(pixel_index);
  const Float cos_cni = zisc::dot(camera_normal, camera_dir);
  const Float geometry_term = (cos_cni * cos_no) / diff2;
  ZISC_ASSERT(0.0 <= geometry_term, "Geometry term is negative.");

  // Calculate the contribution
  const auto contribution = (light_contribution * ray_weight * f * importance) *
                            geometry_term;
  ZISC_ASSERT(!contribution.hasNegative(), "The contribution has negative values.");
  addLightContribution(camera, pixel_index, contribution);
}

/*!
  \details
  No detailed.
  */
Ray LightTracing::generateRay(const World& world,
                              Spectra* light_contribution,
                              const Spectra& ray_weight,
                              CameraModel& camera,
                              Sampler& sampler,
                              zisc::MemoryPool& memory_pool) noexcept
{
  const auto& wavelengths = light_contribution->wavelengths();
  // Sample a light point
  const auto& light_sampler = lightPathLightSampler();
  const auto light_source_info = light_sampler.sample(sampler);
  const auto light_source = light_source_info.object();
  const auto light_point_info = light_source->shape().samplePoint(sampler);
  ZISC_ASSERT(0.0 < light_point_info.pdf(), "The point pdf is negative.");

  // Sample a direction
  const auto& emitter = light_source->material().emitter();
  const IntersectionInfo intersection{light_source, light_point_info};
  const auto light = emitter.makeLight(intersection.uv(), wavelengths, memory_pool);

  // Evaluate the explicit connection
  const auto light_pdf = light_source_info.inverseWeight() *
                         light_point_info.inversePdf();
  ZISC_ASSERT(0.0 < light_pdf, "The light ray coefficient is negative.");
  *light_contribution = light_pdf * (*light_contribution);
  evalExplicitConnection(world, nullptr, light, intersection,
                         *light_contribution, ray_weight, camera, memory_pool);
  // Sample a ray direction
  const auto result = light->sample(nullptr, wavelengths, sampler, &intersection);
  const auto& sampled_vout = std::get<0>(result);
  ZISC_ASSERT(0.0 < sampled_vout.pdf(), "The ray direction pdf is negative.");
  // Evaluate the light_contribution
  const auto& w = std::get<1>(result);
  *light_contribution = w * (*light_contribution);

  // Generate a ray
  const auto& normal = light_point_info.normal();
  const auto ray_epsilon = Method::rayCastEpsilon() * normal;
  ZISC_ASSERT(!isZeroVector(ray_epsilon), "Ray epsilon is zero vector.");
  return Ray::makeRay(light_point_info.point() + ray_epsilon,
                      sampled_vout.direction());
}

/*!
  \details
  No detailed.
  */
inline
void LightTracing::addLightContribution(CameraModel& camera,
                                        const Index2d& index,
                                        const Spectra& contribution) noexcept
{
  {
    std::unique_lock<std::mutex> locker{lock_};
    camera.addContribution(index, contribution);
  }
}

/*!
  \details
  No detailed.
  */
inline
void LightTracing::initialize(System& system,
                              const SettingNodeBase* settings,
                              const Scene& scene) noexcept
{
  const auto method_settings = castNode<RenderingMethodSettingNode>(settings);
  const auto& parameters = method_settings->lightTracingParameters();

  {
    const auto sampler_type = parameters.light_path_light_sampler_type_;
    light_path_light_sampler_ = LightSourceSampler::makeSampler(sampler_type,
                                                                scene.world(),
                                                                system);
  }
}

/*!
  */
inline
const LightSourceSampler& LightTracing::lightPathLightSampler() const noexcept
{
  return *light_path_light_sampler_;
}

/*!
  \details
  No detailed.
  */
inline
void LightTracing::traceLightPath(
    System& system,
    Scene& scene,
    const Wavelengths& sampled_wavelengths) noexcept
{
  auto& sampler = system.globalSampler();

  // Init camera
  {
    auto& camera = scene.camera();
    camera.sampleLensPoint(sampler);
  }

  auto trace_light_path =
  [this, &system, &scene, &sampled_wavelengths](const int thread_id, const uint)
  {
    const auto& camera = scene.camera();
    const uint num_of_pixels = camera.widthResolution() * camera.heightResolution();

    const auto range = system.calcThreadRange(num_of_pixels, thread_id);

    for (uint i = range[0]; i < range[1]; ++i)
      traceLightPath(system, scene, sampled_wavelengths, thread_id);
  };

  {
    auto& thread_pool = system.threadPool();
    constexpr uint start = 0;
    const uint end = thread_pool.numOfThreads();
    auto result = thread_pool.enqueueLoop(trace_light_path, start, end);
    result.get();
  }
}

/*!
  \details
  No detailed.
  */
void LightTracing::traceLightPath(System& system,
                                  Scene& scene,
                                  const Wavelengths& sampled_wavelengths,
                                  const int thread_id) noexcept
{
  // System
  auto& sampler = system.threadSampler(thread_id);
  auto& memory_pool = system.threadMemoryPool(thread_id);
  // Scene
  const auto& world = scene.world();
  auto& camera = scene.camera();
  // Trace info
  const auto& wavelengths = sampled_wavelengths.wavelengths();
  auto light_contribution = makeSampledSpectra(sampled_wavelengths);
  Spectra contribution{wavelengths};
  IntersectionInfo intersection;
  uint path_length = 1;
  bool wavelength_is_selected = false;

  // Generate a light ray
  Spectra ray_weight{wavelengths, 1.0};
  auto ray = generateRay(world, &light_contribution, ray_weight, camera,
                         sampler, memory_pool);

  while (true) {
    // Cast the ray
    intersection = Method::castRay(world, ray);
    if (!intersection.isIntersected())
      break;

    // Get a BxDF of the surface
    const auto& material = intersection.object()->material();
    const auto& surface = material.surface();
    const auto bxdf = surface.makeBxdf(intersection,
                                       wavelengths,
                                       sampler,
                                       memory_pool);
    Method::updateSelectedWavelengthInfo(bxdf,
                                         &light_contribution,
                                         &wavelength_is_selected);

    // Sample next ray
    auto next_ray_weight = ray_weight;
    const auto next_ray = Method::sampleNextRay(path_length, ray, bxdf, intersection,
                                                &ray_weight, &next_ray_weight,
                                                sampler);
    if (!next_ray.isAlive())
      break;
    ++path_length;

    evalExplicitConnection(world, &ray.direction(), bxdf, intersection,
                           light_contribution, ray_weight, camera, memory_pool);

    // Update the ray
    ray = next_ray;
    ray_weight = next_ray_weight;
    // Clear memory
    memory_pool.reset();
  }
  memory_pool.reset();
}

} // namespace nanairo
