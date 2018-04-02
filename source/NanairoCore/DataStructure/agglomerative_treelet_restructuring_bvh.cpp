/*!
  \file agglomerative_treelet_restructuring_bvh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
// Zisc
#include "zisc/error.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "binary_radix_tree_bvh.hpp"
#include "bvh_building_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Setting/bvh_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Shape/shape.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
AgglomerativeTreeletRestructuringBvh::AgglomerativeTreeletRestructuringBvh(
    const SettingNodeBase* settings) noexcept :
        Bvh(settings)
{
  initialize(settings);
}

/*!
  */
AgglomerativeTreeletRestructuringBvh::RestructuringData::RestructuringData(
    const uint treelet_size) noexcept
{
  inner_index_list_.resize(treelet_size - 1);
  leaf_index_list_.resize(treelet_size);
  distance_matrix_.resize((treelet_size * (treelet_size - 1)) / 2);
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
    std::vector<BvhBuildingNode>& tree) const noexcept
{
  auto& parent = tree[parent_index];
  auto& left_child = tree[left_child_index];
  auto& right_child = tree[right_child_index];

  parent.setLeftChildIndex(left_child_index);
  parent.setRightChildIndex(right_child_index);
  parent.setBoundingBox(combine(left_child.boundingBox(),
                                right_child.boundingBox()));

  left_child.setParentIndex(parent_index);
  right_child.setParentIndex(parent_index);
}

/*!
  \details
  No detailed.
  */
