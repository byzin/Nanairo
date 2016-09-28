/*!
  \file agglomerative_treelet_restructuring_bvh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "agglomerative_treelet_restructuring_bvh.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "binary_radix_tree_bvh.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Geometry/geometry.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
AgglomerativeTreeletRestructuringBvh::AgglomerativeTreeletRestructuringBvh(
    const QJsonObject& settings) noexcept :
        Bvh(settings)
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
inline
void AgglomerativeTreeletRestructuringBvh::buildRelationship(
    const uint32 parent_index,
    const uint32 left_child_index,
    const uint32 right_child_index,
    std::vector<BvhNode>& tree) const noexcept
{
  auto& parent = tree[parent_index];
  auto& left_child = tree[left_child_index];
  auto& right_child = tree[right_child_index];

  parent.setLeftChildIndex(left_child_index);
  parent.setRightChildIndex(right_child_index);
  parent.setBoundingBox(combine(left_child.boundingBox(), right_child.boundingBox()));

  left_child.setParentIndex(parent_index);
  right_child.setParentIndex(parent_index);
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::constructBvh(
    System& system,
    const std::vector<Object>& object_list,
    std::vector<BvhNode>& tree) const noexcept
{
  // Allocate memory
  tree.resize((object_list.size() * 2) - 1);

  BinaryRadixTreeBvh::constructBinaryRadixTreeBvh(system, object_list, tree);
  
  std::vector<uint> inner_index_list;
  std::vector<uint> leaf_index_list;
  std::vector<Float> distance_matrix;
  inner_index_list.reserve(treeletSize() - 1);
  leaf_index_list.reserve(treeletSize());
  distance_matrix.resize((treeletSize() * (treeletSize() - 1)) / 2);
  
  constexpr bool multithreading = multithreadingIsEnabled();
  for (uint i = 0; i < optimizationLoopCount(); ++i) {
    restructureTreelet<multithreading>(system, 0, inner_index_list, leaf_index_list, 
                                       distance_matrix, tree);
  }
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::constructOptimalTree(
    std::vector<uint32>& inner_index_list,
    std::vector<uint32>& leaf_index_list,
    std::vector<Float>& distance_matrix,
    std::vector<BvhNode>& tree) const noexcept
{
  using zisc::cast;

  initializeDistanceMatrix(leaf_index_list, tree, distance_matrix);
  while (true) {
    // Find best match
    const auto position = findBestMatch(cast<uint>(leaf_index_list.size()),
                                        distance_matrix);
    const auto row = std::get<0>(position);
    const auto column = std::get<1>(position);
    ZISC_ASSERT(column < row, "Column must be smaller than row.");

    // Build a parent-child relationship
    const auto parent_index = inner_index_list.back();
    const auto left_child_index = leaf_index_list[column];
    const auto right_child_index = leaf_index_list[row];
    buildRelationship(parent_index, left_child_index, right_child_index, tree);

    // Update info
    leaf_index_list[column] = parent_index;
    leaf_index_list[row] = leaf_index_list.back();
    inner_index_list.pop_back();
    leaf_index_list.pop_back();
    if (leaf_index_list.size() == 2)
      break;
    updateDistanceMatrix(leaf_index_list, tree, row, column, distance_matrix);
  }
  ZISC_ASSERT(inner_index_list.size() == 1, 
              "The construction algorithm is not yet completed.");
  buildRelationship(inner_index_list[0], leaf_index_list[0], leaf_index_list[1], tree);
}

/*!
  \details
  No detailed.
  */
