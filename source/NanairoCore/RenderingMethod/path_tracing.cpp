/*!
  \file path_tracing.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "path_tracing.hpp"
// Standard C++ library
#include <future>
#include <thread>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_pool.hpp"
#include "zisc/thread_pool.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/light_source_info.hpp"
#include "NanairoCore/Data/ray.hpp"
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
#include "NanairoCore/Sampling/LightSourceSampler/light_source_sampler.hpp"
#include "NanairoCore/Setting/rendering_method_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
PathTracing::PathTracing(System& system,
                         const SettingNodeBase* settings,
                         const Scene& scene) noexcept :
    RenderingMethod(system, settings)
{
  initialize(system, settings, scene);
}

/*!
  */
constexpr bool PathTracing::explicitConnectionIsEnabled() noexcept
{
  return CoreConfig::pathTracingExplicitConnectionIsEnabled();
}
/*!
  */
constexpr bool PathTracing::implicitConnectionIsEnabled() noexcept
{
  return CoreConfig::pathTracingImplicitConnectionIsEnabled();
}

/*!
  \details
  No detailed.
  */
void PathTracing::render(System& system,
                         Scene& scene,
                         const Wavelengths& sampled_wavelengths) noexcept
{
  traceCameraPath(system, scene, sampled_wavelengths);
}

/*!
  \details
  No detailed.
  */
void PathTracing::evalExplicitConnection(
    const World& world,
    const Ray& ray,
    const ShaderPointer& bxdf,
    const IntersectionInfo& intersection,
    const Spectra& camera_contribution,
    const Spectra& ray_weight,
    Sampler& sampler,
    zisc::MemoryPool& memory_pool,
    Spectra* contribution) const noexcept
{
  constexpr bool connection_is_enabled = explicitConnectionIsEnabled();
  if (!connection_is_enabled || (bxdf->type() == ShaderType::Specular))
    return;

  // Select a light source and sample a point on the light source
  const auto& light_sampler = eyePathLightSampler();
  const auto light_source_info = light_sampler.sample(intersection, sampler);
  const auto light_source = light_source_info.object();
  const auto light_point_info = light_source->shape().samplePoint(sampler);

  // Check if the light is in front or back of the surface
  const bool is_in_front = 0.0 < zisc::dot(intersection.normal(),
                                           light_point_info.point() - intersection.point());
  if (!(is_in_front ? bxdf->isReflective() : bxdf->isTransmissive()))
    return;

  // Make a shadow ray
  const auto shadow_ray = Method::makeShadowRay(intersection.point(),
                                                light_point_info.point(),
                                                intersection.normal(),
                                                is_in_front);
  const Float cos_no = (is_in_front)
      ? zisc::dot(intersection.normal(), shadow_ray.direction())
      : -zisc::dot(intersection.normal(), shadow_ray.direction());
  if (cos_no <= 0.0)
    return;

  // Check the visibility of the light source
  const Float diff2 = (light_point_info.point() - shadow_ray.origin()).squareNorm();
  ZISC_ASSERT(0.0 < diff2, "The diff2 isn't greater than 0.");
  const Float max_shadow_ray_distance = Method::calcShadowRayDistance(diff2);
  const auto shadow_intersection = Method::castRay(world,
                                                   shadow_ray,
                                                   max_shadow_ray_distance);
  if (shadow_intersection.object() != light_source ||
      shadow_intersection.isBackFace())
    return;

  // Evaluate the surface reflectance
  const auto& wavelengths = ray_weight.wavelengths();
  const auto result = bxdf->evalRadianceAndPdf(&ray.direction(),
                                               &shadow_ray.direction(),
                                               wavelengths,
                                               &intersection);
  const auto& f = std::get<0>(result);
  const auto& direction_pdf = std::get<1>(result);
  ZISC_ASSERT(!f.hasNegative(), "The f of BxDF has negative values.");
  ZISC_ASSERT(0.0 <= direction_pdf, "Pdf isn't positive.");

  // Evaluate the light radiance
  const auto& emitter = light_source->material().emitter();
  const auto light = emitter.makeLight(shadow_intersection.uv(),
                                       wavelengths,
                                       memory_pool);
  const auto light_dir = -shadow_ray.direction();
  const auto radiance = light->evalRadiance(nullptr,
                                            &light_dir,
                                            wavelengths,
                                            &shadow_intersection);

  // Calculate the geometry term
  const Float cos_sni = zisc::dot(shadow_intersection.normal(), light_dir);
  const Float geometry_term = cos_sni * cos_no / diff2;
  ZISC_ASSERT(0.0 <= geometry_term, "Geometry term is negative.");

  // Calculate the MIS weight
  const Float inverse_selection_pdf = light_source_info.inverseWeight() *
                                      light_point_info.inversePdf();
  const Float mis_weight = implicitConnectionIsEnabled()
      ? calcMisWeight(direction_pdf, inverse_selection_pdf)
      : 1.0;
  // Calculate the contribution
  const auto c = (camera_contribution * ray_weight * f * radiance) *
                 (geometry_term * inverse_selection_pdf * mis_weight);
  ZISC_ASSERT(!c.hasNegative(), "The contribution has negative values.");
  *contribution += c;
}