inline
Float AgglomerativeTreeletRestructuringBvh::calcNodeDistance(
    const BvhBuildingNode& lhs,
    const BvhBuildingNode& rhs) const noexcept
{
  return combine(lhs.boundingBox(), rhs.boundingBox()).surfaceArea();
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::constructBvh(
    System& system,
    const std::vector<Object>& object_list,
    std::vector<BvhBuildingNode>& tree) const noexcept
{
  // Make a simple BVH tree using fast construction algorithm
  BinaryRadixTreeBvh::constructBinaryRadixTreeBvh(system, object_list, tree);

  RestructuringData data{treeletSize()};;
  for (uint i = 0; i < optimizationLoopCount(); ++i) {
    constexpr bool threading = threadingIsEnabled();
    restructureTreelet<threading>(system, 0, data, tree);
  }
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::constructOptimalTreelet(
    RestructuringData& data,
    std::vector<BvhBuildingNode>& tree) const noexcept
{
  initializeDistanceMatrix(tree, data);

  auto& inner_index_list = data.inner_index_list_;
  auto& leaf_index_list = data.leaf_index_list_;
  auto& distance_matrix = data.distance_matrix_;
  while (true) {
    // Find best match
    const uint num_of_leafs = zisc::cast<uint>(leaf_index_list.size());
    const auto position = findBestMatch(num_of_leafs, distance_matrix);
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
    updateDistanceMatrix(tree, row, column, data);
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
    const uint num_of_leafs,
    const std::vector<Float>& distance_matrix) const noexcept
{
  ZISC_ASSERT(2 < num_of_leafs, "Lack of leaf nodes.");
  uint row = 1,
       column = 0;
  uint index = 1;
  Float min_distance = distance_matrix[0];
  for (uint r = 2; r < num_of_leafs; ++r) {
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
  ZISC_ASSERT(column < row, "The column must be smaller than the row.");
  return std::make_tuple(row, column);
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::formTreelet(
    const uint treelet_size,
    const uint32 root_index,
    const std::vector<BvhBuildingNode>& tree,
    RestructuringData& data) const noexcept
{
  ZISC_ASSERT(3 <= treelet_size, "Treelet size is small: ", treelet_size);

  auto& inner_index_list = data.inner_index_list_;
  auto& leaf_index_list = data.leaf_index_list_;

  inner_index_list.clear();
  leaf_index_list.clear();

  inner_index_list.push_back(root_index);
  // Set child nodes of the root as leaf nodes
  leaf_index_list.push_back(tree[root_index].leftChildIndex());
  leaf_index_list.push_back(tree[root_index].rightChildIndex());

  while (leaf_index_list.size() < treelet_size) {
    uint32 inner_i = 0;
    Float max_surface_area = 0.0;
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
    leaf_index_list.back() = inner_node.leftChildIndex();
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
void AgglomerativeTreeletRestructuringBvh::initialize(
    const SettingNodeBase* settings) noexcept
{
  const auto bvh_settings = castNode<BvhSettingNode>(settings);

  const auto& parameters =
      bvh_settings->agglomerativeTreeletRestructuringParameters();
  {
    treelet_size_ = parameters.treelet_size_;
    ZISC_ASSERT(4 <= treelet_size_, "Invalid treelet size is specified.");
  }
  {
    optimization_loop_count_ = parameters.optimization_loop_;
  }
}

/*!
  \details
  No detailed.
  */
void AgglomerativeTreeletRestructuringBvh::initializeDistanceMatrix(
    const std::vector<BvhBuildingNode>& tree,
    RestructuringData& data) const noexcept
{
  const auto& leaf_index_list = data.leaf_index_list_;

  const uint n = zisc::cast<uint>(leaf_index_list.size());
  ZISC_ASSERT(2 < n, "Lack of leaf nodes: ", n);
  uint index = 0;
  for (uint row = 1; row < n; ++row) {
    const auto& rhs = tree[leaf_index_list[row]];
    for (uint column = 0; column < row; ++column) {
      ZISC_ASSERT(index < ((n * (n - 1)) / 2), "The matrix is buffer overrun.");
      const auto& lhs = tree[leaf_index_list[column]];
      data.distance_matrix_[index] = calcNodeDistance(lhs, rhs);
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
template <bool threading>
uint AgglomerativeTreeletRestructuringBvh::restructureTreelet(
    System& system,
    const uint32 index,
    RestructuringData& data,
    std::vector<BvhBuildingNode>& tree) const noexcept
{
  ZISC_ASSERT(index < tree.size(), "BVH tree is buffer overrun!!.");
  auto& root = tree[index];
  uint num_of_subtree_nodes = 1;
  if (!root.isLeafNode()) {
    if (threading) {
      auto restructure_left_treelet = [this, &system, &root, &data, &tree]()
      {
        return restructureTreelet<>(system, root.leftChildIndex(), data, tree);
      };
      auto restructure_right_treelet = [this, &system, &root, &tree]()
      {
        RestructuringData d{treeletSize()};
        return restructureTreelet<>(system, root.rightChildIndex(), d, tree);
      };
      auto& threads = system.threadManager();
      auto left_result = threads.enqueue<uint>(restructure_left_treelet);
      auto right_result = threads.enqueue<uint>(restructure_right_treelet);
      num_of_subtree_nodes += left_result.get();
      num_of_subtree_nodes += right_result.get();
    }
    else {
      num_of_subtree_nodes +=
          restructureTreelet<>(system, root.leftChildIndex(), data, tree);
      num_of_subtree_nodes +=
          restructureTreelet<>(system, root.rightChildIndex(), data, tree);
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
      formTreelet(treelet_size, index, tree, data);
      constructOptimalTreelet(data, tree);
    }
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
  */
void AgglomerativeTreeletRestructuringBvh::updateDistanceMatrix(
    const std::vector<BvhBuildingNode>& tree,
    const uint row,
    const uint column,
    RestructuringData& data) const noexcept
{
  auto& leaf_index_list = data.leaf_index_list_;
  auto& distance_matrix = data.distance_matrix_;

  ZISC_ASSERT(column < row, "The column must be smaller than the row.");
  const uint num_of_leafs = zisc::cast<uint>(leaf_index_list.size());
  const uint matrix_size = (num_of_leafs * (num_of_leafs - 1)) / 2;
  // Re-calculate the distance
  uint index = (column * (column - 1)) / 2;
  for (uint i = 0; i < column; ++i) {
    ZISC_ASSERT(index < matrix_size, "The index is out of matrix range: ", index);
    const auto& lhs = tree[leaf_index_list[i]];
    const auto& rhs = tree[leaf_index_list[column]];
    distance_matrix[index] = calcNodeDistance(lhs, rhs);
    ++index;
  }
  index = ((column + 2) * (column + 1)) / 2 - 1;
  for (uint i = column + 1; i < num_of_leafs; ++i) {
    ZISC_ASSERT(index < matrix_size, "The index is out of matrix range: ", index);
    const auto& lhs = tree[leaf_index_list[column]];
    const auto& rhs = tree[leaf_index_list[i]];
    distance_matrix[index] = calcNodeDistance(lhs, rhs);
    index += i;
  }
  // Copy
  if (row == num_of_leafs)
    return;
  index = (row * (row - 1)) / 2;
  uint source_index = matrix_size;
  for (uint i = 0; i < row; ++i) {
    ZISC_ASSERT(index < matrix_size, "The index is out of matrix range: ", index);
    if (i != column)
      distance_matrix[index] = distance_matrix[source_index];
    ++index;
    ++source_index;
  }
  index = ((row + 2) * (row + 1)) / 2 - 1;
  ++source_index;
  for (uint i = row + 1; i < num_of_leafs; ++i) {
    ZISC_ASSERT(index < matrix_size, "The index is out of matrix range: ", index);
    distance_matrix[index] = distance_matrix[source_index];
    index += i;
    ++source_index;
  }
}

} // namespace nanairo