std::tuple<uint, uint> AgglomerativeTreeletRestructuringBvh::findBestMatch(
    const uint n,
    const std::vector<Float>& distance_matrix) const noexcept
{
  ZISC_ASSERT(2 < n, "Lack of leaf nodes.");
  uint row = 1,
       column = 0;
  uint index = 1;
  Float min_distance = distance_matrix[0];
  for (uint r = 2; r < n; ++r) {
    for (uint c = 0; c < r; ++c) {
      const Float distance = distance_matrix[index];
      if (distance < min_distance) {
        row = r;
        column = c;
        min_distance = distance;
      }
      ++index;
    }
  }
  ZISC_ASSERT(column < row, "Column must be smaller than row.");
  return std::make_tuple(row, column);
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::formTreelet(
    const uint treelet_size,
    const uint32 root_index,
    const std::vector<BvhNode>& tree,
    std::vector<uint>& inner_index_list,
    std::vector<uint>& leaf_index_list) const noexcept
{
  ZISC_ASSERT(3 <= treelet_size, "Treelet size is small: ", treelet_size);
  
  inner_index_list.clear();
  leaf_index_list.clear();

  inner_index_list.push_back(root_index);
  // Set child nodes of the root as leaf nodes
  leaf_index_list.push_back(tree[root_index].leftChildIndex());
  leaf_index_list.push_back(tree[root_index].rightChildIndex());

  while (leaf_index_list.size() < treelet_size) {
    uint32 inner_i = 0;
    Float max_surface_area = std::numeric_limits<Float>::min();
    // Choose the treelet leaf with the largest surface area
    for (uint i = 0; i < leaf_index_list.size(); ++i) {
      const auto& node = tree[leaf_index_list[i]];
      if (node.isLeafNode())
        continue;
      const auto surface_area = node.boundingBox().surfaceArea();
      if (max_surface_area < surface_area) {
        inner_i = i;
        max_surface_area = surface_area;
      }
    }
    // Turn the selected treelet leaf into the treelet inner
    const auto inner_index = leaf_index_list[inner_i];
    const auto& inner_node = tree[inner_index];
    ZISC_ASSERT(!inner_node.isLeafNode(), "The selected node is leaf node.");
    inner_index_list.push_back(inner_index);
    leaf_index_list[inner_i] = leaf_index_list.back();
    leaf_index_list.pop_back();
    leaf_index_list.push_back(inner_node.leftChildIndex());
    leaf_index_list.push_back(inner_node.rightChildIndex());
  }
  ZISC_ASSERT(inner_index_list.size() == (treelet_size - 1), 
              "The number of inner nodes is invalid");
  ZISC_ASSERT(leaf_index_list.size() == treelet_size, 
              "The number of leaf nodes is invalid");
}

/*!
  \details
  No detailed.
  */
inline
Float AgglomerativeTreeletRestructuringBvh::getNodeDistance(
    const BvhNode& node1,
    const BvhNode& node2) const noexcept
{
  return combine(node1.boundingBox(), node2.boundingBox()).surfaceArea();
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::initialize(
    const QJsonObject& settings) noexcept
{
  treelet_size_ = SceneValue::toInt<uint>(settings, keyword::treeletSize);
  optimization_loop_count_ =
      SceneValue::toInt<uint>(settings, keyword::optimizationLoopCount);
  ZISC_ASSERT(4 <= treelet_size_, "Invalid treelet size is specified.");
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::initializeDistanceMatrix(
    const std::vector<uint>& leaf_index_list,
    const std::vector<BvhNode>& tree,
    std::vector<Float>& distance_matrix) const noexcept
{
  using zisc::cast;

  const uint n = cast<uint>(leaf_index_list.size());
  ZISC_ASSERT(2 < n, "Lack of leaf nodes: ", n);
  uint index = 0;
  for (uint row = 1; row < n; ++row) {
    const auto& node2 = tree[leaf_index_list[row]];
    for (uint column = 0; column < row; ++column) {
      ZISC_ASSERT(index < ((n * (n - 1)) / 2), "The matrix is buffer overrun.");
      const auto& node1 = tree[leaf_index_list[column]];
      distance_matrix[index] = getNodeDistance(node1, node2);
      ++index;
    }
  }
  ZISC_ASSERT(index == ((n * (n - 1)) / 2), "Invalid loop count.");
}

/*!
  \details
  No detailed.
  */
inline
uint AgglomerativeTreeletRestructuringBvh::optimizationLoopCount() const noexcept
{
  return optimization_loop_count_;
}

/*!
  \details
  No detailed.
  */
template <bool multithreading>
uint AgglomerativeTreeletRestructuringBvh::restructureTreelet(
    System& system,
    const uint32 index,
    std::vector<uint>& inner_index_list,
    std::vector<uint>& leaf_index_list,
    std::vector<Float>& distance_matrix,
    std::vector<BvhNode>& tree) const noexcept
{
  ZISC_ASSERT(index < tree.size(), "BVH tree is buffer overrun!!.");
  auto& root = tree[index];
  uint num_of_subtree_nodes = 1;
  if (root.isLeafNode())
    return num_of_subtree_nodes;

  if (multithreading) {
    auto restructure_left_treelet =
    [this, &system, &root, &inner_index_list, &leaf_index_list, 
     &distance_matrix, &tree]()
    {
      auto inner_list = inner_index_list;
      auto leaf_list = leaf_index_list;
      auto matrix = distance_matrix;
      return restructureTreelet<false>(system, root.leftChildIndex(), inner_list, 
                                       leaf_list, matrix, tree);
    };
    auto restructure_right_treelet =
    [this, &system, &root, &inner_index_list, &leaf_index_list, 
     &distance_matrix, &tree]()
    {
      auto& inner_list = inner_index_list;
      auto& leaf_list = leaf_index_list;
      auto& matrix = distance_matrix;
      return restructureTreelet<false>(system, root.rightChildIndex(), inner_list, 
                                       leaf_list, matrix, tree);
    };
    auto& thread_pool = system.threadPool();
    auto left_result = thread_pool.enqueue<uint>(restructure_left_treelet);
    auto right_result = thread_pool.enqueue<uint>(restructure_right_treelet);
    num_of_subtree_nodes += left_result.get();
    num_of_subtree_nodes += right_result.get();
  }
  else {
    num_of_subtree_nodes += restructureTreelet<false>(system,
                                                      root.leftChildIndex(), 
                                                      inner_index_list,
                                                      leaf_index_list,
                                                      distance_matrix,
                                                      tree);
    num_of_subtree_nodes += restructureTreelet<false>(system,
                                                      root.rightChildIndex(), 
                                                      inner_index_list,
                                                      leaf_index_list,
                                                      distance_matrix,
                                                      tree);
  }

  ZISC_ASSERT(3 <= num_of_subtree_nodes, "Lack of nodes.");
  // Check the number of nodes in the subtree
  if (num_of_subtree_nodes == 3) {
    buildRelationship(index, root.leftChildIndex(), root.rightChildIndex(), tree);
  }
  else {
    const auto num_of_leafs = (num_of_subtree_nodes >> 1) + 1;
    const auto treelet_size = (num_of_leafs < treeletSize()) 
        ? num_of_leafs 
        : treeletSize();
    formTreelet(treelet_size, index, tree, inner_index_list, leaf_index_list);
    constructOptimalTree(inner_index_list, leaf_index_list, distance_matrix, tree);
  }
  return num_of_subtree_nodes;
}

/*!
  \details
  No detailed.
  */
inline
uint AgglomerativeTreeletRestructuringBvh::treeletSize() const noexcept
{
  return treelet_size_;
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::updateDistanceMatrix(
    const std::vector<uint>& leaf_index_list,
    const std::vector<BvhNode>& tree,
    const uint row,
    const uint column,
    std::vector<Float>& distance_matrix) const noexcept
{
  using zisc::cast;

  ZISC_ASSERT(column < row, "Column must be smaller than row.");
  const uint n = cast<uint>(leaf_index_list.size());
  const uint k = (n * (n - 1)) / 2;
  // Re-calculation fase
  const auto& new_node = tree[leaf_index_list[column]];
  uint index = (column * (column - 1)) / 2;
  for (uint i = 0; i < column; ++i) {
    ZISC_ASSERT(index < k, "The distance matrix is buffer overrun.");
    const auto& node = tree[leaf_index_list[i]];
    distance_matrix[index] = getNodeDistance(node, new_node);
    ++index;
  }
  index = (((column + 1) * (column + 2)) / 2) - 1;
  for (uint i = column + 1; i < n; ++i) {
    ZISC_ASSERT(index < k, "The distance matrix is buffer overrun.");
    const auto& node = tree[leaf_index_list[i]];
    distance_matrix[index] = getNodeDistance(node, new_node);
    index += i;
  }

  // Copy fase
  if (row == n)
    return;
  index = (row * (row - 1)) / 2;
  uint source_index = k;
  for (uint i = 0; i < row; ++i) {
    ZISC_ASSERT(index < k, "The distance matrix is buffer overrun.");
    ZISC_ASSERT(source_index < (k + n + 1), "The distance matrix is buffer overrun.");
    if (i != column)
      distance_matrix[index] = distance_matrix[source_index];
    ++index;
    ++source_index;
  }
  index = (((row + 1) * (row + 2)) / 2) - 1;
  ++source_index;
  for (uint i = row + 1; i < n; ++i) {
    ZISC_ASSERT(index < k, "The distance matrix is buffer overrun.");
    ZISC_ASSERT(source_index < (k + n + 1), "The distance matrix is buffer overrun.");
    distance_matrix[index] = distance_matrix[source_index];
    index += i;
    ++source_index;
  }
}

} // namespace nanairo