/*!
  \details
  No detailed.
  */
void PathTracing::evalImplicitConnection(
    const World& /* world */,
    const Ray& ray,
    const Float inverse_direction_pdf,
    const IntersectionInfo& intersection,
    const Spectra& camera_contribution,
    const Spectra& ray_weight,
    const bool mis,
    zisc::MemoryPool& memory_pool,
    Spectra* contribution) const noexcept
{
  constexpr bool connection_is_enabled = implicitConnectionIsEnabled();
  if (!connection_is_enabled)
    return;

  const auto object = intersection.object();
  const auto& material = object->material();
  if (!material.isLightSource() || intersection.isBackFace())
    return;

  const auto& wavelengths = ray_weight.wavelengths();
  const auto& vin = ray.direction();

  // Get the light
  const auto& emitter = material.emitter();
  const auto light = emitter.makeLight(intersection.uv(), wavelengths, memory_pool);

  // Evaluate the radiance
  const auto radiance = light->evalRadiance(&vin,
                                            nullptr,
                                            wavelengths,
                                            &intersection);

  // Calculate the MIS weight
  const auto& light_sampler = eyePathLightSampler();
  const auto light_source_info = light_sampler.getInfo(intersection, object);
  const Float selection_pdf = zisc::invert(light_source_info.inverseWeight() *
                                           object->shape().surfaceArea());
  const Float mis_weight = (explicitConnectionIsEnabled() && mis)
      ? calcMisWeight(selection_pdf, inverse_direction_pdf)
      : 1.0;
  // Calculate the contribution
  const auto c = (camera_contribution * ray_weight * radiance) * mis_weight;
  ZISC_ASSERT(!c.hasNegative(), "The contribution has negative values.");
  *contribution += c;
}

/*!
  */
inline
const LightSourceSampler& PathTracing::eyePathLightSampler() const noexcept
{
  return *eye_path_light_sampler_;
}

/*!
  \details
  No detailed.
  */
inline
Ray PathTracing::generateRay(const CameraModel& camera,
                             const Index2d& pixel_index,
                             Sampler& sampler,
                             zisc::MemoryPool& memory_pool,
                             Spectra* weight,
                             Float* inverse_direction_pdf) const noexcept
{
  const auto& wavelengths = weight->wavelengths();
  // Sample a ray origin
  const auto& lens_point = camera.sampledLensPoint();
  // Sample a ray direction
  const auto sensor = camera.makeSensor(pixel_index, wavelengths, memory_pool);
  const auto result = sensor->sample(nullptr, wavelengths, sampler);
  const auto& sampled_vout = std::get<0>(result);
  const auto& w = std::get<1>(result);
  *weight = *weight * w;

  ZISC_ASSERT(inverse_direction_pdf != nullptr, "The pdf is null.");
  *inverse_direction_pdf = sampled_vout.inversePdf();
  return Ray::makeRay(lens_point, sampled_vout.direction());
}

/*!
  \details
  No detailed.
  */
void PathTracing::initialize(System& system,
                             const SettingNodeBase* settings,
                             const Scene& scene) noexcept
{
  const auto method_settings = castNode<RenderingMethodSettingNode>(settings);
  const auto& parameters = method_settings->pathTracingParameters();

  {
    const auto sampler_type = parameters.eye_path_light_sampler_type_;
    eye_path_light_sampler_ = LightSourceSampler::makeSampler(sampler_type,
                                                              scene.world(),
                                                              system);
  }
}

