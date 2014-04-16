/*!
  \file agglomerative_treelet_restructuring_bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_AGGLOMERATIVE_TREELET_RESTRUCTURING_BVH_HPP_
#define _NANAIRO_AGGLOMERATIVE_TREELET_RESTRUCTURING_BVH_HPP_

// Standard C++ library
#include <array>
#include <tuple>
#include <vector>
// Nanairo
#include "bvh.hpp"
#include "bvh_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class SceneSettings;
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
  AgglomerativeTreeletRestructuringBvh(const SceneSettings& settings, 
                                       const QString& prefix);

 private:
  //! Build a parent-child relationship
  void buildRelationship(const uint32 parent_index,
                         const uint32 left_child_index,
                         const uint32 right_child_index,
                         std::vector<BvhNode>& tree) const;

  //! Build a treelet restructuring BVH
  void constructBvh(System& system,
                    const std::vector<Object>& object_list,
                    std::vector<BvhNode>& tree) const override;

  //! Construct the optimal binary tree
  void constructOptimalTree(std::vector<uint32>& inner_index_list,
                            std::vector<uint32>& leaf_index_list,
                            std::vector<Float>& distance_matrix,
                            std::vector<BvhNode>& tree) const;

  //! Return the row column of the minimum distance
  std::tuple<uint, uint> findBestMatch(
      const uint n, 
      const std::vector<Float>& distance_matrix) const;

  //! Form a treelet from the root
  void formTreelet(const uint treelet_size,
                   const uint32 root_index,
                   const std::vector<BvhNode>& tree,
                   std::vector<uint>& inner_index_list,
                   std::vector<uint>& leaf_index_list) const;

  //! Return the surface area of the combined bounding box
  Float getNodeDistance(const BvhNode& node1, const BvhNode& node2) const;

  //! Initialize
  void initialize(const SceneSettings& settings, const QString& prefix);

  //! Initialize the distance matrix, calculate the distances
  void initializeDistanceMatrix(const std::vector<uint>& leaf_index_list,
                                const std::vector<BvhNode>& tree,
                                std::vector<Float>& distance_matrix) const;

  //! Return the optimization loop count
  uint optimizationLoopCount() const;

  //! Restructure a treelet
  template <bool multithreading>
  uint restructureTreelet(System& system,
                          const uint32 index, 
                          std::vector<uint>& inner_index_list,
                          std::vector<uint>& leaf_index_list,
                          std::vector<Float>& distance_matrix,
                          std::vector<BvhNode>& tree) const;

  //! Return the treelet size
  uint treeletSize() const;

  //! Update the distance matrix
  void updateDistanceMatrix(const std::vector<uint>& leaf_index_list,
                            const std::vector<BvhNode>& tree,
                            const uint row,
                            const uint column,
                            std::vector<Float>& distance_matrix) const;


  uint treelet_size_;
  uint optimization_loop_count_;
};

//! \} Core

} // namespace nanairo

#endif // _NANAIRO_AGGLOMERATIVE_TREELET_RESTRUCTURING_BVH_HPP_
