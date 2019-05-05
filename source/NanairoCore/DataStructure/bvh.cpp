/*!
  \file bvh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "bvh.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <limits>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
// Zics
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "agglomerative_treelet_restructuring_bvh.hpp"
#include "binary_radix_tree_bvh.hpp"
#include "bvh_building_node.hpp"
#include "bvh_tree_node.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Setting/bvh_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Bvh::Bvh(System& system, const SettingNodeBase* /* settings */) noexcept :
    tree_{&system.dataMemoryManager()},
    object_list_{&system.dataMemoryManager()}
{
}

/*!
  */
Bvh::~Bvh() noexcept
{
}

/*!
  \details
  No detailed.
  */
IntersectionInfo Bvh::castRay(const Ray& ray,
                              const Float max_distance,
                              const bool expect_no_hit) const noexcept 
{
  ZISC_ASSERT(0.0 < max_distance, "The max_distance is minus.");
  IntersectionInfo intersection;
  intersection.setRayDistance(max_distance);
  uint32 index = 0;
  const auto& bvh_tree = bvhTree();
  const uint32 end_index = zisc::cast<uint32>(bvh_tree.size());
  const auto inv_dir = invert(ray.direction());
  while ((index != end_index) && !(intersection.isIntersected() && expect_no_hit)) {
    const auto& node = bvh_tree[index];
    const auto result = node.boundingBox().testIntersection(ray, inv_dir);
    // If the ray hits the bounding box of the node, enter the node
    if (result.isSuccess() && (result.rayDistance() < intersection.rayDistance())) {
      // A case of leaf node
      if (node.isLeafNode())
        testRayObjectsIntersection(ray, node, &intersection);
      ++index;
    }
    else {
      index = node.failureNextIndex();
    }
  }
  return intersection;
}

/*!
  \details
  No detailed.
  */
void Bvh::construct(System& system,
                    const SettingNodeBase* settings,
                    zisc::pmr::vector<Object>&& object_list) noexcept
{
  ZISC_ASSERT(object_list.size() <= BvhBuildingNode::maxNumOfLeafs(),
              "The size of objects is over.");
  // Allocate memory
  object_list_.reserve(object_list.size());
  if (object_list.size() == 1) {
    tree_.resize(1);
    setTreeInfo(object_list);
  }
  else {
    auto work_resource = settings->workResource();
    zisc::pmr::vector<BvhBuildingNode> tree{work_resource};
    constructBvh(system, object_list, tree);
    sortTreeNode(tree);
    tree_.resize(tree.size());
    setTreeInfo(tree, object_list, zisc::cast<uint32>(tree.size()), 0);
  }
  ZISC_ASSERT(object_list_.size() == object_list.size(),
              "The object list is collapsed.");
}

/*!
  \details
  No detailed.
  */
zisc::UniqueMemoryPointer<Bvh> Bvh::makeBvh(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto bvh_setting_node = castNode<BvhSettingNode>(settings);

  zisc::UniqueMemoryPointer<Bvh> bvh;
  switch (bvh_setting_node->bvhType()) {
   case BvhType::kBinaryRadixTree: {
    bvh = zisc::UniqueMemoryPointer<BinaryRadixTreeBvh>::make(
        &system.dataMemoryManager(),
        system,
        settings);
    break;
   }
   case BvhType::kAgglomerativeTreeletRestructuring: {
    bvh = zisc::UniqueMemoryPointer<AgglomerativeTreeletRestructuringBvh>::make(
        &system.dataMemoryManager(),
        system,
        settings);
    break;
   }
   default: {
    zisc::raiseError("BvhError: Unsupported type is specified.");
    break;
   }
  }
  return bvh;
}

/*!
  */
void Bvh::setupBoundingBox(zisc::pmr::vector<BvhBuildingNode>& tree,
                           const uint32 index) noexcept
{
  auto& node = tree[index];
  // Leaf node
  if (node.isLeafNode()) {
    ZISC_ASSERT(0 < node.numOfObjects(), "Node has no object.");
    const auto& object_list = node.objectList();
    auto bounding_box = object_list[0]->shape().boundingBox();
    for (uint i = 1; i < node.numOfObjects(); ++i) {
      const auto object = object_list[i];
      ZISC_ASSERT(object != nullptr, "The object is null.");
      bounding_box = combine(bounding_box, object->shape().boundingBox());
    }
    node.setBoundingBox(bounding_box);
  }
  // Inernal node
  else {
    const auto& left_node = tree[node.leftChildIndex()];
    const auto& right_node = tree[node.rightChildIndex()];
    const auto combined_box = combine(left_node.boundingBox(),
                                      right_node.boundingBox());
    node.setBoundingBox(combined_box);
  }
}

