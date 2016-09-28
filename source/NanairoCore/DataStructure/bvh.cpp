/*!
  \file bvh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "bvh.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <iterator>
//#include <iostream>
#include <limits>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
// Qt
#include <QJsonObject>
#include <QString>
// Zics
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "agglomerative_treelet_restructuring_bvh.hpp"
#include "approximate_agglomerative_clustering_bvh.hpp"
#include "binary_radix_tree_bvh.hpp"
#include "bvh_node.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  */
Bvh::~Bvh() noexcept
{
}

/*!
  \details
  No detailed.
  */
Bvh::Bvh(const QJsonObject& /* settings */) noexcept
{
}

/*!
  \details
  No detailed.
  */
IntersectionInfo Bvh::castRay(const Ray& ray, const Float max_distance2) const noexcept 
{
  IntersectionInfo intersection;
  Float shortest_distance2 = max_distance2;
  uint32 index = 0;
  while (true) {
    const auto& node = tree_[index];
    const auto result = node.boundingBox().testIntersection(ray);
    // If the ray hits the bounding box of the node, enter the node
    const bool ray_hits_aabb = std::get<0>(result);
    const Float distance = std::get<1>(result);
    if (ray_hits_aabb && (zisc::power<2>(distance) < shortest_distance2)) {
      // A case of leaf node
      if (node.isLeafNode())
        testRayObjectsIntersection(ray, node, &intersection, &shortest_distance2);
      ++index;
    }
    else {
      index = node.failureNextIndex();
    }
    if (index == endIndex())
      break;
  }
  return intersection;
}

/*!
  \details
  No detailed.
  */
void Bvh::construct(System& system, std::vector<Object>&& object_list) noexcept
{
  ZISC_ASSERT(object_list.size() <= BvhNode::maxNumOfObjects(),
              "The size of objects is over.");
  // Allocate memory
  object_list_.reserve(object_list.size());

  if (object_list.size() == 1) {
    end_index_ = 1;
    setUniqueObject(object_list);
  }
  else {
    std::vector<BvhNode> tree;
    constructBvh(system, object_list, tree);
//    printTree(0, 0, tree);
    sortTreeNode(tree);
    tree_.resize(tree.size());
    end_index_ = zisc::cast<uint32>(tree_.size());
    setTreeInfo(tree, object_list, endIndex());
  }
  ZISC_ASSERT(object_list_.size() == object_list.size(), "Object data is collapsed.");
}

/*!
  \details
  No detailed.
  */
std::size_t Bvh::getBvhSize() const noexcept
{
  return tree_.size() * sizeof(tree_[0]) +
         object_list_.size() * sizeof(object_list_[0]);
}

/*!
  \details
  No detailed.
  */
void Bvh::setTreeInfo(const std::vector<BvhNode>& tree,
                      std::vector<Object>& object_list,
                      const uint32 failure_next_index,
                      const uint32 index) noexcept
{
  using zisc::cast;

  // Set a node
  const auto& old_node = tree[index];
  auto& node = tree_[index];
  node.setBoundingBox(old_node.boundingBox());
  node.setObjectIndex(cast<uint32>(object_list_.size()));
  node.setNumOfObjects(old_node.numOfObjects());
  node.setFailureNextIndex(failure_next_index);
  // Set objects
  const auto& node_object_list = old_node.objectList();
  for (uint i = 0; i < node.numOfObjects(); ++i) {
    ZISC_ASSERT(node_object_list[i] != nullptr, "Object is null.");
    const uint object_index = cast<uint>(node_object_list[i] - object_list.data());
    ZISC_ASSERT(object_index < object_list.size(), "invalid index is specified.");
    object_list_.emplace_back(std::move(object_list[object_index]));
  }
  if (!node.isLeafNode()) {
    const uint32 left_child_index = old_node.leftChildIndex();
    const uint32 right_child_index = old_node.rightChildIndex();
    setTreeInfo(tree, object_list, right_child_index, left_child_index);
    setTreeInfo(tree, object_list, failure_next_index, right_child_index);
  }
}

/*!
  \details
  No detailed.
  */
void Bvh::sortTreeNode(std::vector<BvhNode>& tree) const noexcept
{
  std::vector<BvhNode> new_tree;
  new_tree.resize(tree.size());
  uint32 index = 0;
  sortTreeNode(tree, new_tree, index, index);
  ZISC_ASSERT(index < tree.size(), "Invalid sort operation.");

  tree.swap(new_tree);
  auto tail = tree.begin() + index + 1;
  tree.erase(tail, tree.end());
  ZISC_ASSERT(0 < tree.size(), "Tree data is collapsed.");
  ZISC_ASSERT(tree.size() == (index + 1), "Tree data is collapsed.");
}

/*!
  \details
  No detailed.
  */
void Bvh::sortTreeNode(const std::vector<BvhNode>& old_tree,
                       std::vector<BvhNode>& tree,
                       const uint32 old_index,
                       uint32& index) const noexcept
{
  const uint32 i = index;
  const auto& old_node = old_tree[old_index];
  auto& node = tree[i];
  node = old_node;
  if (!node.isLeafNode()) {
    // Left child
    ++index;
    const uint32 left_child_index = index;
    node.setLeftChildIndex(left_child_index);
    sortTreeNode(old_tree, tree, old_node.leftChildIndex(), index);
    // Right child
    ++index;
    const uint32 right_child_index = index;
    node.setRightChildIndex(right_child_index);
    sortTreeNode(old_tree, tree, old_node.rightChildIndex(), index);
  }
}

/*!
  \details
  No detailed.
  */
void Bvh::setUniqueObject(std::vector<Object>& object_list) noexcept
{
  // Object list
  object_list_.emplace_back(std::move(object_list[0]));

  // Node
  tree_.resize(1);
  auto& node = tree_[0];
  node.setBoundingBox(object_list_[0].geometry().boundingBox());
  node.setObjectIndex(0);
  node.setNumOfObjects(1);
  node.setFailureNextIndex(endIndex());
}

/*!
  \details
  No detailed.
  */
UniquePointer<Bvh> makeBvh(const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  Bvh* bvh = nullptr;

  const auto type = SceneValue::toString(settings, keyword::type);
  switch (keyword::toHash32(type)) {
    case toHash32(keyword::binaryRadixTreeBvh): {
      bvh = new BinaryRadixTreeBvh{settings};
      break;
    }
    case zisc::toHash32(keyword::approximateAgglomerativeClusteringBvh): {
      bvh = new ApproximateAgglomerativeClusteringBvh{settings};
      break;
    }
    case toHash32(keyword::agglomerativeTreeletRestructuringBvh): {
      bvh = new AgglomerativeTreeletRestructuringBvh{settings};
      break;
    }
    default: {
      zisc::raiseError("BvhError: Unsupported type is specified.");
      break;
    }
  }
  return UniquePointer<Bvh>{bvh};
}

} // namespace nanairo
