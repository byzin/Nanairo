/*!
  \file approximate_agglomerative_clustering_bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_APPROXIMATE_AGGLOMERATIVE_CLUSTERING_BVH_HPP_
#define _NANAIRO_APPROXIMATE_AGGLOMERATIVE_CLUSTERING_BVH_HPP_

// Standard C++ library
#include <array>
#include <tuple>
#include <vector>
#include <utility>
// Nanairo
#include "agglomerative_cluster.hpp"
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
  "Efficient BVH Construction via Approximate Agglomerative Clustering"
  http://graphics.cs.cmu.edu/projects/aac/aac_build.pdf
  */
class ApproximateAgglomerativeClusteringBvh : public Bvh
{
 public:
  //! Create a approximate agglomerative clustering BVH
  ApproximateAgglomerativeClusteringBvh(const SceneSettings& settings, 
                                        const QString& prefix);


 private:
  using ClusterPointer = std::unique_ptr<AgglomerativeCluster>;


  //! Build the tree
  template <bool multithreading>
  std::vector<ClusterPointer> buildTree(System& system,
                                        const uint64 bit,
                                        MortonCodeIterator begin,
                                        MortonCodeIterator end,
                                        std::vector<Float>& distance_matrix) const;

  //! Combine clusters
  void combineClusters(const uint n,
                       std::vector<ClusterPointer>& cluster_list,
                       std::vector<Float>& distance_matrix) const;

  //! Build AAC BVH
  void constructBvh(System& system,
                    const std::vector<Object>& object_list,
                    std::vector<BvhNode>& tree) const override;

  //! Return the delta
  uint delta() const;

  //! Evaluate the reduction function
  uint f(const uint n) const;

  //! Return the pre-calculated f(delta)
  uint fDelta() const;

  //! Find best match
  std::tuple<uint, uint> findBestMatch(
      const uint n,
      const std::vector<Float>& distance_matrix) const;

  //! Initialize
  void initialize(const SceneSettings& settings, const QString& prefix);

  //! Initialize the distance matrix
  void initializeDistanceMatrix(const std::vector<ClusterPointer>& cluster_list,
                                std::vector<Float>& distance_matrix) const;

  //! Set node
  uint32 setNode(const AgglomerativeCluster& cluster, 
                 std::vector<BvhNode>& tree,
                 uint& number) const;

  //! Update the distance matrix
  void updateDistanceMatrix(const std::vector<ClusterPointer>& cluster_list,
                            const uint row,
                            const uint column,
                            std::vector<Float>& distance_matrix) const;


  static constexpr Float kAlpha = 0.5;
  static_assert((0.0 <= kAlpha) && (kAlpha <= 1.0), 
                "## Alpha must be 0.0 <= a <= 1.0 .");


  uint delta_;
  uint f_delta_;
  Float c_,
        k_;
};

//! \} Core

} // namespace nanairo

#endif // _NANAIRO_APPROXIMATE_AGGLOMERATIVE_CLUSTERING_BVH_HPP_
