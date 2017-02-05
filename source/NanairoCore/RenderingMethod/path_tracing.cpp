/*!
  \file path_tracing.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "path_tracing.hpp"
// Standard C++ library
#include <future>
#include <thread>
#include <tuple>
#include <utility>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/arithmetic_array.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
#include "NanairoCore/Sampling/light_source_sampler.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
PathTracing::PathTracing(const System& system,
                         const QJsonObject& settings) noexcept :
    RenderingMethod(system, settings)
{
  initialize(settings);
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
    MemoryPool& memory_pool,
    Spectra* contribution) const noexcept
{
  if (!explicitConnectionIsEnabled())
    return;
  if (bxdf->type() == ShaderType::Specular)
    return;

  // Select a light source and sample a point on the light source
  const auto& sampled_light_source = world.sampleLightSource(sampler);
  const auto light_source = sampled_light_source.object();
  const auto light_point_info = light_source->shape().samplePoint(sampler);
  const auto& light_point = std::get<0>(light_point_info);

  // Make a shadow ray
  const auto& normal = intersection.normal();
  const auto shadow_ray = Method::makeShadowRay(intersection.point(),
                                                light_point.point(),
                                                normal);
  const Float cos_no = zisc::dot(normal, shadow_ray.direction());
  // If the shadow ray hits the back face of the light source, quit connectino
  if (cos_no < 0.0)
    return;

  // Check the visibility of the light source
  const Float diff2 = (light_point.point() - shadow_ray.origin()).squareNorm();
  ZISC_ASSERT(0.0 < diff2, "The diff2 isn't greater than 0.");
  const Float max_shadow_ray_distance = Method::calcShadowRayDistance(diff2);
  const auto shadow_intersection = Method::castRay(world,
                                                   shadow_ray,
                                                   max_shadow_ray_distance);
  if (shadow_intersection.object() != light_source ||
      shadow_intersection.isReverseFace())
    return;

  // Evaluate the surface reflectance
  const auto& wavelengths = ray_weight.wavelengths();
  const auto result = bxdf->evalRadianceAndPdf(&ray.direction(),
                                               &shadow_ray.direction(),
                                               normal,
                                               wavelengths);
  const auto& f = std::get<0>(result);
  const auto& direction_pdf = std::get<1>(result);
  ZISC_ASSERT(!f.hasNegative(), "The f of BxDF has negative values.");
  ZISC_ASSERT(0.0 <= direction_pdf, "Pdf isn't positive.");

  // Evaluate the light radiance
  const auto& emitter = light_source->material().emitter();
  const auto light = emitter.makeLight(shadow_intersection.textureCoordinate(),
                                       wavelengths,
                                       memory_pool);
  const auto& shadow_normal = shadow_intersection.normal();
  const auto light_dir = -shadow_ray.direction();
  const auto radiance = light->evalRadiance(nullptr,
                                            &light_dir,
                                            shadow_normal,
                                            wavelengths);

  // Calculate the geometry term
  const Float cos_sni = zisc::dot(shadow_normal, light_dir);
  const Float geometry_term = cos_sni * cos_no / diff2;
  ZISC_ASSERT(0.0 < geometry_term, "Geometry term is negative.");

  // Calculate the MIS weight
  const Float inverse_selection_pdf = sampled_light_source.inverseWeight() *
                                      light_point.inversePdf();
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
    const World& world,
    const Ray& ray,
    const Float inverse_direction_pdf,
    const IntersectionInfo& intersection,
    const Spectra& camera_contribution,
    const Spectra& ray_weight,
    const bool mis,
    MemoryPool& memory_pool,
    Spectra* contribution) const noexcept
{
  if (!implicitConnectionIsEnabled())
    return;

  const auto object = intersection.object();
  const auto& material = object->material();
  if (!material.isLightSource() || intersection.isReverseFace())
    return;

  const auto& wavelengths = ray_weight.wavelengths();
  const auto& vin = ray.direction();
  const auto& normal = intersection.normal();

  // Get the light
  const auto& emitter = material.emitter();
  const auto light = emitter.makeLight(intersection.textureCoordinate(),
                                       wavelengths,
                                       memory_pool);

  // Evaluate the radiance
  const auto radiance = light->evalRadiance(&vin, nullptr, normal, wavelengths);

  // Calculate the MIS weight
  const Float selection_pdf = world.lightSourceSampler().getPdf(object) /
                              object->shape().surfaceArea();
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
constexpr bool PathTracing::explicitConnectionIsEnabled() noexcept
{
  return CoreConfig::pathTracingExplicitConnectionIsEnabled();
}

/*!
  */
inline
constexpr bool PathTracing::implicitConnectionIsEnabled() noexcept
{
  return CoreConfig::pathTracingImplicitConnectionIsEnabled();
}

/*!
  \details
  No detailed.
  */
inline
Ray PathTracing::generateRay(const CameraModel& camera,
                             const uint x,
                             const uint y,
                             Sampler& sampler,
                             MemoryPool& memory_pool,
                             Spectra* weight,
                             Float* inverse_direction_pdf) const noexcept
{
  const auto& wavelengths = weight->wavelengths();
  // Sample a ray origin
  const auto& lens_point = camera.sampledLensPoint();
  // Sample a ray direction
  const auto sensor = camera.makeSensor(x, y, wavelengths, memory_pool);
  const auto result = sensor->sample(nullptr, camera.normal(), wavelengths, sampler);
  const auto& sampled_vout = std::get<0>(result);
  const auto& w = std::get<1>(result);
  *weight = *weight * w;

  *inverse_direction_pdf = sampled_vout.inversePdf();
  return Ray{lens_point, sampled_vout.direction()};
}

/*!
  \details
  No detailed.
  */
void PathTracing::initialize(const QJsonObject& /* settings */) noexcept
{
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

  // Set camera
  auto& camera = scene.camera();
  camera.sampleLensPoint(sampler);
  camera.jitter(sampler);

  auto trace_camera_path =
  [this, &system, &scene, &sampled_wavelengths] (const int thread_id, const uint y)
  {
    const auto& c = scene.camera();
    for (uint x = 0; x < c.widthResolution(); ++x)
      traceCameraPath(system, scene, sampled_wavelengths, thread_id, x, y);
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  const uint end = camera.heightResolution();
  auto result = thread_pool.enqueueLoop(trace_camera_path, start, end);
  result.get();
}

/*!
  \details
  No detailed.
  */
void PathTracing::traceCameraPath(System& system,
                                  Scene& scene,
                                  const Wavelengths& sampled_wavelengths,
                                  const int thread_id,
                                  const uint x,
                                  const uint y) noexcept
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
  auto ray = generateRay(camera, x, y, sampler, memory_pool,
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
    const auto bxdf = surface.makeBxdf(intersection.textureCoordinate(), 
                                       intersection.isReverseFace(),
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
  camera.addContribution(x, y, contribution);
  // Clear memory
  memory_pool.reset();
}

} // namespace nanairo
