///*!
//  \file contribution_weighted_light_source_sampler.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2018 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef NANAIRO_CONTRIBUTION_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP
//#define NANAIRO_CONTRIBUTION_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP
//
//// Standard C++ library
//#include <memory>
//#include <limits>
//#include <tuple>
//#include <vector>
//// Zisc
//#include "zisc/cumulative_distribution_function.hpp"
//#include "zisc/compensated_summation.hpp"
//// Nanairo
//#include "light_source_sampler.hpp"
//#include "power_weighted_light_source_sampler.hpp"
//#include "NanairoCore/Data/shape_point.hpp"
//#include "NanairoCore/Data/light_source_reference.hpp"
//#include "NanairoCore/Data/ray.hpp"
//#include "NanairoCore/DataStructure/photon_map.hpp"
//
//namespace zisc {
//
//// Forward declaration
//class MemoryPool;
//
//} // namespace zisc
//
////! \addtogroup Core
////! \{
//
//namespace nanairo {
//
//// Forward declaration
//class IntersectionInfo;
//class KnnPhotonList;
//class Object;
//class Sampler;
//class System;
//class World;
//class WavelengthSamples;
//
///*!
//  */
//class ContributionWeightedLightSourceSampler : public LightSourceSampler
//{
//  using LightSourceCdf = PowerWeightedLightSourceSampler::LightSourceCdf;
//  using SamplerData = std::tuple<uint, // Split size
//                                 std::vector<std::vector<LightSourceReference>>,
//                                 std::vector<std::unique_ptr<LightSourceCdf>>>;
//  using Photon = Ray;
//
// public:
//  //! Create a light source sampler
//  ContributionWeightedLightSourceSampler(System& system,
//                                         const World& world,
//                                         const uint max_surface_split,
//                                         const uint num_of_photons) noexcept;
//
//
//  //! Return the light suorce reference by the light source
//  const LightSourceReference& getReference(
//      const IntersectionInfo& info,
//      const Object* light_source) const noexcept override;
//
//  //! Sample a light source
//  const LightSourceReference& sample(const IntersectionInfo& info,
//                                     Sampler& sampler) const noexcept override;
//
// private:
//  //! Find and return the closest intersection of the ray
//  IntersectionInfo castRay(
//      const World& world,
//      const Ray& ray,
//      const Float max_distance = std::numeric_limits<Float>::max()) const noexcept;
//
//  //! Estimate the contributions of light sources
//  void estimateContributions(
//      const PhotonMap& contribution_map,
//      const Point3& point,
//      const Vector3& normal,
//      const Float radius2,
//      KnnPhotonList* contribution_search_list,
//      std::vector<zisc::CompensatedSummation<Float>>* contribution_list) const noexcept;
//
//  //! Generate a photon
//  Photon generatePhoton(const WavelengthSamples& wavelengths,
//                        const Object& light_source,
//                        Sampler& sampler,
//                        zisc::MemoryPool& memory_pool) const noexcept;
//
//  //! Return the index by the st coordinate
//  uint getIndex(const uint split_level, const Point2& st) const noexcept;
//
//  //! Return the light source CDF
//  std::unique_ptr<LightSourceCdf>& getLightSourceCdf(
//      SamplerData& data,
//      const uint index) noexcept;
//
//  //! Return the light source CDF
//  const std::unique_ptr<LightSourceCdf>& getLightSourceCdf(
//      const SamplerData& data,
//      const uint index) const noexcept;
//
//  //! Return the point info of the object
//  ShapePoint getPointInfo(const Object* object,
//                          const uint split_size,
//                          const uint index) const noexcept;
//
//  //! Return the reference list of the sampler
//  std::vector<LightSourceReference>& getReferenceList(
//      SamplerData& data,
//      const uint index) noexcept;
//
//  //! Return the reference list of the sampler
//  const std::vector<LightSourceReference>& getReferenceList(
//      const SamplerData& data,
//      const uint index) const noexcept;
//
//  //! Return the scene area
//  Float getSceneArea(const World& world) const noexcept;
//
//  //! Return the contribution search radius2
//  Float getSearchRadius2(const Object& object,
//                         const uint split_level) const noexcept;
//
//  //! Return the split level of the sampler
//  uint getSplitLevel(const SamplerData& data) const noexcept;
//
//  //! Return the split size of the sampler
//  uint getSplitSize(const uint split_level) const noexcept;
//
//  //! Return the st coordinate by the index
//  Point2 getStCoordinate(const uint split_size, const uint index) const noexcept;
//
//  //! Initialize
//  void initialize(System& system,
//                  const World& world,
//                  const uint max_surface_split,
//                  const uint num_of_photons) noexcept;
//
//  //! Initialize as a empty sampler
//  void initEmptySamplerData(const Object& object,
//                            const uint max_surface_split,
//                            const Float scene_area,
//                            const std::vector<const Object*>& light_source_list,
//                            SamplerData& data) const noexcept;
//
//  //! Make a contribution map
//  std::unique_ptr<PhotonMap> makeContributionMap(
//      System& system,
//      const World& world,
//      const uint num_of_photons) const noexcept;
//
//  //! Make a light source CDF
//  std::unique_ptr<LightSourceCdf> makeLightSourceCdf(
//      const std::vector<LightSourceReference>& reference_list,
//      std::vector<const LightSourceReference*>& ref_pointer_list,
//      std::vector<Float>& pdf_list) const noexcept;
//
//  //! Return the object list of the world
//  const std::vector<Object>& objectList() const noexcept;
//
//  //! Set the contributions
//  void setContributions(
//      const std::vector<zisc::CompensatedSummation<Float>>& contribution_list,
//      std::vector<LightSourceReference>* reference_list) const noexcept;
//
//
//  std::vector<SamplerData> sampler_data_list_;
//  const std::vector<Object>* object_list_;
//};
//
////! \} Core
//
//} // namespace nanairo
//
//#endif // NANAIRO_CONTRIBUTION_WEIGHTED_LIGHT_SOURCE_SAMPLER_HPP
