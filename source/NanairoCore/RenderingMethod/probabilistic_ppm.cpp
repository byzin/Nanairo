/*!
  \file probabilistic_ppm.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "probabilistic_ppm.hpp"
// Standard C++ library
#include <cmath>
#include <future>
#include <thread>
#include <tuple>
#include <utility>
// Qt
#include <QString>
#include <QJsonObject>
#include <QtGlobal>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "rendering_method.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
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
#include "NanairoCore/Utility/unique_pointer.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ProbabilisticPpm::ProbabilisticPpm(const System& system,
                                   const QJsonObject& settings) noexcept :
    RenderingMethod(system, settings),
    photon_map_{system},
    cycle_{0}
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void ProbabilisticPpm::render(System& system,
                              Scene& scene,
                              const Wavelengths& sampled_wavelengths) noexcept
{
  photon_map_.clear();
  updateRadius();
  tracePhoton(system, scene, sampled_wavelengths);
  traceCameraPath(system, scene, sampled_wavelengths);
}

/*!
  */
inline
Float ProbabilisticPpm::calcWeight(const Float t) const noexcept
{
  constexpr Float k = 1.1;
  constexpr Float inverse_k = zisc::invert(k);
  constexpr Float normalizer = zisc::invert(1.0 - (2.0 / (3.0 * k)));
  return normalizer * (1.0 - t * inverse_k);
}

/*!
  \details
  No detailed.
  */
auto ProbabilisticPpm::estimateRadiance(
    const int thread_id,
    const Vector3& vin,
    const IntersectionInfo& intersection,
    const ShaderPointer& bxdf,
    const WavelengthSamples& wavelengths,
    const bool wavelength_is_selected) noexcept -> Spectra
{

  // Search photon cache
  auto& photon_list = thread_photon_list_[thread_id];
  const auto& normal = intersection.normal();
  photon_list.clear();
  photon_map_.search(intersection.point(), normal, radius2_, &photon_list);

  // Estimate radiance
  Spectra radiance{wavelengths, 0.0};
  for (uint i = 0; i < photon_list.size(); ++i) {
    const auto& photon_point = photon_list[i];
    // Evaluate reflectance
    const auto photon_cache = std::get<1>(photon_point);
    const auto vout = -photon_cache->incidentDirection();
    const auto f = bxdf->evalRadiance(&vin, &vout, normal, wavelengths);
    ZISC_ASSERT(!f.hasNegative(), "The f has negative value.");
    // Calculate the photon weight
    const Float distance = zisc::sqrt(std::get<0>(photon_point));
    const Float t = distance * inverse_radius_;
    ZISC_ASSERT(zisc::isInBounds(t, 0.0, 1.0), "The t is out of the range [0, 1).");
    const auto weight = calcWeight(t);
    ZISC_ASSERT(0.0 < weight, "The kernel weight is negative.");
    // Calculate the contribution
    const auto contribution = f * photon_cache->energy() * weight;
    ZISC_ASSERT(!contribution.hasNegative(), "The contribution has negative.");
    if (wavelength_is_selected || photon_cache->wavelengthIsSelected())
      radiance += contribution * wavelengths.primaryInverseProbability();
    else
      radiance += contribution;
  }
  return radiance * inverse_estimation_area_;
}

/*!
  \details
  No detailed.
  */
inline
void ProbabilisticPpm::evalImplicitConnection(
    const Ray& ray,
    const IntersectionInfo& intersection,
    const Spectra& camera_contribution,
    const Spectra& ray_weight,
    MemoryPool& memory_pool,
    Spectra* contribution) const noexcept
{
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

  // Calculate the contribution
  const auto c = camera_contribution * ray_weight * radiance;
  ZISC_ASSERT(!c.hasNegative(), "The contribution has negative value.");
  *contribution += c;
}

/*!
  */
inline
constexpr uint ProbabilisticPpm::expectedMaxReflectionCount() noexcept
{
  return 16;
}

/*!
  \details
  No detailed.
  */
