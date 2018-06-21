/*!
  \file photon_map.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "photon_map.hpp"
// Standard C++ library
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <iterator>
#include <mutex>
#include <thread>
#include <vector>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
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
PhotonMap::PhotonMap() noexcept :
    num_of_nodes_{0}
{
}

/*!
  \details
  No detailed.
  */
void PhotonMap::construct(System& system) noexcept
{
  ZISC_ASSERT(0 < node_list_->size(), "The size of the tree is zero.");

  auto work_resource = &system.globalMemoryManager();

  // Allocate the tree memory
  const std::size_t node_size = num_of_nodes_.load(std::memory_order_relaxed);
  {
    std::size_t memory = 1;
    while (memory < node_size)
      memory = memory << 1;
    tree_ = decltype(tree_)::make(
        work_resource,
        decltype(tree_)::value_type{work_resource});
    tree_->resize(memory, nullptr);
  }

  // Construct KD-tree
  constexpr bool threading = threadingIsEnabled();
  auto begin = node_list_->begin();
  auto end = begin + node_size;
  splitAtMedian<threading>(system, 1, begin, end);
}

/*!
  */
void PhotonMap::initialize(System& system,
                           const std::size_t estimated_num_of_nodes) noexcept
{
  auto work_resource = &system.globalMemoryManager();

  std::size_t n = num_of_nodes_.exchange(0, std::memory_order_relaxed);
  n = (n != 0) ? (n << 1) : estimated_num_of_nodes;

  node_body_list_ = decltype(node_body_list_)::make(
      work_resource,
      decltype(node_body_list_)::value_type{work_resource});
  node_body_list_->resize(n);

  node_list_ = decltype(node_list_)::make(
      work_resource,
      decltype(node_list_)::value_type{work_resource});
  node_list_->resize(n, nullptr);
}

/*!
  */
void PhotonMap::reset() noexcept
{
  node_body_list_.reset();
  node_list_.reset();
  tree_.reset();
}

/*!
  \details
  No detailed.
  */
void PhotonMap::search(const Point3& point,
                       const Vector3& normal,
                       const Float radius2,
                       const bool is_frontside_culling,
                       const bool is_backside_culling,
                       KnnPhotonList* photon_list) const noexcept
{
  uint index = 1;
  while (index != 0) {
    const auto node = (*tree_)[index - 1];
    testInsideCircle(point, normal, radius2, node,
                     is_frontside_culling, is_backside_culling, photon_list);
    // Internal node
    if (node->nodeType() != PhotonMapNode::NodeType::kLeaf) {
      const uint axis = zisc::cast<uint>(node->nodeType());
      const Float axis_diff = point[axis] - node->point()[axis];
      const Float axis_diff2 = zisc::power<2>(axis_diff);
      // Left child node
      const uint left_child_index = index << 1;
      const auto left_child_node = (*tree_)[left_child_index - 1];
      if (left_child_node != nullptr &&
          (axis_diff < 0.0 || axis_diff2 < radius2)) {
        index = left_child_index;
        continue;
      }
      // Right child node
      const uint right_child_index = left_child_index + 1;
      const auto right_child_node = (*tree_)[right_child_index - 1];
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
void PhotonMap::store(const Point3& point,
                      const Vector3& vin,
                      const SampledSpectra& photon_energy,
                      const Float inverse_sampling_pdf,
                      const bool wavelength_is_selected) noexcept
{
  const std::size_t index = num_of_nodes_++;

  if (node_list_->size() <= index) {
    std::unique_lock<std::mutex> locker{lock_};
    if (node_list_->size() <= index) {
      node_body_list_->resize(2 * node_body_list_->size());
      node_list_->resize(2 * node_list_->size(), nullptr);
    }
  }

  auto& node = (*node_body_list_)[index];
  {
    auto& cache = node.cache();
    cache.setEnergy(photon_energy);
    cache.setPoint(point);
    cache.setIncidentDirection(vin);
    cache.setInversePdf(inverse_sampling_pdf);
    cache.setWavelengthIsSelected(wavelength_is_selected);
  }
  (*node_list_)[index] = &node;
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
      const auto parent_node = (*tree_)[parent_index - 1];
      const auto axis = zisc::cast<uint>(parent_node->nodeType());
      const Float axis_diff = point[axis] - parent_node->point()[axis];
      const Float axis_diff2 = zisc::power<2>(axis_diff);
      const auto node = (*tree_)[index - 1];
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
template <bool threading>
void PhotonMap::splitAtMedian(System& system,
                              const uint number,
                              NodeIterator begin,
                              NodeIterator end) noexcept
{
  ZISC_ASSERT((number - 1) < tree_->size(), "The index is out of range.");
  const uint size = zisc::cast<uint>(std::distance(begin, end));
  if (size == 0) {
    (*tree_)[number - 1] = nullptr;
  }
  // Leaf node
  else if (size == 1) {
    (*begin)->setNodeType(PhotonMapNode::NodeType::kLeaf);
    (*tree_)[number - 1] = *begin;
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
    (*tree_)[number - 1] = *median;

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
      auto& threads = system.threadManager();
      auto work_resource = &system.globalMemoryManager();
      auto left_result = threads.enqueue<void>(split_left_group, work_resource);
      auto right_result = threads.enqueue<void>(split_right_group, work_resource);
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
                                 const bool is_frontside_culling,
                                 const bool is_backside_culling,
                                 KnnPhotonList* photon_list) const noexcept
{
  const Float distance2 = (point - node->point()).squareNorm();
  if (distance2 < radius2) {
    const auto& cache = node->cache();
    const auto& vin = cache.incidentDirection();
    const Float cos_theta = -zisc::dot(normal, vin);
    if ((!is_frontside_culling && (0.0 < cos_theta)) ||
        (!is_backside_culling && (cos_theta < 0.0)))
      photon_list->insert(distance2, &cache);
  }
}

} // namespace nanairo
