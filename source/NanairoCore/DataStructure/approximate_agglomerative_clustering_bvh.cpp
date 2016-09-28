/*!
  \file approximate_agglomerative_clustering_bvh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "approximate_agglomerative_clustering_bvh.hpp"
// Standard C++ library
#include <array>
#include <cmath>
#include <functional>
#include <future>
#include <iterator>
#include <limits>
#include <tuple>
#include <vector>
#include <utility>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "bvh.hpp"
#include "bvh_node.hpp"
#include "agglomerative_cluster.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ApproximateAgglomerativeClusteringBvh::ApproximateAgglomerativeClusteringBvh(
    const QJsonObject& settings) noexcept :
        Bvh(settings)
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
template <bool multithreading>
auto ApproximateAgglomerativeClusteringBvh::buildTree(
    System& system,
    const uint64 bit,
    MortonCodeIterator begin,
    MortonCodeIterator end,
    std::vector<Float>& distance_matrix) const noexcept
    -> std::vector<ClusterPointer> 
{
  using zisc::cast;

  std::vector<ClusterPointer> cluster_list;
  const uint num_of_primitives = cast<uint>(std::distance(begin, end));
  if (num_of_primitives < delta()) {
    cluster_list.reserve(num_of_primitives);
    for (auto iterator = begin; iterator != end; ++iterator)
      cluster_list.emplace_back(new AgglomerativeCluster{std::get<0>(*iterator)});
    combineClusters(fDelta(), cluster_list, distance_matrix);
    return cluster_list;
  }

  auto split_position = findSplitPosition(bit, begin, end);
  if ((split_position == begin) || (split_position == end)) {
    if (bit != 1) {
      return buildTree<multithreading>(system, bit - 1, begin, end, distance_matrix);
    }
    else {
      const uint length = num_of_primitives;
      split_position = begin + (length >> 1);
    }
  }

  const uint64 next_bit = (bit != 1) ? bit - 1 : bit;
  std::vector<ClusterPointer> left_cluster_list,
                              right_cluster_list;
  if (multithreading) {
    auto build_left_tree =
    [this, &system, next_bit, begin, split_position, &distance_matrix]()
    {
      auto matrix = distance_matrix;
      return buildTree<false>(system, next_bit, begin, split_position, matrix);
    };
    auto build_right_tree =
    [this, &system, next_bit, split_position, end, &distance_matrix]()
    {
      auto& matrix = distance_matrix;
      return buildTree<false>(system, next_bit, split_position, end, matrix);
    };
    auto& thread_pool = system.threadPool();
    auto left_result = 
        thread_pool.enqueue<std::vector<ClusterPointer>>(build_left_tree);
    auto right_result = 
        thread_pool.enqueue<std::vector<ClusterPointer>>(build_right_tree);
    left_cluster_list = left_result.get();
    right_cluster_list = right_result.get();
  }
  else {
    left_cluster_list = buildTree<false>(system, next_bit, begin, 
                                         split_position, distance_matrix);
    right_cluster_list = buildTree<false>(system, next_bit, split_position, 
                                          end, distance_matrix);
  }
  cluster_list.reserve(left_cluster_list.size() + right_cluster_list.size());
  for (auto& cluster : left_cluster_list)
    cluster_list.emplace_back(std::move(cluster));
  for (auto& cluster : right_cluster_list)
    cluster_list.emplace_back(std::move(cluster));

  combineClusters(f(num_of_primitives), cluster_list, distance_matrix);
  return cluster_list;
}

/*!
  \details
  No detailed.
  */
void ApproximateAgglomerativeClusteringBvh::combineClusters(
    const uint n,
    std::vector<ClusterPointer>& cluster_list,
    std::vector<Float>& distance_matrix) const noexcept
{
  using zisc::cast;

  if (cluster_list.size() <= n)
    return;
  initializeDistanceMatrix(cluster_list, distance_matrix);
  while (true) {
    // Find the best match
    const auto position = findBestMatch(cast<uint>(cluster_list.size()), 
                                        distance_matrix);
    const auto row = std::get<0>(position);
    const auto column = std::get<1>(position);
    ZISC_ASSERT(column < row, "Column must be smaller than row.");

    // Build a parent-child relationship
    ClusterPointer left_child_cluster{std::move(cluster_list[column])};
    ClusterPointer right_child_cluster{std::move(cluster_list[row])};
    auto* parent_cluster = new AgglomerativeCluster{std::move(left_child_cluster),
                                                    std::move(right_child_cluster)};

    // Update info
    ZISC_ASSERT(!cluster_list[column], "Cluster[column] must be empty.");
    ZISC_ASSERT(!cluster_list[row], "Cluster[row] must be empty.");
    cluster_list[column].reset(parent_cluster);
    if (row != (cluster_list.size() - 1))
      cluster_list[row] = std::move(cluster_list.back());
    cluster_list.pop_back();
    if (cluster_list.size() == n)
      break;
    updateDistanceMatrix(cluster_list, row, column, distance_matrix);
  }
}

/*!
  \details
  No detailed.
  */
