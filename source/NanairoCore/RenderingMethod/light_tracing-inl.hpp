/*!
  \file light_tracing-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_TRACING_INL_HPP
#define NANAIRO_LIGHT_TRACING_INL_HPP

#include "light_tracing.hpp"
// Standard C++ library
#include <bitset>
#include <cstdint>
#include <functional>
#include <future>
#include <tuple>
#include <utility>
#include <vector>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/algorithm.hpp"
#include "zisc/arithmetic_array.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
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
LightTracing<kSampleSize>::LightTracing(const System& system,
                                        const QJsonObject& settings) noexcept :
    RenderingMethod<kSampleSize>(settings)
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void LightTracing<kSampleSize>::render(System& system,
                                       Scene& scene,
                                       const Wavelengths& sampled_wavelengths) noexcept
{
  traceLightPath(system, scene, sampled_wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void LightTracing<kSampleSize>::evaluateExplicitConnection(
    const World& world,
    const CameraModel& camera,
    const int thread_id,
    const ShaderPointer& bxdf,
    const IntersectionInfo& intersection,
    const Vector3* vin,
    const Spectra& ray_weight,
    MemoryPool& memory_pool) noexcept
{
  if (bxdf->type() == ShaderType::Specular)
    return;

  const auto& wavelengths = ray_weight.wavelengths();
  const auto& normal = intersection.normal();

  // Make a shadow ray
  const auto ray_epsilon = Method::rayCastEpsilon() * normal;
  ZISC_ASSERT(!isZeroVector(ray_epsilon), 
              "Ray epsilon must not be zero vector.");
  const auto ray_origin = intersection.point() + ray_epsilon;
  const auto diff = camera.sampledLensPoint() - ray_origin;
  const auto ray_direction = diff.normalized();
  const Float cos_theta_no = zisc::dot(normal, ray_direction);
  if (cos_theta_no < 0.0)
    return;
  const Ray shadow_ray{ray_origin, ray_direction};

  // Check the visibility of the camera
  const auto diff2 = diff.squareNorm();
  ZISC_ASSERT(0.0 < diff2, "Diff^2 must be greater than 0.");
  const auto shadow_intersection = 
      Method::castRay(world, shadow_ray, diff2);
  if (shadow_intersection.isIntersected())
    return;

  // Get the pixel location
  uint x = 0,
       y = 0;
  const bool ray_hits_film = 
      camera.getPixelLocation(ray_direction, &x, &y);
  if (!ray_hits_film)
    return;

  // Evaluate the surface reflectance
  const auto f = 
      bxdf->evaluateRadiance(vin, &ray_direction, normal, wavelengths);
  ZISC_ASSERT(!hasNegativeFloat(f), 
              "The f of sensor has negative values.");

  // Evaluate the camera importance
  const auto sensor = camera.makeSensor(x, y, wavelengths, memory_pool);
  const auto camera_dir = -ray_direction;
  const auto importance = sensor->evaluateRadiance(nullptr, 
                                                   &camera_dir,
                                                   camera.normal(),
                                                   wavelengths);

  // Calculate the geometry term
  const Float cos_theta_cni = zisc::dot(camera.normal(), camera_dir);
  const Float geometry_term = (cos_theta_cni * cos_theta_no) / diff2;
  ZISC_ASSERT(0.0 < geometry_term, 
              "Geometry term must be greater than 0.");

  // Calculate the contribution
  const auto contribution = ray_weight * f * importance * geometry_term;
  ZISC_ASSERT(!hasNegativeFloat(contribution), 
              "The contribution has negative values.");
  addLightContribution(camera, thread_id, x, y, contribution);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
Ray LightTracing<kSampleSize>::generateRay(const World& world,
                                           const CameraModel& camera,
                                           const int thread_id,
                                           Sampler& sampler,
                                           MemoryPool& memory_pool,
                                           Spectra* weight) noexcept
{
  const auto& wavelengths = weight->wavelengths();
  // Sample a light point
  const auto& sampled_light_source = world.sampleLightSource(sampler);
  const auto light_source = sampled_light_source.object();
  const auto light_point = light_source->geometry().samplePoint(sampler);
  const auto& point = std::get<0>(light_point);
  const auto& normal = std::get<1>(light_point);
  ZISC_ASSERT(0.0 < point.pdf(), "Invalid point pdf.");

  // Sample a direction
  const auto& emitter = light_source->material().emitter();
  IntersectionInfo intersection{point.point(), normal, light_source, false};
  const auto light = emitter.makeLight(intersection.textureCoordinate(),
                                       wavelengths,
                                       memory_pool);
  const auto result = 
      light->sample(nullptr, normal, wavelengths, sampler);
  const auto& sampled_vout = std::get<0>(result);
  ZISC_ASSERT(0.0 < sampled_vout.pdf(), "Invalid direction pdf.");
  const auto& w = std::get<1>(result);
  *weight = *weight * w;

  // Evaluate the weight
  const auto k = sampled_light_source.inverseWeight() * 
                 point.inversePdf() * 
                 ray_weight_;
  ZISC_ASSERT(0.0 < k, "Invalid light ray coefficient.");
  *weight = k * (*weight);

  // Evaluate the explicit connection
  evaluateExplicitConnection(world, camera, thread_id, light, intersection,
                             nullptr, *weight, memory_pool);

  // Generate a ray
  const auto ray_epsilon = Method::rayCastEpsilon() * normal;
  ZISC_ASSERT(!isZeroVector(ray_epsilon), 
              "Ray epsilon must not be zero vector.");
  return Ray{point.point() + ray_epsilon, sampled_vout.direction()};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void LightTracing<kSampleSize>::addContribution(const System& system,
                                                CameraModel& camera) noexcept
{
  const auto& thread_pool = system.threadPool();
  uint index = 0;
  for (uint thread_id = 0; thread_id < thread_pool.numOfThreads(); ++thread_id) {
    for (uint y = 0; y < camera.heightResolution(); ++y) {
      for (uint x = 0; x < camera.widthResolution(); ++x) {
        if (hasLightContribution(index))
          camera.addContribution(x, y, light_contribution_buffer_[index]);
        ++index;
      }
    }
  }
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void LightTracing<kSampleSize>::addLightContribution(const CameraModel& camera,
                                                     const int thread_id,
                                                     const uint x,
                                                     const uint y,
                                                     const Spectra& contribution) noexcept 
{
  using zisc::cast;
  const uint index = (cast<uint>(thread_id) * camera.heightResolution() + y) * 
                     camera.widthResolution() + x;
  if (!hasLightContribution(index)) {
    light_contribution_buffer_[index] = contribution;
    setLightContributionFlag(index, true);
  }
  else {
    light_contribution_buffer_[index] += contribution;
  }
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void LightTracing<kSampleSize>::clearLightContribution() noexcept
{
  for (auto& flag : light_contribution_flag_)
    flag.reset();
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool LightTracing<kSampleSize>::hasLightContribution(const uint index) const noexcept
{
  const uint i = index / flagBitsetSize();
  const uint position = index - (i * flagBitsetSize());
  return light_contribution_flag_[i][position];
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void LightTracing<kSampleSize>::initialize(const System& system,
                                           const QJsonObject& /* settings */) noexcept
{
  using zisc::cast;

  const auto num_of_threads = system.threadPool().numOfThreads();

  const auto width = system.imageWidthResolution();
  const auto height = system.imageHeightResolution();

  uint num_of_light_rays = width * height;
  num_of_thread_rays_ = num_of_light_rays / num_of_threads;
  ZISC_ASSERT(0 < num_of_thread_rays_, "Invalid the number of rays.");
  num_of_light_rays = num_of_thread_rays_ * num_of_threads;
  ray_weight_ = cast<Float>(width * height) / cast<Float>(num_of_light_rays);
  ZISC_ASSERT(0.0 < ray_weight_, "Invalid ray weight.");

  light_contribution_buffer_.resize(num_of_threads * width * height);
  ZISC_ASSERT(0 < light_contribution_buffer_.size(), "Invalid buffer size.");

  const uint flag_size = ((light_contribution_buffer_.size() % flagBitsetSize()) == 0)
      ? cast<uint>(light_contribution_buffer_.size()) / flagBitsetSize()
      : cast<uint>(light_contribution_buffer_.size()) / flagBitsetSize() + 1;
  light_contribution_flag_.resize(flag_size);
  ZISC_ASSERT(0 < light_contribution_flag_.size(), "Invalid flag size.");
  ZISC_ASSERT(sizeof(light_contribution_flag_[0]) == 8, "Invalid flag data size.");
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
uint LightTracing<kSampleSize>::numOfThreadRays() const noexcept
{
  return num_of_thread_rays_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void LightTracing<kSampleSize>::setLightContributionFlag(const uint index,
                                                         const bool flag) noexcept
{
  const uint i = index / flagBitsetSize();
  const uint position = index - (i * flagBitsetSize());
  light_contribution_flag_[i].set(position, flag);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void LightTracing<kSampleSize>::traceLightPath(System& system,
                                               Scene& scene,
                                               const Wavelengths& sampled_wavelengths) noexcept 
{
  auto& sampler = system.globalSampler();

  // Set camera
  auto& camera = scene.camera();
  camera.sampleLensPoint(sampler);
  camera.jitter(sampler);

  clearLightContribution();

  auto trace_light_path =
  [this, &system, &scene, &sampled_wavelengths](const int thread_id, const uint)
  {
    for (uint i = 0; i < numOfThreadRays(); ++i) {
      traceLightPath(system, scene, sampled_wavelengths, thread_id);
    }
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  const uint end = thread_pool.numOfThreads();
  auto result = thread_pool.enqueueLoop(trace_light_path, start, end);
  result.get();
  addContribution(system, camera);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void LightTracing<kSampleSize>::traceLightPath(System& system,
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
  const auto wavelength_weight = makeSampledSpectra(sampled_wavelengths);
  Spectra contribution{wavelengths};
  IntersectionInfo intersection;
  uint path_length = 1;
  bool wavelength_is_selected = false;

  // Generate a light ray
  auto ray_weight = wavelength_weight;
  auto ray = generateRay(world, camera, thread_id, 
                         sampler, memory_pool, &ray_weight);

  while (true) {
    // Cast the ray
    intersection = Method::castRay(world, ray);
    if (!intersection.isIntersected())
      break;

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
                              &ray_weight, &next_ray_weight, sampler);
    if (!next_ray.isAlive())
      break;

    ++path_length;

    evaluateExplicitConnection(world, camera, thread_id, bxdf, 
                               intersection, &ray.direction(), ray_weight,
                               memory_pool);

    // Update ray
    ray = next_ray;
    ray_weight = next_ray_weight;
    // Clear memory
    memory_pool.reset();
  }
//  camera.addContribution();
  memory_pool.reset();
}

} // namespace nanairo

#endif // _NANAIRO_LIGHT_TRACING_INL_HPP_
