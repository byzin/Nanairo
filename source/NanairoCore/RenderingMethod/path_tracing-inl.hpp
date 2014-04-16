/*!
  \file path_tracing-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_PATH_TRACING_INL_HPP_
#define _NANAIRO_PATH_TRACING_INL_HPP_

#include "path_tracing.hpp"
// Standard C++ library
#include <cstdint>
#include <functional>
#include <future>
#include <tuple>
#include <utility>
// Qt
#include <QString>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/algorithm.hpp"
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
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
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
#include "NanairoCore/Utility/floating_point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
PathTracing<kSampleSize>::PathTracing(const SceneSettings& settings) :
    RenderingMethod<kSampleSize>(settings)
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void PathTracing<kSampleSize>::render(System& system,
                                      Scene& scene,
                                      const Wavelengths& sampled_wavelengths)
{
  traceCameraPath(system, scene, sampled_wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void PathTracing<kSampleSize>::evaluateExplicitConnection(
    const World& world,
    const Ray& ray,
    const ShaderPointer& bxdf,
    const IntersectionInfo& intersection,
    const Spectra& ray_weight,
    Sampler& sampler,
    MemoryPool& memory_pool,
    Spectra* contribution) const
{
#ifdef NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY
  return;
#endif // NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY

  if (bxdf->type() == ShaderType::Specular)
    return;

  const auto& wavelengths = ray_weight.wavelengths();
  const auto& normal = intersection.normal();

  // Select a light source and sample a point on the light source
  const auto& sampled_light_source = world.sampleLightSource(sampler);
  const auto light_source = sampled_light_source.object();
  const auto light_point = light_source->geometry().samplePoint(sampler);

  // Make a shadow ray
  const auto ray_epsilon = Method::rayCastEpsilon() * normal;
  ZISC_ASSERT(!isZeroVector(ray_epsilon),
              "Ray epsilon must not be zero vector.");
  const auto ray_origin = intersection.point() + ray_epsilon;
  const auto diff = std::get<0>(light_point).point() - ray_origin;
  const auto ray_direction = diff.normalized();
  const Float cos_theta_no = zisc::dot(normal, ray_direction);
  if (cos_theta_no < 0.0)
    return;
  const Ray shadow_ray{ray_origin, ray_direction};

  // Check the visibility of the light source
  const auto diff2 = diff.squareNorm();
  ZISC_ASSERT(0.0 < diff2, "Diff^2 must be greater than 0.");
  constexpr Float diff_epsilon = 0.0001;
  constexpr Float diff_extension = 1.0 + zisc::power<2>(diff_epsilon);
  static_assert(1.0 < diff_extension,
                "Diff extension must be greater than 1.");
  const Float max_diff2 = diff2 * diff_extension;
  const auto shadow_intersection = 
      Method::castRay(world, shadow_ray, max_diff2); 
  if (shadow_intersection.object() != light_source || 
      shadow_intersection.isReverseFace())
    return;

  // Evaluate the surface reflectance 
  const auto result = bxdf->evaluateRadianceAndPdf(&ray.direction(), 
                                                   &ray_direction,
                                                   normal,
                                                   wavelengths);
  const auto& f = std::get<0>(result);
  const auto& direction_pdf = std::get<1>(result);
  ZISC_ASSERT(!hasNegativeFloat(f), "The f of BxDF has negative values.");
  ZISC_ASSERT(0.0 <= direction_pdf, "Pdf must be positive.");

  // Evaluate the light radiance
  const auto& emitter = light_source->material().emitter();
  const auto light = 
      emitter.makeLight(shadow_intersection.textureCoordinate(),
                        wavelengths, 
                        memory_pool);
  const auto& shadow_normal = shadow_intersection.normal();
  const auto light_dir = -ray_direction;
  const auto radiance = light->evaluateRadiance(nullptr, 
                                                &light_dir, 
                                                shadow_normal,
                                                wavelengths);

  // Calculate the geometry term
  const Float cos_theta_sni = zisc::dot(shadow_normal, light_dir);
  const Float geometry_term = cos_theta_sni * cos_theta_no / diff2;
  ZISC_ASSERT(0.0 < geometry_term, 
              "Geometry term must be greater than 0.");

  // Calculate the MIS weight
  const Float inverse_selection_pdf = 
      sampled_light_source.inverseWeight() *
      std::get<0>(light_point).inversePdf();
#ifndef NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY
  const Float mis_weight = 
      calcMisWeight(direction_pdf, inverse_selection_pdf);
#else // NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY
  constexpr Float mis_weight = 1.0;
#endif // NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY

  // Calculate the contribution
  const auto c = ray_weight * f * radiance * 
                 (geometry_term * inverse_selection_pdf * mis_weight);
  ZISC_ASSERT(!hasNegativeFloat(c), 
              "The contribution has negative values.");
  *contribution += c; 
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> template <bool Mis> inline
void PathTracing<kSampleSize>::evaluateImplicitConnection(
    const World& world,
    const Ray& ray,
    const Float inverse_direction_pdf,
    const IntersectionInfo& intersection,
    const Spectra& ray_weight,
    MemoryPool& memory_pool,
    Spectra* contribution) const
{
#ifdef NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY
  return;
#endif // NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY

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
  const auto radiance = 
      light->evaluateRadiance(&vin, nullptr, normal, wavelengths);

  // Calculate the MIS weight
#ifndef NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY
  const Float selection_pdf = world.lightSourceSampler().getPdf(object) /
                              object->geometry().surfaceArea();
  const Float mis_weight = (Mis)
      ? calcMisWeight(selection_pdf, inverse_direction_pdf)
      : 1.0;
#else // NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY
  constexpr Float mis_weight = 1.0;
#endif // NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY

  // Calculate the contribution
  const auto c = radiance * ray_weight * mis_weight;
  ZISC_ASSERT(!hasNegativeFloat(c), 
              "The contribution has negative values.");
  *contribution += c;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
Ray PathTracing<kSampleSize>::generateRay(
    const CameraModel& camera,
    const uint x, 
    const uint y,
    Sampler& sampler,
    MemoryPool& memory_pool,
    Spectra* weight,
    Float* inverse_direction_pdf) const
{
  const auto& wavelengths = weight->wavelengths();
  // Sample a ray origin
  const auto& lens_point = camera.sampledLensPoint();
  // sample a ray direction
  const auto sensor = camera.makeSensor(x, y, wavelengths, memory_pool);
  const auto result = 
      sensor->sample(nullptr, camera.normal(), wavelengths, sampler);
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
template <uint kSampleSize> inline
void PathTracing<kSampleSize>::initialize(const SceneSettings& /* settings */)
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
Float PathTracing<kSampleSize>::calcMisWeight(
    const Float pdf1, 
    const Float inverse_pdf2) const
{
  const Float p = zisc::power<kMisHeuristicBeta>(pdf1 * inverse_pdf2);
  return 1.0 / (p + 1.0);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PathTracing<kSampleSize>::traceCameraPath(
    System& system, 
    Scene& scene,
    const Wavelengths& sampled_wavelengths)
{
  auto& sampler = system.globalSampler();

  // Set camera
  auto& camera = scene.camera();
  camera.sampleLensPoint(sampler);
  camera.jitter(sampler);

  std::function<void (const int, const uint)> trace_camera_path{
  [this, &system, &scene, &sampled_wavelengths]
  (const int thread_id, const uint y)
  {
    const auto& camera = scene.camera();
    for (uint x = 0; x < camera.widthResolution(); ++x) {
      traceCameraPath(system, scene, sampled_wavelengths, thread_id, x, y);
    }
  }};

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  const uint end = camera.heightResolution();
  auto result = thread_pool.loop(std::move(trace_camera_path), start, end);
  result.get();
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void PathTracing<kSampleSize>::traceCameraPath(
    System& system,
    Scene& scene,
    const Wavelengths& sampled_wavelengths,
    const int thread_id,
    const uint x,
    const uint y)
{
  // System
  auto& sampler = system.threadSampler(thread_id);
  auto& memory_pool = system.threadMemoryPool(thread_id);
  // Scene
  const auto& world = scene.world();
  auto& camera = scene.camera();
  // Trace info
  const auto& wavelengths = sampled_wavelengths.wavelengths();
  const auto wavelength_weight = makeSampledSpectra(sampled_wavelengths);
  Spectra contribution{wavelengths};
  IntersectionInfo intersection;
  uint path_length = 1;
  bool wavelength_is_selected = false;

  // Generate a camera ray
  Float inverse_direction_pdf;
  auto ray_weight = wavelength_weight;
  auto ray = generateRay(camera, x, y, sampler, memory_pool, 
                         &ray_weight, &inverse_direction_pdf);

  while (true) {
    // Cast the ray
    intersection = Method::castRay(world, ray);
    if (!intersection.isIntersected())
      break;

    if (path_length == 1) {
      constexpr bool mis = false;
      evaluateImplicitConnection<mis>(world, ray, inverse_direction_pdf, 
                                      intersection, ray_weight, 
                                      memory_pool, &contribution);
    }
    else {
      constexpr bool mis = true;
      evaluateImplicitConnection<mis>(world, ray, inverse_direction_pdf, 
                                      intersection, ray_weight, 
                                      memory_pool, &contribution);
    }

    // Get a BxDF of the surface
    const auto& material = intersection.object()->material();
    const auto& surface = material.surface();
    const auto bxdf = surface.makeBxdf(intersection.textureCoordinate(), 
                                       intersection.isReverseFace(),
                                       wavelengths, 
                                       memory_pool);
    Method::updateSelectedWavelengthInfo(bxdf, 
                                         &ray_weight, 
                                         &wavelength_is_selected);

    // Sample next ray
    auto next_ray_weight = ray_weight;
    const auto next_ray = 
        Method::sampleNextRay(path_length, ray, bxdf, intersection, 
                              &ray_weight, &next_ray_weight, sampler,
                              &inverse_direction_pdf);
    if (!next_ray.isAlive())
      break;

    ++path_length;

    evaluateExplicitConnection(world, ray, bxdf, intersection, 
                               ray_weight, sampler, memory_pool, 
                               &contribution);

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

#endif // _NANAIRO_PATH_TRACING_INL_HPP_
