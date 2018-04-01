/*!
  \file agglomerative_treelet_restructuring_bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_AGGLOMERATIVE_TREELET_RESTRUCTURING_BVH_HPP
#define NANAIRO_AGGLOMERATIVE_TREELET_RESTRUCTURING_BVH_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <tuple>
#include <vector>
// Nanairo
#include "bvh.hpp"
#include "bvh_building_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  For the details of this algorithm,
  please see the paper entitled
  "Bounding Volume Hierarchy Optimization through Agglomerative Treelet Restructuring"
  */
class AgglomerativeTreeletRestructuringBvh : public Bvh
{
 public:
  //! Create a treelet restructuring BVH
  AgglomerativeTreeletRestructuringBvh(const SettingNodeBase* settings) noexcept;

 private:
  struct RestructuringData
  {
    RestructuringData(const uint treelet_size) noexcept;

    std::vector<uint32> inner_index_list_;
    std::vector<uint32> leaf_index_list_;
    std::vector<Float> distance_matrix_;
  };


  //! Build a parent-child relationship
  void buildRelationship(const uint32 parent_index,
                         const uint32 left_child_index,
                         const uint32 right_child_index,
                         std::vector<BvhBuildingNode>& tree) const noexcept;

  //! Calculate the surface area of the combined bounding box
  Float calcNodeDistance(const BvhBuildingNode& lhs,
                         const BvhBuildingNode& rhs) const noexcept;

  //! Build a treelet restructuring BVH
  void constructBvh(System& system,
                    const std::vector<Object>& object_list,
                    std::vector<BvhBuildingNode>& tree) const noexcept override;

  //! Construct the optimal binary treelet
  void constructOptimalTreelet(RestructuringData& data,
                               std::vector<BvhBuildingNode>& tree) const noexcept;

  //! Return the row column of the minimum distance
  std::tuple<uint, uint> findBestMatch(
      const uint num_of_leafs,
      const std::vector<Float>& distance_matrix) const noexcept;

  //! Form a treelet from the root
  void formTreelet(const uint treelet_size,
                   const uint32 root_index,
                   const std::vector<BvhBuildingNode>& tree,
                   RestructuringData& data) const noexcept;

  //! Initialize
  void initialize(const SettingNodeBase* settings) noexcept;

  //! Initialize the distance matrix, calculate the distances
  void initializeDistanceMatrix(const std::vector<BvhBuildingNode>& tree,
                                RestructuringData& data) const noexcept;

  //! Return the optimization loop count
  uint optimizationLoopCount() const noexcept;

  //! Restructure a treelet
  template <bool threading = false>
  uint restructureTreelet(System& system,
                          const uint32 index,
                          RestructuringData& data,
                          std::vector<BvhBuildingNode>& tree) const noexcept;

  //! Return the treelet size
  uint treeletSize() const noexcept;

  //! Update the distance matrix
  void updateDistanceMatrix(const std::vector<BvhBuildingNode>& tree,
                            const uint row,
                            const uint column,
                            RestructuringData& data) const noexcept;


  uint treelet_size_;
  uint optimization_loop_count_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_AGGLOMERATIVE_TREELET_RESTRUCTURING_BVH_HPP
