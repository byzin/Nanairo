/*!
  \file bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_HPP
#define NANAIRO_BVH_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
// Nanairo
#include "bvh_building_node.hpp"
#include "bvh_tree_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Ray;
class Object;
class System;

//! \addtogroup Core
//! \{

enum class BvhType : uint32
{
  kBinaryRadixTree            = zisc::toHash32("BinaryRadixTree"),
  kAgglomerativeTreeletRestructuring
                              = zisc::toHash32("AgglomerativeTreeletRestructuring")
};

/*!
  \details
  No detailed.
  */
class Bvh
{
 public:
  //! Initialize BVH
  Bvh(const SettingNodeBase* settings) noexcept;

  //! Finalize BVH
  virtual ~Bvh() noexcept;


  //! Return the tree of BVH
  const std::vector<BvhTreeNode>& bvhTree() const noexcept;

  //! Cast the ray and find the intersection closest to the ray origin
  IntersectionInfo castRay(const Ray& ray, const Float max_distance) const noexcept;

  //! Build BVH
  void construct(System& system, std::vector<Object>&& object_list) noexcept;

  //! Make BVH
  static std::unique_ptr<Bvh> makeBvh(const SettingNodeBase* settings) noexcept;

  //! Return the object list
  const std::vector<Object>& objectList() const noexcept;

 protected:
  //! Build BVH
  virtual void constructBvh(System& system,
                            const std::vector<Object>& object_list,
                            std::vector<BvhBuildingNode>& tree) const noexcept = 0;

  //! Check if multi-threading is enabled
  static constexpr bool threadingIsEnabled() noexcept;

  //! Set the bounding box of the node
  template <bool threading = false>
  static void setupBoundingBoxes(System& system,
                                 std::vector<BvhBuildingNode>& tree,
                                 const uint32 index) noexcept;

  //! Set the bounding box of the node
  static void setupBoundingBox(std::vector<BvhBuildingNode>& tree,
                               const uint32 index) noexcept;

 private:
  //! Set the tree node and the object list
  void setTreeInfo(const std::vector<BvhBuildingNode>& tree,
                   std::vector<Object>& object_list,
                   const uint32 failure_next_index,
                   const uint32 index) noexcept;

  //! Set the tree with a object
  void setTreeInfo(std::vector<Object>& object_list) noexcept;

  //! Sort nodes by the search order
  void sortTreeNode(std::vector<BvhBuildingNode>& tree) const noexcept;

  //! Sort nodes by the search order
  void sortTreeNode(const std::vector<BvhBuildingNode>& tree,
                    std::vector<BvhBuildingNode>& new_tree,
                    const uint32 old_index,
                    uint32& index) const noexcept;

  //! Test ray-objects of a leaf node intersection
  void testRayObjectsIntersection(const Ray& ray,
                                  const BvhTreeNode& leaf_node,
                                  IntersectionInfo* intersection) const noexcept;


  std::vector<BvhTreeNode> tree_;
  std::vector<Object> object_list_;
};

//! \} Core

} // namespace nanairo

#include "bvh-inl.hpp"

#endif // NANAIRO_BVH_HPP