void ApproximateAgglomerativeClusteringBvh::constructBvh(
    System& system ,
    const std::vector<Object>& object_list,
    std::vector<BvhNode>& tree) const noexcept
{
  using zisc::cast;

  // Allocate memory
  const uint num_of_objects = cast<uint>(object_list.size());
  tree.resize((num_of_objects << 1) - 1);
  
  // Sort leaf nodes by the morton code
  std::vector<BvhNode> leaf_node_list;
  leaf_node_list.reserve(num_of_objects);
  for (const auto& object : object_list)
    leaf_node_list.emplace_back(&object);
  auto code_list = makeMortonCodeList(leaf_node_list);

  const auto n = delta() * 2;
  std::vector<Float> distance_matrix;
  distance_matrix.resize((n * (n - 1)) / 2 , 0.0);

  constexpr bool multithreading = multithreadingIsEnabled();

  // Build tree
  constexpr uint bit = 63;
  auto cluster_list = buildTree<multithreading>(system, bit, code_list.begin(), 
                                                code_list.end(), distance_matrix);
  combineClusters(1, cluster_list, distance_matrix);
  uint number = 0;
  setNode(*cluster_list[0], tree, number); 
  tree[0].setParentIndex(BvhNode::nonObjectIndex());
}

/*!
  \details
  No detailed.
  */
inline
uint ApproximateAgglomerativeClusteringBvh::delta() const noexcept
{
  return delta_;
}

/*!
  \details
  No detailed.
  */
inline
uint ApproximateAgglomerativeClusteringBvh::f(const uint n) const noexcept
{
  using zisc::cast;
  return cast<uint>(c_ * std::pow(cast<Float>(n), k_));
}

/*!
  \details
  No detailed.
  */
inline
uint ApproximateAgglomerativeClusteringBvh::fDelta() const noexcept
{
  return f_delta_;
}

/*!
  \details
  No detailed.
  */
std::tuple<uint, uint> ApproximateAgglomerativeClusteringBvh::findBestMatch(
    const uint n,
    const std::vector<Float>& distance_matrix) const noexcept
{
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
void ApproximateAgglomerativeClusteringBvh::initialize(
    const QJsonObject& settings) noexcept
{
  using zisc::cast;

  delta_ = SceneValue::toInt<uint>(settings, keyword::delta);
  const Float epsilon = SceneValue::toFloat<Float>(settings, keyword::epsilon);
  c_ = std::pow(cast<Float>(delta()), kAlpha + epsilon) * 0.5;
  k_ = kAlpha - epsilon;
  f_delta_ = f(delta());
}

/*!
  \details
  No detailed.
  */
void ApproximateAgglomerativeClusteringBvh::initializeDistanceMatrix(
    const std::vector<ClusterPointer>& cluster_list,
    std::vector<Float>& distance_matrix) const noexcept
{
  using zisc::cast;

  const uint n = cast<uint>(cluster_list.size());
  const auto k = (n * (n - 1)) / 2;
  if (distance_matrix.size() < k)
    distance_matrix.resize(k, 0.0);

  uint index = 0;
  for (uint row = 1; row < n; ++row) {
    const auto& cluster2 = cluster_list[row];
    for (uint column = 0; column < row; ++column) {
      ZISC_ASSERT(index < k, "The matrix is buffer overrun.");
      const auto& cluster1 = cluster_list[column];
      distance_matrix[index] = getClusterDistance(*cluster1, *cluster2);
      ++index;
    }
  }
  ZISC_ASSERT(index == k, "Invalid loop count.");
}

/*!
  \details
  No detailed.
  */
uint32 ApproximateAgglomerativeClusteringBvh::setNode(
    const AgglomerativeCluster& cluster, 
    std::vector<BvhNode>& tree,
    uint& number) const noexcept
{
  const uint32 index = zisc::cast<uint32>(number);
  ZISC_ASSERT(index < tree.size(), "BVH overflow!");
  ++number;

  auto& node = tree[index];
  if (cluster.isLeaf()) {
    auto object_list = cluster.objectList();
    for (const auto object : object_list)
      node.addObjectWithoutBoundingBox(object);
  }
  else {
    const auto left_child_index = setNode(cluster.leftChildCluster(), tree, number);
    const auto right_child_index = setNode(cluster.rightChildCluster(), tree, number);
    tree[left_child_index].setParentIndex(index);
    tree[right_child_index].setParentIndex(index);
    node.setLeftChildIndex(left_child_index);
    node.setRightChildIndex(right_child_index);
  }
  node.setBoundingBox(cluster.boundingBox());

  return index;
}

/*!
  \details
  No detailed.
  */
void ApproximateAgglomerativeClusteringBvh::updateDistanceMatrix(
    const std::vector<ClusterPointer>& cluster_list,
    const uint row,
    const uint column,
    std::vector<Float>& distance_matrix) const noexcept
{
  using zisc::cast;

  ZISC_ASSERT(column < row, "Column must be samller than row.");
  const uint n = cast<uint>(cluster_list.size());
  const uint k = (n * (n - 1)) / 2;
  // Re-calculation fase
  const auto& new_cluster = cluster_list[column];
  uint index = (column * (column - 1)) / 2;
  for (uint i = 0; i < column; ++i) {
    ZISC_ASSERT(index < k, "The distance matrix is buffer overrun.");
    const auto& cluster = cluster_list[i];
    distance_matrix[index] = getClusterDistance(*cluster, *new_cluster);
    ++index;
  }
  index = (((column + 1) * (column + 2)) / 2) - 1;
  for (uint i = column + 1; i < n; ++i) {
    ZISC_ASSERT(index < k, "The distance matrix is buffer overrun.");
    const auto& cluster = cluster_list[i];
    distance_matrix[index] = getClusterDistance(*cluster, *new_cluster);
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
