/*!
  \file photon_map-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_MAP_INL_HPP
#define NANAIRO_PHOTON_MAP_INL_HPP

#include "photon_map.hpp"
// Standard C++ library
#include <algorithm>
#include <iterator>
#include <thread>
#include <vector>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "knn_photon_list.hpp"
#include "photon_map_node.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
PhotonMap<kSampleSize>::PhotonMap(const System& system) noexcept
{
  initialize(system);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::clear() noexcept
{
  node_counter_ = 0;
  for (auto& node_list : thread_node_list_)
    node_list.clear();
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::construct(System& system) noexcept
{
  ZISC_ASSERT(0 < node_list_.size(), "The size of the tree is zero.");
  // Allocate memory
  if (tree_.size() < node_list_.size())
    tree_.resize(node_list_.size(), nullptr);
  // Construct KD-tree
  constexpr bool threading = threadingIsEnabled();
  auto begin = node_list_.begin();
  auto end = begin + node_counter_;
  splitAtMedian<threading>(system, 1, begin, end);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::reserve(const uint num_of_thread_caches) noexcept
{
  // Thread cache list
  for (auto& node_list : thread_node_list_)
    node_list.reserve(num_of_thread_caches);
  // Node list
  const uint num_of_caches = num_of_thread_caches * numOfThreads();
  node_list_.resize(num_of_caches, nullptr);
  tree_.resize(num_of_caches, nullptr);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void PhotonMap<kSampleSize>::search(const Point3& point,
                                    const Vector3& normal,
                                    const Float radius2,
                                    PhotonList* photon_list) const noexcept
{
  uint index = 1;
  while (index != 0) {
    const auto node = tree_[index - 1];
    testInsideCircle(point, normal, radius2, node, photon_list);
    // Internal node
    if (node->nodeType() != MapNode::NodeType::kLeaf) {
      const uint axis = zisc::cast<uint>(node->nodeType());
      const Float axis_diff = point[axis] - node->point()[axis];
      const Float axis_diff2 = zisc::power<2>(axis_diff);
      // Left child node
      const uint left_child_index = index << 1;
      const auto left_child_node = tree_[left_child_index - 1];
      if (left_child_node != nullptr &&
          (axis_diff < 0.0 || axis_diff2 < radius2)) {
        index = left_child_index;
        continue;
      }
      // Right child node
      const uint right_child_index = left_child_index + 1;
      const auto right_child_node = tree_[right_child_index - 1];
      if (right_child_node != nullptr &&
          (0.0 <= axis_diff || axis_diff2 < radius2)) {
        index = right_child_index;
        continue;
      }
    }
    index = nextSearchIndex(point, radius2, index);
  }
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::store(const int thread_id,
                                   const Point3& point,
                                   const Vector3& vin,
                                   const Spectra& photon_energy,
                                   const bool wavelength_is_selected) noexcept
{
  // Store a photon cache
  auto& node_list = thread_node_list_[thread_id];
  node_list.emplace_back(photon_energy, point, vin, wavelength_is_selected);
  // Add a cache to the node list
  auto& node = node_list.back();
  const auto index = node_counter_++;
  // Allocate the memory of the node list
  if (node_list_.size() <= index) {
    std::unique_lock<std::mutex> locker{lock_};
    if (node_list_.size() <= index)
      node_list_.resize(2 * node_list_.size(), nullptr);
  }
  node_list_[index] = &node;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
uint PhotonMap<kSampleSize>::getLongestAxis(NodeIterator begin,
                                            NodeIterator end) const noexcept
{
  auto min_point = (*begin)->point().data();
  auto max_point = min_point;
  for (auto iterator = ++begin; iterator != end; ++iterator) {
    min_point = zisc::minElements(min_point, (*iterator)->point().data());
    max_point = zisc::maxElements(max_point, (*iterator)->point().data());
  }
  const auto axis_diff = max_point - min_point;
  return (axis_diff[1] < axis_diff[0])
      ? (axis_diff[2] < axis_diff[0])
          ? zisc::cast<uint>(MapNode::NodeType::kXAxisSplit)
          : zisc::cast<uint>(MapNode::NodeType::kZAxisSplit)
      : (axis_diff[2] < axis_diff[1])
          ? zisc::cast<uint>(MapNode::NodeType::kYAxisSplit)
          : zisc::cast<uint>(MapNode::NodeType::kZAxisSplit);
}

/*!
  */
