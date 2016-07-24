/*!
  \file bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_HPP
#define NANAIRO_BVH_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "bvh_node.hpp"
#include "bvh_tree_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;
class QString;

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Ray;
class Object;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Bvh
{
 public:
  //! Initialize BVH
  Bvh(const QJsonObject& settings) noexcept;

  //! Finalize BVH
  virtual ~Bvh() noexcept;


  //! Build BVH
  void construct(System& system, std::vector<Object>&& object_list) noexcept;

  //! Return the BVH byte size
  std::size_t getBvhSize() const noexcept;

  //! Cast the ray and find the intersection closest to the ray origin
  IntersectionInfo castRay(const Ray& ray, const Float max_distance2) const noexcept;

  //! Return the object list
  const std::vector<Object>& objectList() const noexcept;

  //! Return the tree of BVH
  const std::vector<BvhTreeNode>& bvhTree() const noexcept;

 protected:
  //! Build BVH
  virtual void constructBvh(System& system,
                            const std::vector<Object>& object_list,
                            std::vector<BvhNode>& tree) const noexcept = 0;

  //! Set the bounding box of the node
  template <bool multithreading>
  static void setBoundingBox(System& system,
                             std::vector<BvhNode>& tree, 
                             const uint32 index = 0) noexcept;

 private:
  //! Return the end index of the ray traversal
  uint32 endIndex() const noexcept;

  //! Set the tree node and the object list
  void setTreeInfo(const std::vector<BvhNode>& tree,
                   std::vector<Object>& object_list,
                   const uint32 failure_next_index,
                   const uint32 index = 0) noexcept;

  //! Set
  void setUniqueObject(std::vector<Object>& object_list) noexcept;

  //! Sort nodes by the search order
  void sortTreeNode(std::vector<BvhNode>& tree) const noexcept;

  //! Sort nodes by the search order
  void sortTreeNode(const std::vector<BvhNode>& old_tree, 
                    std::vector<BvhNode>& tree,
                    const uint32 old_index,
                    uint32& index) const noexcept;

  //! Test ray-objects of a leaf node intersection
  void testRayObjectsIntersection(const Ray& ray,
                                  const BvhTreeNode& leaf_node,
                                  IntersectionInfo* intersection,
                                  Float* shortest_distance2) const noexcept;


  std::vector<BvhTreeNode> tree_;
  std::vector<Object> object_list_;
  uint32 end_index_;

 protected:
  //! Return the max number of objects per node
  static constexpr uint maxNumOfObjectsPerNode() noexcept
  {
    return kMaxNumOfObjectsPerNode;
  }

  //! Check if multi-threading is enabled
  static constexpr bool multithreadingIsEnabled() noexcept
  {
    return true;
  }
};

//! Make BVH
UniquePointer<Bvh> makeBvh(const QJsonObject& settings) noexcept;

//! \} Core

} // namespace nanairo

#include "bvh-inl.hpp"

#endif // NANAIRO_BVH_HPP
