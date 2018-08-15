/*!
  \file bvh-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_INL_HPP
#define NANAIRO_BVH_INL_HPP

#include "bvh.hpp"
// Standard C++ library
#include <utility>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/thread_manager.hpp"
// Nanairo
#include "bvh_building_node.hpp"
#include "bvh_tree_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
const zisc::pmr::vector<BvhTreeNode>& Bvh::bvhTree() const noexcept
{
  return tree_;
}

/*!
  \details
  No detailed.
  */
inline
const zisc::pmr::vector<Object>& Bvh::objectList() const noexcept
{
  return object_list_;
}

/*!
  */
inline
constexpr bool Bvh::threadingIsEnabled() noexcept
{
  return true;
}

/*!
  */
template <bool threading> inline
void Bvh::setupBoundingBoxes(System& system,
                             zisc::pmr::vector<BvhBuildingNode>& tree,
                             const uint32 index) noexcept
{
  auto& node = tree[index];
  // Inernal node
  if (!node.isLeafNode()) {
    const auto left_child_index = node.leftChildIndex();
    const auto right_child_index = node.rightChildIndex();
    // Threding
    if (threading) {
      auto set_left_bounding_box = [&system, &tree, left_child_index]()
      {
        Bvh::setupBoundingBoxes<>(system, tree, left_child_index);
      };
      auto set_right_bounding_box = [&system, &tree, right_child_index]()
      {
        Bvh::setupBoundingBoxes<>(system, tree, right_child_index);
      };
      auto& threads = system.threadManager();
      auto work_resource = tree.get_allocator().resource();
      auto left_result =
          threads.enqueue<void>(set_left_bounding_box, work_resource);
      auto right_result =
          threads.enqueue<void>(set_right_bounding_box, work_resource);
      left_result.wait();
      right_result.wait();
    }
    // Sequence
    else {
      setupBoundingBoxes<>(system, tree, left_child_index);
      setupBoundingBoxes<>(system, tree, right_child_index);
    }
  }
  setupBoundingBox(tree, index);
}

} // namespace nanairo

#endif // NANAIRO_BVH_INL_HPP
