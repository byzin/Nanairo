/*!
  \file agglomerative_cluster.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_AGGLOMERATIVE_CLUSTER_HPP_
#define _NANAIRO_AGGLOMERATIVE_CLUSTER_HPP_

// Standard C++ library
#include <array>
#include <memory>
#include <vector>
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class BvhNode;
class Object;

/*!
  \details
  No detailed.
  */
class AgglomerativeCluster
{
  using ObjectPointer = const Object*;

 public:
  //! Create a singleton cluster
  AgglomerativeCluster(const BvhNode* leaf_node);

  //! Create a cluster
  AgglomerativeCluster(std::unique_ptr<AgglomerativeCluster>&& left_child_cluster,
                       std::unique_ptr<AgglomerativeCluster>&& right_child_cluster);

  //! Free memory
  ~AgglomerativeCluster();


  //! Return the bounding box
  const Aabb& boundingBox() const;

  //! Return the cluster cost
  Float cost() const;

  //! Check if the cluster is leaf
  bool isLeaf() const;

  //! Return the left child cluster
  const AgglomerativeCluster& leftChildCluster() const;

  //! Return the number of primitives
  uint numOfPrimitives() const;

  //! Return the object list
  std::array<ObjectPointer, kMaxNumOfObjectsPerNode> objectList() const;

  //! Return the right child cluster
  const AgglomerativeCluster& rightChildCluster() const;

 private:
  // Prohibit copy
  AgglomerativeCluster(const AgglomerativeCluster&) = delete;
  AgglomerativeCluster& operator=(const AgglomerativeCluster&) = delete;


  //! Initialize a singleton cluster
  void initializeLeafCluster(const BvhNode* leaf_node);

  //! Initialize a inner cluster
  void initializeInnerCluster(
      std::unique_ptr<AgglomerativeCluster>&& left_child_cluster,
      std::unique_ptr<AgglomerativeCluster>&& right_child_cluster);

  //! Treat this cluster as a leaf
  void setAsLeaf();

  union
  {
    struct
    {
      ObjectPointer* object_list_;
      AgglomerativeCluster* dummy_;
    } leaf_;
    struct
    {
      AgglomerativeCluster* left_child_cluster_;
      AgglomerativeCluster* right_child_cluster_;
    } inner_;
  } data_;
  Aabb bounding_box_;
  uint32 num_of_objects_;
  float cost_;

 private:
  //! Return the cost of a ray-box intersection
  static constexpr Float rayBoxIntersectionCost()
  {
    return 1.2;
  }
};

using ClusterIterator = std::vector<AgglomerativeCluster>::iterator;

//! Calculate the distance between a and b
Float getClusterDistance(const AgglomerativeCluster& a, const AgglomerativeCluster& b);

} // namespace nanairo

#include "agglomerative_cluster-inl.hpp"

#endif // _NANAIRO_AGGLOMERATIVE_CLUSTER_HPP_
