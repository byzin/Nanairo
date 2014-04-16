/*!
  \file photon_map-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_PHOTON_MAP_INL_HPP_
#define _NANAIRO_PHOTON_MAP_INL_HPP_

#include "photon_map.hpp"
// Standard C++ library
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/thread_pool.hpp"
// Nanairo
#include "knn_photon_list.hpp"
#include "photon_map_node.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
PhotonMap<kSampleSize>::PhotonMap(System& system)
{
  initialize(system);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::clear()
{
  for (auto& photon_cache_list : thread_photon_cache_list_)
    photon_cache_list.clear();
  for (auto& node_list : thread_node_list_)
    node_list.clear();
  node_reference_list_.clear();
  std::fill(tree_.begin(), tree_.end(), nullptr);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void PhotonMap<kSampleSize>::construct(System& system)
{
  // Calculate the number of nodes
  uint num_of_node = 0;
  for (const auto& node_list : thread_node_list_)
    num_of_node += node_list.size();

  // Reserve memory for cache
  if (tree_.size() < num_of_node) {
    node_reference_list_.reserve(num_of_node);
    const uint level = getTreeLevel(num_of_node);
    const uint num_of_tree_nodes = (1 << level) - 1;
    const uint num_of_results = (num_of_tree_nodes >> kNonThreadLevel) - 1;
    tree_.resize(num_of_tree_nodes, nullptr);
    result_list_.resize(num_of_results);
  }

  // Initialize node cache list
  for (auto& node_list : thread_node_list_) {
    for (auto& node : node_list)
      node_reference_list_.emplace_back(&node);
  }

  // Construct KD-tree
  splitMedian(system, 1, node_reference_list_.begin(), node_reference_list_.end());

  for (auto& result : result_list_) {
    if (result.valid())
      result.get();
  }
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::reserve(System& system, const uint size)
{
  auto& thread_pool = system.threadPool();
  const uint thread_photons = size / thread_pool.numOfThreads();

  for (auto& photon_cache_list : thread_photon_cache_list_)
    photon_cache_list.reserve(thread_photons);
  for (auto& node_list : thread_node_list_)
    node_list.reserve(thread_photons);
  node_reference_list_.reserve(size);

  const uint level = getTreeLevel(size);
  const uint num_of_tree_nodes = (1 << level) - 1;
  const uint num_of_results = (num_of_tree_nodes >> kNonThreadLevel) - 1;
  tree_.resize(num_of_tree_nodes);
  result_list_.resize(num_of_results);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void PhotonMap<kSampleSize>::search(const Point3& point, 
                                    const Vector3& normal,
                                    const Float radius2,
                                    PhotonList& photon_list) const
{
  const auto test_inside_circle = 
  [this, radius2, &point, &normal](const MapNode* node, PhotonList& photon_list)
  {
    const Float distance2 = (point - node->point()).squareNorm();
    if (distance2 < radius2) {
      const auto photon_cache = node->photonCache();
      const auto& vin = photon_cache->incidentDirection();
      const Float cos_theta = -zisc::dot(normal, vin);
      if (cos_theta > 0.0)
        photon_list.insert(distance2, photon_cache);
    }
  };

  // Return the next node index
  const auto back_to_next = [this, radius2, &point](uint index)
  {
    while (true) {
      // Back to parent node until the current node is left node
      while ((index & 1) == 1)
        index = index >> 1;
      if (index == 0)
        return index;
      ++index;
      const uint parent_index = index >> 1; 
      const auto parent_node = tree_[parent_index - 1];
      const auto flag = parent_node->flag();
      const Float axis_length = point[flag] - (parent_node->point())[flag];
      const Float axis_length2 = axis_length * axis_length;
      const auto node = tree_[index - 1];
      if (node != nullptr && 
          (axis_length >= 0.0 || axis_length2 < radius2))
        return index;
    }
    zisc::raiseError("PhotonMapError: Photon search failed!!");
    return index;
  };

  uint index = 1;
  while (true) {
    const auto node = tree_[index - 1];
    test_inside_circle(node, photon_list);
    if (node->flag() == MapNode::kLeafNodeFlag) {
      index = back_to_next(index);
      if (index == 0)
        return;
      continue;
    }

    const auto flag = node->flag();
    const Float axis_length = point[flag] - (node->point())[flag];
    const Float axis_length2 = axis_length * axis_length;
    // Left child node
    const uint left_child_node_index = index << 1;
    const auto left_child_node = tree_[left_child_node_index - 1];
    if (left_child_node != nullptr && 
        (axis_length < 0.0 || axis_length2 < radius2)) {
      index = left_child_node_index;
      continue;
    }
    // Right child node
    const uint right_child_node_index = (index << 1) + 1;
    const auto right_child_node = tree_[right_child_node_index - 1];
    if (right_child_node != nullptr && 
        (axis_length >= 0.0 || axis_length2 < radius2)){
      index = right_child_node_index;
      continue;
    }
    index = back_to_next(index);
    if (index == 0)
      return;
  }
  zisc::raiseError("PhotonMapError: Photon search failed.");
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
uint PhotonMap<kSampleSize>::getTreeLevel(uint num_of_nodes) const
{
  uint level = 0;
  while (num_of_nodes != 0) {
    num_of_nodes = num_of_nodes >> 1;
    ++level;
  }
  return level;
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
                                   const bool wavelength_is_selected)
{
  auto& photon_cache_list = thread_photon_cache_list_[thread_id];
  auto& node_list = thread_node_list_[thread_id];

  photon_cache_list.emplace_back(photon_energy, point, vin, wavelength_is_selected);
  const auto& photon_cache = photon_cache_list.back();
  node_list.emplace_back(&photon_cache);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
uint PhotonMap<kSampleSize>::getLongestAxis(NodeIterator begin, 
                                                   NodeIterator end) const
{
  auto iterator = begin;
  auto min_point = (*iterator)->point().data();
  auto max_point = min_point;
  ++iterator;
  while (iterator != end) {
    min_point = zisc::minElements(min_point, (*iterator)->point().data());
    max_point = zisc::maxElements(max_point, (*iterator)->point().data());
    ++iterator;
  }

  const auto axis_length = max_point - min_point;
  return (axis_length[0] > axis_length[1])
      ? (axis_length[0] > axis_length[2]) ? MapNode::kXAxisFlag : MapNode::kZAxisFlag
      : (axis_length[1] > axis_length[2]) ? MapNode::kYAxisFlag : MapNode::kZAxisFlag;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMap<kSampleSize>::initialize(System& system)
{
  auto& thread_pool = system.threadPool();
  thread_photon_cache_list_.resize(thread_pool.numOfThreads());
  thread_node_list_.resize(thread_pool.numOfThreads());
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
void PhotonMap<kSampleSize>::splitMedian(System& system,
                                         const uint number,
                                         NodeIterator begin,
                                         NodeIterator end)
{
  const uint axis = getLongestAxis(begin, end);
  const auto compare = [axis](const MapNode* a, const MapNode* b)
  {
    return a->point().get(axis) < b->point().get(axis);
  };
  std::sort(begin, end, compare);

  // Get a median node
  auto median = begin;
  const uint size = std::distance(begin, end);
  std::advance(median, size >> 1);
  (*median)->setFlag(axis);
  tree_[number - 1] = *median;

  auto& thread_pool = system.threadPool();
  constexpr uint non_thread_line = 1 << kNonThreadLevel;

  // Operate the left group
  const uint left_size = std::distance(begin, median);
  const uint left_number = number << 1;
  if (left_size == 1) {
    (*begin)->setFlag(MapNode::kLeafNodeFlag);
    tree_[left_number - 1] = *begin;
  }
  else if (1 < left_size && left_size <= non_thread_line) {
    splitMedian(system, left_number, begin, median);
  }
  else if (non_thread_line < left_size) {
    std::function<void ()> split_group{[this, left_number, begin, median, &system]()
    {
      splitMedian(system, left_number, begin, median);
    }};
    result_list_[left_number - 2] = thread_pool.enqueue(std::move(split_group));
  }

  // Operate the right group
  auto right_begin = median + 1;
  const uint right_size = std::distance(right_begin, end);
  const uint right_number = left_number + 1;
  if (right_size == 1) {
    (*right_begin)->setFlag(MapNode::kLeafNodeFlag);
    tree_[right_number - 1] = *right_begin;
  }
  else if (1 < right_size && right_size <= non_thread_line) {
    splitMedian(system, right_number, right_begin, end);
  }
  else if (non_thread_line < right_size) {
    std::function<void ()> split_group{[this, right_number, right_begin, end, &system]()
    {
      splitMedian(system, right_number, right_begin, end);
    }};
    result_list_[right_number - 2] = thread_pool.enqueue(std::move(split_group));
  }
}

} // namespace nanairo

#endif // _NANAIRO_PHOTON_MAP_INL_HPP_
