/*!
  \file binary_radix_tree_bvh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "binary_radix_tree_bvh.hpp"
// Standard C++ library
#include <algorithm>
#include <functional>
#include <tuple>
#include <vector>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/thread_manager.hpp"
// Nanairo
#include "aabb.hpp"
#include "bvh.hpp"
#include "bvh_building_node.hpp"
#include "morton_code.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
BinaryRadixTreeBvh::BinaryRadixTreeBvh(System& system,
                                       const SettingNodeBase* settings) noexcept :
    Bvh(system, settings)
{
}

/*!
  \details
  No detailed.
  */
void BinaryRadixTreeBvh::constructBinaryRadixTreeBvh(
    System& system,
    const zisc::pmr::vector<Object>& object_list,
    zisc::pmr::vector<BvhBuildingNode>& tree) noexcept
{
  const auto num_of_nodes = 2 * object_list.size() - 1;
  tree.resize(num_of_nodes);

  auto work_resource = tree.get_allocator().resource();
  // Make a morton code list
  zisc::pmr::vector<BvhBuildingNode> leaf_node_list{work_resource};
  zisc::pmr::vector<MortonCode> morton_code_list{work_resource};
  {
    leaf_node_list.reserve(object_list.size());
    for (const auto& object : object_list)
      leaf_node_list.emplace_back(&object);
    morton_code_list = MortonCode::makeList(leaf_node_list);
  }

  constexpr bool threading = threadingIsEnabled();
  {
    auto first = morton_code_list.begin();
    auto begin = first;
    auto end = morton_code_list.end();
    constexpr uint key_bit = 8 * sizeof(MortonCode::CodeType) - 1;
    split<threading>(system, key_bit, 0, tree, first, begin, end);
  }
  setupBoundingBoxes<threading>(system, tree, 0);
}

/*!
  \details
  No detailed.
  */
void BinaryRadixTreeBvh::constructBvh(
    System& system,
    const zisc::pmr::vector<Object>& object_list,
    zisc::pmr::vector<BvhBuildingNode>& tree) const noexcept
{
  constructBinaryRadixTreeBvh(system, object_list, tree);
}

/*!
  \details
  No detailed.
  */
template <bool threading>
void BinaryRadixTreeBvh::split(System& system,
                               uint bit,
                               const uint32 index,
                               zisc::pmr::vector<BvhBuildingNode>& tree,
                               MortonCode::Iterator first,
                               MortonCode::Iterator begin,
                               MortonCode::Iterator end) noexcept
{
  using zisc::cast;

  const auto size = std::distance(begin, end);
  const uint32 internal_node_size = cast<uint32>(tree.size() >> 1);

  ZISC_ASSERT(index < tree.size(), "The index exceeds the tree size: ", index);
  ZISC_ASSERT(0 < size, "The size of the range isn't positive: ", size);

  if (1 < size) {
    // Internal node
    // Split leaf node list using the morton code
    auto split_position = end;
    for (;(0 < bit) && (split_position == begin || split_position == end); --bit)
      split_position = MortonCode::findSplit(bit, begin, end);
    if ((bit == 0) && (split_position == begin || split_position == end))
      split_position = begin + (size >> 1);

    uint32 right_child_index = cast<uint32>(std::distance(first, split_position));
    uint32 left_child_index = right_child_index - 1;
    left_child_index = (std::distance(begin, split_position) == 1)
        ? left_child_index + internal_node_size // Leaf node
        : left_child_index; // Internal node
    right_child_index = (std::distance(split_position, end) == 1)
        ? right_child_index + internal_node_size // Leaf node
        : right_child_index; // Internal node
    if (threading) {
      auto split_left_range =
      [&system, bit, left_child_index, &tree, first, begin, split_position]()
      {
        const auto key = (0 < bit) ? bit - 1 : bit;
        split<>(system, key, left_child_index, tree, first, begin, split_position);
      };
      auto split_right_range =
      [&system, bit, right_child_index, &tree, first, split_position, end]()
      {
        const auto key = (0 < bit) ? bit - 1 : bit;
        split<>(system, key, right_child_index, tree, first, split_position, end);
      };

      auto work_resource = tree.get_allocator().resource();
      auto& threads = system.threadManager();
      auto left_result = threads.enqueue<void>(split_left_range, work_resource);
      auto right_result = threads.enqueue<void>(split_right_range, work_resource);
      left_result.get();
      right_result.get();
    }
    else {
      const auto key = (0 < bit) ? bit - 1 : bit;
      split<>(system, key, left_child_index, tree, first, begin, split_position);
      split<>(system, key, right_child_index, tree, first, split_position, end);
    }

    ZISC_ASSERT(tree[index].parentIndex() == BvhBuildingNode::nullIndex(),
                "The node is rewrited.");
    tree[index].setLeftChildIndex(left_child_index);
    tree[index].setRightChildIndex(right_child_index);
    // Child nodes
    tree[left_child_index].setParentIndex(index);
    tree[right_child_index].setParentIndex(index);
  }
  else {
    // Leaf node
    // Check if node is leaf node
    ZISC_ASSERT(internal_node_size <= index, "The index is for internal: ", index);
    tree[index] = *(begin->node());
  }
}

} // namespace nanairo
