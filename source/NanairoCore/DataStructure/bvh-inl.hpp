/*!
  \file bvh-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_INL_HPP
#define NANAIRO_BVH_INL_HPP

#include "bvh.hpp"
// Standard C++ library
#include <functional>
#include <vector>
#include <utility>
// Nanairo
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
const std::vector<BvhTreeNode>& Bvh::bvhTree() const noexcept
{
  return tree_;
}

/*!
  \details
  No detailed.
  */
inline
const std::vector<Object>& Bvh::objectList() const noexcept
{
  return object_list_;
}

/*!
  */
template <bool threading> inline
void Bvh::setBoundingBox(System& system,
                         std::vector<BvhNode>& tree,
                         const uint32 index) noexcept
{
  auto& node = tree[index];
  // Inernal node
  if (!node.isLeafNode()) {
    const auto left_child_index = node.leftChildIndex();
    const auto right_child_index = node.rightChildIndex();
    auto set_left_bounding_box = [&system, &tree, left_child_index]()
    {
      setBoundingBox<false>(system, tree, left_child_index);
    };
    auto set_right_bounding_box = [&system, &tree, right_child_index]()
    {
      setBoundingBox<false>(system, tree, right_child_index);
    };
    // Threding
    if (threading) {
      auto& thread_pool = system.threadPool();
      auto left_result = thread_pool.enqueue<void>(set_left_bounding_box);
      auto right_result = thread_pool.enqueue<void>(set_right_bounding_box);
      left_result.get();
      right_result.get();
    }
    // Sequence
    else {
      set_left_bounding_box();
      set_right_bounding_box();
    }
  }
  setBoundingBox(tree, index);
}

/*!
  */
inline
constexpr bool Bvh::threadingIsEnabled() noexcept
{
  return true;
}

/*!
  \details
  No detailed.
  */
inline
uint32 Bvh::endIndex() const noexcept
{
  return end_index_;
}

} // namespace nanairo

#endif // NANAIRO_BVH_INL_HPP