inline
auto ProbabilisticPpm::generatePhoton(
    const World& world,
    Spectra* light_contribution,
    Sampler& sampler,
    MemoryPool& memory_pool) const noexcept -> Photon
{
  const auto& wavelengths = light_contribution->wavelengths();
  // Sample a light point
  const auto& light_source_info = world.sampleLightSource(sampler);
  const auto light_source = light_source_info.object();
  const auto light_point_info = light_source->shape().samplePoint(sampler);
  const auto& point = std::get<0>(light_point_info);
  const auto& normal = std::get<1>(light_point_info);
  const auto& texture_coordinate = std::get<2>(light_point_info);
  ZISC_ASSERT(0.0 < point.pdf(), "The point pdf is negative.");

  // Sample a direction
  const auto& emitter = light_source->material().emitter();
  const IntersectionInfo intersection{point.point(),
                                      normal,
                                      texture_coordinate,
                                      light_source,
                                      false};
  const auto light = emitter.makeLight(intersection.textureCoordinate(),
                                       wavelengths,
                                       memory_pool);
  const auto result = light->sample(nullptr, normal, wavelengths, sampler);
  const auto& sampled_vout = std::get<0>(result);
  ZISC_ASSERT(0.0 < sampled_vout.pdf(), "The vout pdf is negative.");
  // Evaluate the light contribution
  const auto& w = std::get<1>(result);
  const auto k = (light_source_info.inverseWeight() * point.inversePdf()) *
                 photon_power_scale_;
  ZISC_ASSERT(0.0 < k, "The photon coefficient is negative.");
  *light_contribution = k * w;

  const auto ray_epsilon = Method::rayCastEpsilon() * normal;
  ZISC_ASSERT(!isZeroVector(ray_epsilon), "Ray epsilon is zero vector.");
  return Photon{point.point() + ray_epsilon, sampled_vout.direction()};
}

/*!
  \details
  No detailed.
  */
inline
Ray ProbabilisticPpm::generateRay(
    const CameraModel& camera,
    const uint x,
    const uint y,
    Sampler& sampler,
    MemoryPool& memory_pool,
    Spectra* weight) const noexcept
{
  const auto& wavelengths = weight->wavelengths();
  // Sample a ray origin
  const auto& lens_point = camera.sampledLensPoint();
  // Sample a ray direction
  auto sensor = camera.makeSensor(x, y, wavelengths, memory_pool);
  const auto result = sensor->sample(nullptr, camera.normal(), wavelengths, sampler);
  const auto& sampled_vout = std::get<0>(result);
  const auto& w = std::get<1>(result);
  *weight = *weight * w;
  return Ray{lens_point, sampled_vout.direction()};
}

/*!
  \details
  No detailed.
  */
