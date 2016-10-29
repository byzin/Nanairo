/*!
  \file approximate_agglomerative_clustering_bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_APPROXIMATE_AGGLOMERATIVE_CLUSTERING_BVH_HPP
#define NANAIRO_APPROXIMATE_AGGLOMERATIVE_CLUSTERING_BVH_HPP

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
class QJsonObject;

namespace nanairo {

// Forward declaration
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
  ApproximateAgglomerativeClusteringBvh(const QJsonObject& settings) noexcept;


 private:
  using ClusterPointer = std::unique_ptr<AgglomerativeCluster>;


  //! Build the tree
  template <bool threading>
  std::vector<ClusterPointer> buildTree(System& system,
                                        const uint64 bit,
                                        MortonCodeIterator begin,
                                        MortonCodeIterator end,
                                        std::vector<Float>& distance_matrix) const noexcept;

  //! Combine clusters
  void combineClusters(const uint n,
                       std::vector<ClusterPointer>& cluster_list,
                       std::vector<Float>& distance_matrix) const noexcept;

  //! Build AAC BVH
  void constructBvh(System& system,
                    const std::vector<Object>& object_list,
                    std::vector<BvhNode>& tree) const noexcept override;

  //! Return the delta
  uint delta() const noexcept;

  //! Evaluate the reduction function
  uint f(const uint n) const noexcept;

  //! Return the pre-calculated f(delta)
  uint fDelta() const noexcept;

  //! Find best match
  std::tuple<uint, uint> findBestMatch(
      const uint n,
      const std::vector<Float>& distance_matrix) const noexcept;

  //! Initialize
  void initialize(const QJsonObject& settings) noexcept;

  //! Initialize the distance matrix
  void initializeDistanceMatrix(const std::vector<ClusterPointer>& cluster_list,
                                std::vector<Float>& distance_matrix) const noexcept;

  //! Set node
  uint32 setNode(const AgglomerativeCluster& cluster, 
                 std::vector<BvhNode>& tree,
                 uint& number) const noexcept;

  //! Update the distance matrix
  void updateDistanceMatrix(const std::vector<ClusterPointer>& cluster_list,
                            const uint row,
                            const uint column,
                            std::vector<Float>& distance_matrix) const noexcept;


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

#endif // NANAIRO_APPROXIMATE_AGGLOMERATIVE_CLUSTERING_BVH_HPP
