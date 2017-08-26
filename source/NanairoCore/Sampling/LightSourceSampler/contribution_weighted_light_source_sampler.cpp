/*!
  \file contribution_weighted_light_source_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "contribution_weighted_light_source_sampler.hpp"
// Standard C++ library
#include <cmath>
#include <limits>
#include <memory>
#include <tuple>
#include <vector>
// Zisc
#include "zisc/cumulative_distribution_function.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "light_source_sampler.hpp"
#include "power_weighted_light_source_sampler.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/DataStructure/knn_photon_list.hpp"
#include "NanairoCore/DataStructure/photon_map.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  */
ContributionWeightedLightSourceSampler::ContributionWeightedLightSourceSampler(
    System& system,
    const World& world,
    const uint max_surface_split,
    const uint num_of_photons) noexcept :
        object_list_{&world.bvh().objectList()}
{
  initialize(system, world, max_surface_split, num_of_photons);
}

/*!
  */
const LightSourceReference& ContributionWeightedLightSourceSampler::getReference(
    const IntersectionInfo& info,
    const Object* light_source) const noexcept
{
  const auto object = info.object();
  ZISC_ASSERT(object != nullptr, "The object is null.");

  const uint object_index = zisc::cast<uint>(object - &(objectList()[0]));
  const auto& sampler_data = sampler_data_list_[object_index];

  const auto split_level = getSplitLevel(sampler_data);
  const auto i = getIndex(split_level, info.st());

  const auto& reference_list = getReferenceList(sampler_data, i);
  return LightSourceSampler::getReference(reference_list, light_source);
}

/*!
  */
const LightSourceReference& ContributionWeightedLightSourceSampler::sample(
    const IntersectionInfo& info,
    Sampler& sampler) const noexcept
{
  const auto object = info.object();
  ZISC_ASSERT(object != nullptr, "The object is null.");

  const uint object_index = zisc::cast<uint>(object - &(objectList()[0]));
  const auto& sampler_data = sampler_data_list_[object_index];

  const auto split_level = getSplitLevel(sampler_data);
  const auto i = getIndex(split_level, info.st());

  const auto& light_source_cdf = getLightSourceCdf(sampler_data, i);

  const Float y = sampler.sample();
  const auto sampled_light_source = light_source_cdf->inverseFunction(y);

  return *sampled_light_source;
}

/*!
  \details
  No detailed.
  */
inline
IntersectionInfo ContributionWeightedLightSourceSampler::castRay(
    const World& world,
    const Ray& ray,
    const Float max_distance) const noexcept
{
  const auto& bvh = world.bvh();
  return bvh.castRay(ray, max_distance);
}

/*!
  */
void ContributionWeightedLightSourceSampler::estimateContributions(
    const PhotonMap& contribution_map,
    const Point3& point,
    const Vector3& normal,
    const Float radius2,
    KnnPhotonList* contribution_search_list,
    std::vector<zisc::CompensatedSummation<Float>>* contribution_list) const noexcept
{
  // Search contributions
  contribution_search_list->clear();
  contribution_map.search(point, normal, radius2, contribution_search_list);

  // Estimate contribution 
  for (auto& value : *contribution_list) {
    constexpr Float initial_contribution = 1.0e-3;
    value.set(initial_contribution);
  }
  for (uint i = 0; i < contribution_search_list->size(); ++i) {
    const auto& contribution_cache = *std::get<1>((*contribution_search_list)[i]);
    const auto& light_id = contribution_cache.energy();
    const uint light_number = zisc::cast<uint>(light_id.intensity(0));
    ZISC_ASSERT(light_number == zisc::cast<uint>(std::lround(light_id.intensity(0))),
                "The light number is wrong.");
    const Float c = -zisc::dot(contribution_cache.incidentDirection(), normal);
    ZISC_ASSERT(0.0 <= c, "The contribution is minus.");
    (*contribution_list)[light_number].add(c);
  }
}

/*!
  */