/*!
  \details
  No detailed.
  */
void Bvh::setTreeInfo(const zisc::pmr::vector<BvhBuildingNode>& tree,
                      zisc::pmr::vector<Object>& object_list,
                      const uint32 failure_next_index,
                      const uint32 index) noexcept
{
  const auto& node = tree[index];
  // Set the node
  {
    auto& new_node = tree_[index];
    const uint32 object_index = zisc::cast<uint32>(object_list_.size());
    new_node.setBoundingBox(node.boundingBox());
    new_node.setObjectInfo(object_index, node.numOfObjects());
    new_node.setFailureNextIndex(failure_next_index);
  }
  // Set the objects
  const auto& node_object_list = node.objectList();
  for (uint i = 0; i < node.numOfObjects(); ++i) {
    ZISC_ASSERT(node_object_list[i] != nullptr, "The object is null.");
    const uint object_index = zisc::cast<uint>(node_object_list[i] -
                                               object_list.data());
    ZISC_ASSERT(object_index < object_list.size(), "invalid index is specified.");
    object_list_.emplace_back(std::move(object_list[object_index]));
  }
  if (!node.isLeafNode()) {
    // Child nodes
    const uint32 left_child_index = node.leftChildIndex();
    const uint32 right_child_index = node.rightChildIndex();
    setTreeInfo(tree, object_list, right_child_index, left_child_index);
    setTreeInfo(tree, object_list, failure_next_index, right_child_index);
  }
}

/*!
  \details
  No detailed.
  */
void Bvh::setTreeInfo(zisc::pmr::vector<Object>& object_list) noexcept
{
  // Set the object
  object_list_.emplace_back(std::move(object_list[0]));
  // Set the node
  auto& node = tree_[0];
  node.setBoundingBox(object_list_[0].shape().boundingBox());
  node.setObjectInfo(0, 1);
  node.setFailureNextIndex(zisc::cast<uint32>(object_list.size()));
}

/*!
  \details
  No detailed.
  */
void Bvh::sortTreeNode(zisc::pmr::vector<BvhBuildingNode>& tree) const noexcept
{
  uint32 index = 0;
  // Sort the tree data
  {
    zisc::pmr::vector<BvhBuildingNode> new_tree{tree.get_allocator().resource()};
    new_tree.resize(tree.size());
    sortTreeNode(tree, new_tree, index, index);
    ZISC_ASSERT(index < tree.size(), "Invalid sort operation.");
    tree.swap(new_tree);
    ++index;
  }
  auto tail = tree.begin() + index;
  tree.erase(tail, tree.end());
  ZISC_ASSERT(tree.size() == index, "The tree data is collapsed.");
}

/*!
  \details
  No detailed.
  */
void Bvh::sortTreeNode(const zisc::pmr::vector<BvhBuildingNode>& tree,
                       zisc::pmr::vector<BvhBuildingNode>& new_tree,
                       const uint32 index,
                       uint32& new_index) const noexcept
{
  const auto& node = tree[index];
  auto& new_node = new_tree[new_index];
  new_node = node;
  if (!new_node.isLeafNode()) {
    // Left child
    ++new_index;
    const uint32 left_child_index = new_index;
    new_node.setLeftChildIndex(left_child_index);
    sortTreeNode(tree, new_tree, node.leftChildIndex(), new_index);
    // Right child
    ++new_index;
    const uint32 right_child_index = new_index;
    new_node.setRightChildIndex(right_child_index);
    sortTreeNode(tree, new_tree, node.rightChildIndex(), new_index);
  }
}

/*!
  \details
  No detailed.
  */
inline
void Bvh::testRayObjectsIntersection(const Ray& ray,
                                     const BvhTreeNode& leaf_node,
                                     IntersectionInfo* intersection) const noexcept
{
  ZISC_ASSERT(intersection != nullptr, "The intersection is null.");
  const auto& object_list = objectList();
  for (uint i = 0; i < leaf_node.numOfObjects(); ++i) {
    const auto object_index = leaf_node.objectIndex() + i;
    const auto& object = object_list[object_index];
    const auto result = object.shape().testIntersection(ray, intersection);
    if (result)
      intersection->setObject(&object);
  }
}

} // namespace nanairo
