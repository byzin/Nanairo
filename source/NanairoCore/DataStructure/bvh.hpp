/*!
  \file bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "zisc/memory_resource.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/unique_memory_pointer.hpp"
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
  kBinaryRadixTree            = zisc::Fnv1aHash32::hash("BinaryRadixTree"),
  kAgglomerativeTreeletRestructuring = zisc::Fnv1aHash32::hash("AgglomerativeTreeletRestructuring")
};

/*!
  \details
  No detailed.
  */
class Bvh : public zisc::NonCopyable<Bvh>
{
 public:
  //! Initialize BVH
  Bvh(System& system, const SettingNodeBase* settings) noexcept;

  //! Finalize BVH
  virtual ~Bvh() noexcept;


  //! Return the tree of BVH
  const zisc::pmr::vector<BvhTreeNode>& bvhTree() const noexcept;

  //! Cast the ray and find the intersection closest to the ray origin
  IntersectionInfo castRay(const Ray& ray,
                           const Float max_distance,
                           const bool expect_no_hit = false) const noexcept;

  //! Build BVH
  void construct(System& system,
                 const SettingNodeBase* settings,
                 zisc::pmr::vector<Object>&& object_list) noexcept;

  //! Make BVH
  static zisc::UniqueMemoryPointer<Bvh> makeBvh(
      System& system,
      const SettingNodeBase* settings) noexcept;

  //! Return the object list
  const zisc::pmr::vector<Object>& objectList() const noexcept;

 protected:
  //! Build BVH
  virtual void constructBvh(
      System& system,
      const zisc::pmr::vector<Object>& object_list,
      zisc::pmr::vector<BvhBuildingNode>& tree) const noexcept = 0;

  //! Check if multi-threading is enabled
  static constexpr bool threadingIsEnabled() noexcept;

  //! Set the bounding box of the node
  template <bool threading = false>
  static void setupBoundingBoxes(System& system,
                                 zisc::pmr::vector<BvhBuildingNode>& tree,
                                 const uint32 index) noexcept;

  //! Set the bounding box of the node
  static void setupBoundingBox(zisc::pmr::vector<BvhBuildingNode>& tree,
                               const uint32 index) noexcept;

 private:
  //! Set the tree node and the object list
  void setTreeInfo(const zisc::pmr::vector<BvhBuildingNode>& tree,
                   zisc::pmr::vector<Object>& object_list,
                   const uint32 failure_next_index,
                   const uint32 index) noexcept;

  //! Set the tree with a object
  void setTreeInfo(zisc::pmr::vector<Object>& object_list) noexcept;

  //! Sort nodes by the search order
  void sortTreeNode(zisc::pmr::vector<BvhBuildingNode>& tree) const noexcept;

  //! Sort nodes by the search order
  void sortTreeNode(const zisc::pmr::vector<BvhBuildingNode>& tree,
                    zisc::pmr::vector<BvhBuildingNode>& new_tree,
                    const uint32 old_index,
                    uint32& index) const noexcept;

  //! Test ray-objects of a leaf node intersection
  void testRayObjectsIntersection(const Ray& ray,
                                  const BvhTreeNode& leaf_node,
                                  IntersectionInfo* intersection) const noexcept;


  zisc::pmr::vector<BvhTreeNode> tree_;
  zisc::pmr::vector<Object> object_list_;
};

//! \} Core

} // namespace nanairo

#include "bvh-inl.hpp"

#endif // NANAIRO_BVH_HPP
