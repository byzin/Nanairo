/*!
  \file photon_map.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

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
PhotonMap::PhotonMap(const System& system) noexcept
{
  initialize(system);
}

/*!
  \details
  No detailed.
  */
void PhotonMap::clear() noexcept
{
  node_counter_ = 0;
  for (auto& node_list : thread_node_list_)
    node_list.clear();
}

/*!
  \details
  No detailed.
  */
void PhotonMap::construct(System& system) noexcept
{
  ZISC_ASSERT(0 < node_list_.size(), "The size of the tree is zero.");
  // Allocate memory
  const uint node_size = zisc::cast<uint>(node_counter_.load());
  uint memory = 1;
  while (memory < node_size)
    memory = memory << 1;
  tree_.resize(memory, nullptr);


//  if (tree_.size() < node_list_.size())
//    tree_.resize(node_list_.size(), nullptr);
  // Construct KD-tree
  constexpr bool threading = threadingIsEnabled();
  auto begin = node_list_.begin();
  auto end = begin + node_size;
  splitAtMedian<threading>(system, 1, begin, end);
}

/*!
  \details
  No detailed.
  */
void PhotonMap::reserve(const uint num_of_thread_caches) noexcept
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
void PhotonMap::search(const Point3& point,
                       const Vector3& normal,
                       const Float radius2,
                       KnnPhotonList* photon_list) const noexcept
{
  uint index = 1;
  while (index != 0) {
    const auto node = tree_[index - 1];
    testInsideCircle(point, normal, radius2, node, photon_list);
    // Internal node
    if (node->nodeType() != PhotonMapNode::NodeType::kLeaf) {
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
void PhotonMap::store(const int thread_id,
                      const Point3& point,
                      const Vector3& vin,
                      const SampledSpectra& photon_energy,
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
uint PhotonMap::getLongestAxis(NodeIterator begin,
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
          ? zisc::cast<uint>(PhotonMapNode::NodeType::kXAxisSplit)
          : zisc::cast<uint>(PhotonMapNode::NodeType::kZAxisSplit)
      : (axis_diff[2] < axis_diff[1])
          ? zisc::cast<uint>(PhotonMapNode::NodeType::kYAxisSplit)
          : zisc::cast<uint>(PhotonMapNode::NodeType::kZAxisSplit);
}

/*!
  */
uint PhotonMap::nextSearchIndex(const Point3& point,
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
void PhotonMap::initialize(const System& system) noexcept
{
  const auto num_of_threads = system.threadPool().numOfThreads();
  thread_node_list_.resize(num_of_threads);
}

/*!
  \details
  No detailed.
  */
template <bool threading>
void PhotonMap::splitAtMedian(System& system,
                              const uint number,
                              NodeIterator begin,
                              NodeIterator end) noexcept
{
  if (tree_.size() <= (number - 1))
    std::cout << "Size: " << tree_.size() << ", index: " << (number - 1) << std::endl;
  ZISC_ASSERT((number - 1) < tree_.size(), "The index is out of range.");
  const uint size = zisc::cast<uint>(std::distance(begin, end));
  if (size == 0) {
    tree_[number - 1] = nullptr;
  }
  // Leaf node
  else if (size == 1) {
    (*begin)->setNodeType(PhotonMapNode::NodeType::kLeaf);
    tree_[number - 1] = *begin;
  }
  // Internal node
  else if (1 < size) {
    // Sort the nodes by the longest axis of the photon area
    const uint axis = getLongestAxis(begin, end);
    const auto compare = [axis](const PhotonMapNode* a, const PhotonMapNode* b)
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
inline
void PhotonMap::testInsideCircle(const Point3& point,
                                 const Vector3& normal,
                                 const Float radius2,
                                 const PhotonMapNode* node,
                                 KnnPhotonList* photon_list) const noexcept
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

} // namespace nanairo