auto ContributionWeightedLightSourceSampler::generatePhoton(
    const WavelengthSamples& wavelengths,
    const Object& light_source,
    Sampler& sampler,
    MemoryPool& memory_pool) const noexcept -> Photon
{
  // Sample a point
  const auto sampled_point = light_source.shape().samplePoint(sampler);
  const auto& point = std::get<0>(sampled_point);
  const auto& normal = std::get<1>(sampled_point);
  const auto& uv = std::get<2>(sampled_point);
  ZISC_ASSERT(0.0 < point.pdf(), "The point pdf is negative.");

  // Sample a direction
  const auto& emitter = light_source.material().emitter();
  const IntersectionInfo intersection{point.point(),
                                      normal,
                                      uv,
                                      &light_source,
                                      false};
  const auto light = emitter.makeLight(intersection.textureCoordinate(),
                                       wavelengths,
                                       memory_pool);
  const auto vout_result = light->sample(nullptr, normal, wavelengths, sampler);
  const auto& sampled_vout = std::get<0>(vout_result);
  ZISC_ASSERT(0.0 < sampled_vout.pdf(), "The vout pdf is negative.");

  // Make a photon
//  const auto ray_epsilon = rayCastEpsilon() * normal;
  const auto ray_epsilon = 1.0e-6 * normal;

  ZISC_ASSERT(!isZeroVector(ray_epsilon), "Ray epsilon is zero vector.");
  return Photon{point.point() + ray_epsilon, sampled_vout.direction()};
}

/*!
  */