template <uint kSampleSize>
uint PhotonMap<kSampleSize>::nextSearchIndex(const Point3& point,
                                             const Float radius2,
                                             uint index) const noexcept
{
  // Back to parent node until the current node is left node
  while (index != 0) {
    while ((index & 1) == 1)
      index = index >> 1;
    if (index != 0) {
      ++index;
      const uint parent_index = index >> 1;
      const auto parent_node = tree_[parent_index - 1];
      const auto axis = zisc::cast<uint>(parent_node->nodeType());
      const Float axis_diff = point[axis] - parent_node->point()[axis];
      const Float axis_diff2 = zisc::power<2>(axis_diff);
      const auto node = tree_[index - 1];
      if (node != nullptr && (0.0 <= axis_diff || axis_diff2 < radius2))
        break;
    }
  }
  return index;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::initialize(const System& system) noexcept
{
  const auto num_of_threads = system.threadPool().numOfThreads();
  thread_node_list_.resize(num_of_threads);
}

/*!
  */
template <uint kSampleSize> inline
uint PhotonMap<kSampleSize>::numOfNodes() const noexcept
{
  uint num_of_nodes = 0;
  for (const auto& node_list : thread_node_list_)
    num_of_nodes += zisc::cast<uint>(node_list.size());
  return num_of_nodes;
}

/*!
  */
template <uint kSampleSize> inline
uint PhotonMap<kSampleSize>::numOfThreads() const noexcept
{
  return zisc::cast<uint>(thread_node_list_.size());
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> template <bool threading>
void PhotonMap<kSampleSize>::splitAtMedian(System& system,
                                           const uint number,
                                           NodeIterator begin,
                                           NodeIterator end) noexcept
{
  const uint size = zisc::cast<uint>(std::distance(begin, end));
  if (size == 0) {
    tree_[number - 1] = nullptr;
  }
  // Leaf node
  else if (size == 1) {
    (*begin)->setNodeType(MapNode::NodeType::kLeaf);
    tree_[number - 1] = *begin;
  }
  // Internal node
  else if (1 < size) {
    // Sort the nodes by the longest axis of the photon area
    const uint axis = getLongestAxis(begin, end);
    const auto compare = [axis](const MapNode* a, const MapNode* b)
    {
      return a->point().get(axis) < b->point().get(axis);
    };
    std::sort(begin, end, compare);
    // Set a median node
    auto median = begin;
    std::advance(median, size >> 1);
    (*median)->setNodeType(axis);
    tree_[number - 1] = *median;

    // Operate the child nodes
    const uint left_number = number << 1;
    auto split_left_group = [this, &system, left_number, begin, median]()
    {
      splitAtMedian<false>(system, left_number, begin, median);
    };
    auto right_begin = median + 1;
    const uint right_number = left_number + 1;
    auto split_right_group = [this, &system, right_number, right_begin, end]()
    {
      splitAtMedian<false>(system, right_number, right_begin, end);
    };
    // Threading
    if (threading) {
      auto& thread_pool = system.threadPool();
      auto left_result = thread_pool.enqueue<void>(split_left_group);
      auto right_result = thread_pool.enqueue<void>(split_right_group);
      left_result.get();
      right_result.get();
    }
    // Sequence
    else {
      split_left_group();
      split_right_group();
    }
  }
}

/*!
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::testInsideCircle(const Point3& point,
                                              const Vector3& normal,
                                              const Float radius2,
                                              const MapNode* node,
                                              PhotonList* photon_list) const noexcept
{
  const Float distance2 = (point - node->point()).squareNorm();
  if (distance2 < radius2) {
    const auto& cache = node->cache();
    const auto& vin = cache.incidentDirection();
    const Float cos_theta = -zisc::dot(normal, vin);
    if (0.0 < cos_theta)
      photon_list->insert(distance2, &cache);
  }
}

/*!
  */
template <uint kSampleSize> inline
constexpr bool PhotonMap<kSampleSize>::threadingIsEnabled() noexcept
{
  return true;
}

} // namespace nanairo

#endif // NANAIRO_PHOTON_MAP_INL_HPP