void ProbabilisticPpm::initialize(const System& system,
                                  const QJsonObject& settings) noexcept
{
  using zisc::cast;
  auto& thread_pool = system.threadPool();

  {
    const auto num_of_photons = SceneValue::toInt<uint>(settings,
                                                        keyword::numOfPhotons);
    if (num_of_photons < thread_pool.numOfThreads())
      qWarning("  PPPM warning: The num of photons is less than the num of threads.");
    num_of_thread_photons_ = num_of_photons / thread_pool.numOfThreads();
    photon_power_scale_ = 
        zisc::invert(cast<Float>(num_of_thread_photons_ * thread_pool.numOfThreads()));
    qInfo("  PPPM num of photons: %ld", cast<long int>(num_of_thread_photons_));
  }
  {
    alpha_ = SceneValue::toFloat<Float>(settings, keyword::radiusReductionRate);
    qInfo("  PPPM radius reduction rate: %f", cast<float>(alpha_));
  }
  {
    const auto initial_radius =
        SceneValue::toFloat<Float>(settings, keyword::photonSearchRadius);
    radius2_ = zisc::power<2>(initial_radius) / alpha_;
    qInfo("  PPPM photon search radius: %f", cast<float>(initial_radius));
  }

  // Set a clear function
  {
    const auto radius2 = radius2_;
    auto clear_function = [this, radius2]()
    {
      cycle_ = 0;
      radius2_ = radius2;
    };
    Method::setClearFunction(clear_function);
  }

  photon_map_.reserve(num_of_thread_photons_ * expectedMaxReflectionCount());
  thread_photon_list_.resize(thread_pool.numOfThreads());

  {
    const auto k = SceneValue::toInt<uint>(settings, keyword::kNearestNeighbor);
    qInfo("  PPPM the k of knn: %ld", cast<long int>(k));
    for (auto& photon_list : thread_photon_list_)
      photon_list.setK(k);
  }
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
    const Wavelengths& sampled_wavelengths) noexcept
{
  auto& sampler = system.globalSampler();

  // Set camera
  auto& camera = scene.camera();
  camera.sampleLensPoint(sampler);
  camera.jitter(sampler);

  auto trace_camera_path =
  [this, &system, &scene, &sampled_wavelengths](const int thread_id, const uint y)
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
void ProbabilisticPpm::traceCameraPath(
    System& system,
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
  uint path_length = 1;
  bool wavelength_is_selected = false;

  // Generate a camera ray
  Spectra ray_weight{wavelengths, 1.0};
  auto ray = generateRay(camera, x, y, sampler, memory_pool, &camera_contribution);

  while (ray.isAlive()) {
    // Cast the ray
    const auto intersection = Method::castRay(world, ray);
    if (!intersection.isIntersected())
      break;

    evalImplicitConnection(ray, intersection, camera_contribution, ray_weight,
                           memory_pool, &contribution);

    // Evaluate material
    const auto& material = intersection.object()->material();
    const auto& surface = material.surface();
    const auto bxdf = surface.makeBxdf(intersection.textureCoordinate(),
                                       intersection.isReverseFace(),
                                       wavelengths,
                                       sampler,
                                       memory_pool);
    wavelength_is_selected = wavelength_is_selected || bxdf->wavelengthIsSelected();

    if (!surfaceHasPhotonMap(bxdf)) {
      auto previous_weight = ray_weight;
      ray = Method::sampleNextRay(path_length, ray, bxdf, intersection,
                                  &previous_weight, &ray_weight, sampler);
    }
    else {
      const auto& vin = ray.direction();
      const auto radiance = estimateRadiance(thread_id, vin, intersection, bxdf,
                                             wavelengths, wavelength_is_selected);
      contribution += camera_contribution * ray_weight * radiance;
      break;
    }
    memory_pool.reset();
  }
  camera.addContribution(x, y, contribution);
  memory_pool.reset();
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
  auto trace_photon =
  [this, &system, &scene, &sampled_wavelengths]
  (const int thread_id, const uint)
  {
    for (uint number = 0; number < num_of_thread_photons_; ++number)
      tracePhoton(system, scene, sampled_wavelengths, thread_id);
  };
  // Trace photons
  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  const uint end = thread_pool.numOfThreads();
  auto result = thread_pool.enqueueLoop(trace_photon, start, end);
  result.get();
  // Construct a photon map
  photon_map_.construct(system);
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
  auto& memory_pool = system.threadMemoryPool(thread_id);
  // Scene
  const auto& world = scene.world();
  // Trace info
  const auto& wavelengths = sampled_wavelengths.wavelengths();
  Spectra light_contribution{wavelengths, 1.0};
  uint path_length = 2;
  bool wavelength_is_selected = false;

  // Generate a photon
  auto photon_weight = light_contribution;
  auto photon = generatePhoton(world, &light_contribution, sampler, memory_pool);

  while(true) {
    // Phton object intersection test
    const auto intersection = Method::castRay(world, photon);
    if (!intersection.isIntersected())
      break;

    // Evaluate the surface
    const auto& surface = intersection.object()->material().surface();
    const auto bxdf = surface.makeBxdf(intersection.textureCoordinate(),
                                       intersection.isReverseFace(),
                                       wavelengths,
                                       sampler,
                                       memory_pool);
    wavelength_is_selected = wavelength_is_selected || bxdf->wavelengthIsSelected();

    auto next_photon_weight = photon_weight;
    const auto next_photon = Method::sampleNextRay(path_length, photon, bxdf,
                                                   intersection, &photon_weight,
                                                   &next_photon_weight, sampler);
    if (!next_photon.isAlive())
      break;
    ++path_length;

    if (surfaceHasPhotonMap(bxdf)) {
      const auto photon_energy = light_contribution * photon_weight;
      photon_map_.store(thread_id, intersection.point(), photon.direction(),
                        photon_energy, wavelength_is_selected);
    }

    // Update the photon
    photon = next_photon;
    photon_weight = next_photon_weight;
    // Clear memory
    memory_pool.reset();
  }
  memory_pool.reset();
}

/*!
  \details
  No detailed.
  */
inline
void ProbabilisticPpm::updateRadius() noexcept
{
  ++cycle_;
  const Float radius_scale = (zisc::cast<Float>(cycle_ - 1) + alpha_) /
                             zisc::cast<Float>(cycle_);
  radius2_ = radius_scale * radius2_;
  inverse_radius_ = zisc::invert(zisc::sqrt(radius2_));

  constexpr Float k = zisc::invert(zisc::kPi<Float>);
  inverse_estimation_area_ = k / radius2_;
}

} // namespace nanairo