uint ContributionWeightedLightSourceSampler::getIndex(
    const uint split_level,
    const Point2& st) const noexcept
{
  ZISC_ASSERT(zisc::isInClosedBounds(st[0], 0.0, 1.0), "The s isn't in [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(st[1], 0.0, 1.0), "The t isn't in [0, 1].");
  const Float s = st[0] * zisc::cast<Float>(split_level);
  const Float t = st[1] * zisc::cast<Float>(split_level);

  const uint is = zisc::cast<uint>(s);
  const uint it = zisc::cast<uint>(t);
  const bool is_right_side = 1.0 < (s + t - zisc::cast<Float>(is + it));

  // Calculate the index of split triangle
  const uint index = (2 * is) +
                     (getSplitSize(split_level) - getSplitSize(split_level - it)) +
                     (is_right_side ? 1 : 0);
  ZISC_ASSERT(zisc::isInBounds(index, 0u, getSplitSize(split_level)),
              "The index is out of range [0, split_size].");
  return index;
}

/*!
  */
Point2 ContributionWeightedLightSourceSampler::getStCoordinate(
    const uint split_level,
    const uint index) const noexcept
{
  ZISC_ASSERT(zisc::isInBounds(index, 0u, getSplitSize(split_level)),
              "The index is out of range [0, split_size].");
  // Compute t
  uint it = 0;
  for (it = split_level - 1;
       index < (getSplitSize(split_level) - getSplitSize(split_level - it));
       --it){;}
  ZISC_ASSERT(zisc::isInBounds(it, 0u, split_level), "The t isn't in [0, level).");
  // Compute s
  const uint i = index -
                 (getSplitSize(split_level) - getSplitSize(split_level - it));
  const uint is = i >> 1;
  ZISC_ASSERT(zisc::isInBounds(is, 0u, split_level), "The s isn't in [0, level).");
  // Check right size
  const bool is_right_side = ((i & 1) == 1);

  Point2 st{zisc::cast<Float>(is) + (is_right_side ? 0.75 : 0.25),
            zisc::cast<Float>(it) + (is_right_side ? 0.75 : 0.25)};
  st[0] = st[0] / zisc::cast<Float>(split_level);
  ZISC_ASSERT(zisc::isInClosedBounds(st[0], 0.0, 1.0), "The s isn't in [0, 1].");
  st[1] = st[1] / zisc::cast<Float>(split_level);
  ZISC_ASSERT(zisc::isInClosedBounds(st[1], 0.0, 1.0), "The t isn't in [0, 1].");
  return st;
}

/*!
  */
inline
auto ContributionWeightedLightSourceSampler::getLightSourceCdf(
    SamplerData& data,
    const uint index) noexcept -> std::unique_ptr<LightSourceCdf>&
{
  auto& cdf_list = std::get<2>(data);
  return cdf_list[index];
}

/*!
  */
inline
auto ContributionWeightedLightSourceSampler::getLightSourceCdf(
    const SamplerData& data,
    const uint index) const noexcept -> const std::unique_ptr<LightSourceCdf>&
{
  const auto& cdf_list = std::get<2>(data);
  return cdf_list[index];
}

/*!
  */
ShapePoint ContributionWeightedLightSourceSampler::getPointInfo(
    const Object* object,
    const uint split_level,
    const uint index) const noexcept
{
  const auto& shape = object->shape();
  const auto st = getStCoordinate(split_level, index);
  return shape.getPoint(st);
}

/*!
  */
inline
std::vector<LightSourceReference>&
ContributionWeightedLightSourceSampler::getReferenceList(
    SamplerData& data,
    const uint index) noexcept
{
  auto& reference_list_array = std::get<1>(data);
  return reference_list_array[index];
}

/*!
  */
inline
const std::vector<LightSourceReference>&
ContributionWeightedLightSourceSampler::getReferenceList(
    const SamplerData& data,
    const uint index) const noexcept
{
  const auto& reference_list_array = std::get<1>(data);
  return reference_list_array[index];
}

/*!
  */
Float ContributionWeightedLightSourceSampler::getSceneArea(const World& world) 
    const noexcept
{
  const auto& scene_aabb = (world.bvh().bvhTree())[0].boundingBox();
  const auto scene_diagonal = scene_aabb.maxPoint() - scene_aabb.minPoint();
  ZISC_ASSERT(0.0 < scene_diagonal[0], "The scene diagonal is minus.");
  ZISC_ASSERT(0.0 < scene_diagonal[1], "The scene diagonal is minus.");
  ZISC_ASSERT(0.0 < scene_diagonal[2], "The scene diagonal is minus.");
  Float scene_area = scene_diagonal[0] * scene_diagonal[1] +
                     scene_diagonal[1] * scene_diagonal[2] +
                     scene_diagonal[2] * scene_diagonal[0];
  scene_area = scene_area / 3.0;
  return scene_area;
}

/*!
  */
Float ContributionWeightedLightSourceSampler::getSearchRadius2(
    const Object& object,
    const uint split_level) const noexcept
{
  constexpr Float extention = 1.1;
  const Float radius2 =
      extention * object.shape().surfaceArea() /
      (zisc::kPi<Float> * zisc::cast<Float>(getSplitSize(split_level)));
  return radius2;
}

/*!
  */
inline
uint ContributionWeightedLightSourceSampler::getSplitLevel(const SamplerData& data)
    const noexcept
{
  return std::get<0>(data);
}

/*!
  */
inline
uint ContributionWeightedLightSourceSampler::getSplitSize(const uint split_level)
    const noexcept
{
  return zisc::power<2>(split_level);
}

void ContributionWeightedLightSourceSampler::initialize(
    System& system,
    const World& world,
    const uint max_surface_split,
    const uint num_of_photons) noexcept
{
  const auto& object_list = objectList();
  const auto& light_source_list = world.lightSourceList();
  auto& thread_pool = system.threadPool();

  // Make a contribution map
  const auto contribution_map = makeContributionMap(system, world, num_of_photons);

  // Contribution search list
  std::vector<KnnPhotonList> contribution_search_list_array;
  contribution_search_list_array.resize(thread_pool.numOfThreads());
  for (auto& contribution_search_list : contribution_search_list_array)
    contribution_search_list.setK(num_of_photons / 10);

  // Contribution list
  std::vector<std::vector<zisc::CompensatedSummation<Float>>> contribution_list_array;
  contribution_list_array.resize(thread_pool.numOfThreads());
  for (auto& contribution_list : contribution_list_array)
    contribution_list.resize(light_source_list.size());

  // Reference pointer list
  std::vector<std::vector<const LightSourceReference*>> ref_pointer_list_array;
  ref_pointer_list_array.resize(thread_pool.numOfThreads());
  for (auto& ref_pointer_list : ref_pointer_list_array)
    ref_pointer_list.resize(light_source_list.size());
  // Light source pdf list
  std::vector<std::vector<Float>> pdf_list_array;
  pdf_list_array.resize(thread_pool.numOfThreads());
  for (auto& pdf_list : pdf_list_array)
    pdf_list.resize(light_source_list.size());

  const Float scene_area = getSceneArea(world);

  // Make CDF of light sources per objects
  sampler_data_list_.resize(object_list.size());
  auto make_light_cdf =
  [this, &object_list, &light_source_list, &contribution_map, &contribution_search_list_array, &contribution_list_array, &ref_pointer_list_array, &pdf_list_array, max_surface_split, scene_area]
  (const int thread_id, const uint object_number)
  {
    const auto& object = object_list[object_number];
    auto& data = sampler_data_list_[object_number];
    initEmptySamplerData(object, max_surface_split, scene_area, light_source_list, data);

    // Initialize reference list and cdf list
    const uint split_level = getSplitLevel(data);
    const Float radius2 = getSearchRadius2(object, split_level);
    for (uint i = 0; i < getSplitSize(split_level); ++i) {
      const auto point_info = getPointInfo(&object, split_level, i);
      // Set light source reference
      auto& contribution_list = contribution_list_array[thread_id];
      auto& contribution_search_list = contribution_search_list_array[thread_id];
      estimateContributions(*contribution_map,
                            point_info.point(),
                            point_info.normal(),
                            radius2,
                            &contribution_search_list,
                            &contribution_list);
      auto& reference_list = getReferenceList(data, i);
      setContributions(contribution_list, &reference_list);
      // Set CDF
      auto& cdf = getLightSourceCdf(data, i);
      auto& ref_pointer_list = ref_pointer_list_array[thread_id];
      auto& pdf_list = pdf_list_array[thread_id];
      cdf = makeLightSourceCdf(reference_list, ref_pointer_list, pdf_list);
    }
  };
  constexpr uint start = 0;
  const uint end = zisc::cast<uint>(object_list.size());
  auto result = thread_pool.enqueueLoop(make_light_cdf, start, end);
  result.get();
}

/*!
  */
void ContributionWeightedLightSourceSampler::initEmptySamplerData(
    const Object& object,
    const uint max_surface_split,
    const Float scene_area,
    const std::vector<const Object*>& light_source_list,
    SamplerData& data) const noexcept
{
  // Initialize split size
  auto& split_level = std::get<0>(data);
  {
    Float k = object.shape().surfaceArea() / scene_area;
    k = zisc::clamp(k, 0.0, 1.0);
    k = 1.0 - zisc::power<2>(1.0 - k);
    split_level = zisc::cast<uint>(
        std::lround(k * zisc::cast<Float>(max_surface_split)));
    constexpr uint min_surface_split = 1;
    split_level = zisc::clamp(split_level, min_surface_split, max_surface_split);
  }
  // Initialize the array of the reference list
  const uint split_size = getSplitSize(split_level);
  auto& reference_list_array = std::get<1>(data);
  reference_list_array.resize(split_size);
  for (auto& reference_list : reference_list_array) {
    reference_list.reserve(light_source_list.size());
    for (const auto light_source : light_source_list)
      reference_list.emplace_back(light_source);
  }
  // Initialize the array of the CDF
  auto& cdf_array = std::get<2>(data);
  cdf_array.resize(split_size);
}

/*!
  */
std::unique_ptr<PhotonMap>
ContributionWeightedLightSourceSampler::makeContributionMap(
    System& system,
    const World& world,
    const uint num_of_photons) const noexcept
{
  WavelengthSamples wavelengths;
  wavelengths[0] = getWavelength(0);
  wavelengths[1] = getWavelength(1);
  wavelengths[2] = getWavelength(2);

  auto contribution_map = std::make_unique<PhotonMap>(system);

  const PowerWeightedLightSourceSampler power_weighted_sampler{world};
  const auto& reference_list = power_weighted_sampler.referenceList();
  contribution_map->clear();
  contribution_map->reserve(num_of_photons);

  auto trace_contribution =
  [this, &system, &world, &wavelengths, &contribution_map, &reference_list, num_of_photons]
  (const int thread_id, const uint light_number)
  {
    auto& sampler = system.threadSampler(thread_id);
    auto& memory_pool = system.threadMemoryPool(thread_id);

    const auto& light_source_reference = reference_list[light_number];
    const uint num_of_light_photons = zisc::cast<uint>(
        light_source_reference.weight() * zisc::cast<Float>(num_of_photons));
    const SampledSpectra light_id{wavelengths, zisc::cast<Float>(light_number)};
    for (uint i = 0; i < num_of_light_photons; ++i) {
      const auto photon = generatePhoton(wavelengths,
                                         *light_source_reference.object(),
                                         sampler,
                                         memory_pool);
      const auto intersection = castRay(world, photon);
      if (intersection.isIntersected()) {
        contribution_map->store(thread_id,
                                intersection.point(),
                                photon.direction(),
                                light_id,
                                false);
      }
      memory_pool.reset();
    }
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  const uint end = zisc::cast<uint>(reference_list.size());
  auto result = thread_pool.enqueueLoop(trace_contribution, start, end);
  result.get();

  contribution_map->construct(system);
  return contribution_map;
}

/*!
  */
auto ContributionWeightedLightSourceSampler::makeLightSourceCdf(
    const std::vector<LightSourceReference>& reference_list,
    std::vector<const LightSourceReference*>& ref_pointer_list,
    std::vector<Float>& pdf_list) const noexcept
        -> std::unique_ptr<LightSourceCdf>
{
  ZISC_ASSERT(ref_pointer_list.size() == reference_list.size(),
              "The size of pointer list is wrong.");
  ZISC_ASSERT(pdf_list.size() == reference_list.size(),
              "The size of pdf list is wrong.");
  for (uint i = 0; i < reference_list.size(); ++i) {
    const auto& light_source_reference = reference_list[i];
    ref_pointer_list[i] = &light_source_reference;
    pdf_list[i] = light_source_reference.weight();
  }

  return std::make_unique<LightSourceCdf>(ref_pointer_list.begin(),
                                          ref_pointer_list.end(),
                                          pdf_list.begin(),
                                          pdf_list.end());
}

/*!
  */
inline
const std::vector<Object>& ContributionWeightedLightSourceSampler::objectList()
    const noexcept
{
  ZISC_ASSERT(object_list_ != nullptr, "The object list is null.");
  return *object_list_;
}

/*!
  */
void ContributionWeightedLightSourceSampler::setContributions(
    const std::vector<zisc::CompensatedSummation<Float>>& contribution_list,
    std::vector<LightSourceReference>* reference_list) const noexcept
{
  // Calculate normalizer
  zisc::CompensatedSummation<Float> sum{0.0};
  for (const auto& contribution : contribution_list)
    sum.add(contribution.get());
  const Float normalizer = zisc::invert(sum.get());
  // Set contributions
  ZISC_ASSERT(contribution_list.size() == reference_list->size(),
              "The sizes of lists aren't same.");
  for (uint i = 0; i < reference_list->size(); ++i) {
    const auto& contribution = contribution_list[i];
    auto& light_reference = (*reference_list)[i];
    light_reference.setWeight(normalizer * contribution.get());
  }
}

} // namespace nanairo