/*!
  \details
  No detailed.
  */
inline
Float PathTracing::calcMisWeight(const Float pdf1,
                                 const Float inverse_pdf2) const noexcept
{
  const Float p = zisc::power<CoreConfig::misHeuristicBeta()>(pdf1 * inverse_pdf2);
  return zisc::invert(p + 1.0);
}

/*!
  \details
  No detailed.
  */
inline
void PathTracing::traceCameraPath(System& system,
                                  Scene& scene,
                                  const Wavelengths& sampled_wavelengths) noexcept
{
  auto& sampler = system.globalSampler();

  // Init camera
  {
    auto& camera = scene.camera();
    camera.sampleLensPoint(sampler);
    camera.jitter(sampler);
  }

  auto trace_camera_path =
  [this, &system, &scene, &sampled_wavelengths](const int thread_id,
                                                const uint index)
  {
    const auto& camera = scene.camera();
    auto tile = RenderingMethod::getRenderingTile(camera.imageResolution(), index);

    for (uint i = 0; i < tile.numOfPixels(); ++i) {
      const auto& pixel_index = tile.current();
      traceCameraPath(system, scene, sampled_wavelengths, thread_id, pixel_index);
      tile.next();
    }
  };

  {
    const auto& camera = scene.camera();
    auto& thread_pool = system.threadPool();
    constexpr uint start = 0;
    const uint end = RenderingMethod::calcNumOfTiles(camera.imageResolution());
    auto result = thread_pool.enqueueLoop(trace_camera_path, start, end);
    result.get();
  }
}

/*!
  \details
  No detailed.
  */
void PathTracing::traceCameraPath(System& system,
                                  Scene& scene,
                                  const Wavelengths& sampled_wavelengths,
                                  const int thread_id,
                                  const Index2d& pixel_index) noexcept
{
  // System
  auto& sampler = system.threadSampler(thread_id);
  auto& memory_pool = system.threadMemoryPool(thread_id);
  // Scene
  const auto& world = scene.world();
  auto& camera = scene.camera();
  // Trace info
  const auto& wavelengths = sampled_wavelengths.wavelengths();
  auto camera_contribution = makeSampledSpectra(sampled_wavelengths);
  Spectra contribution{wavelengths};
  IntersectionInfo intersection;
  uint path_length = 1;
  bool wavelength_is_selected = false;

  // Generate a camera ray
  Float inverse_direction_pdf;
  Spectra ray_weight{wavelengths, 1.0};
  auto ray = generateRay(camera, pixel_index, sampler, memory_pool,
                         &camera_contribution, &inverse_direction_pdf);

  while (true) {
    // Cast the ray
    intersection = Method::castRay(world, ray);
    if (!intersection.isIntersected())
      break;

    const bool mis = path_length != 1;
    evalImplicitConnection(world, ray, inverse_direction_pdf, intersection,
                           camera_contribution, ray_weight, mis,
                           memory_pool, &contribution);

    // Get a BxDF of the surface
    const auto& material = intersection.object()->material();
    const auto& surface = material.surface();
    const auto bxdf = surface.makeBxdf(intersection,
                                       wavelengths,
                                       sampler,
                                       memory_pool);
    Method::updateSelectedWavelengthInfo(bxdf,
                                         &camera_contribution,
                                         &wavelength_is_selected);

    // Sample next ray
    auto next_ray_weight = ray_weight;
    const auto next_ray = Method::sampleNextRay(path_length, ray, bxdf, intersection,
                                                &ray_weight, &next_ray_weight,
                                                sampler, &inverse_direction_pdf);
    if (!next_ray.isAlive())
      break;
    ++path_length;

    evalExplicitConnection(world, ray, bxdf, intersection,
                           camera_contribution, ray_weight,
                           sampler, memory_pool, &contribution);

    // Update ray
    ray = next_ray;
    ray_weight = next_ray_weight;
    // Clear memory
    memory_pool.reset();
  }
  camera.addContribution(pixel_index, contribution);
  // Clear memory
  memory_pool.reset();
}

} // namespace nanairo
