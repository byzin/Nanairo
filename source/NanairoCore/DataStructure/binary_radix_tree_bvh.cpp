/*!
  \file binary_radix_tree_bvh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "aabb.hpp"
#include "bvh.hpp"
#include "bvh_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
BinaryRadixTreeBvh::BinaryRadixTreeBvh(const QJsonObject& settings) noexcept :
    Bvh(settings)
{
}

/*!
  \details
  No detailed.
  */
void BinaryRadixTreeBvh::constructBinaryRadixTreeBvh(
    System& system,
    const std::vector<Object>& object_list,
    std::vector<BvhNode>& tree) noexcept
{
  // Create leaf node list
  std::vector<BvhNode> leaf_node_list;
  leaf_node_list.reserve(object_list.size());
  for (const auto& object : object_list)
    leaf_node_list.emplace_back(&object);

  auto morton_code_list = makeMortonCodeList(leaf_node_list);

  constexpr bool threading = threadingIsEnabled();
  auto first = morton_code_list.begin();
  auto begin = first;
  auto end = morton_code_list.end();
  splitInMortonCode<threading>(system, 63, 0, tree, first, begin, end);
  setBoundingBox<threading>(system, tree, 0);
}

/*!
  \details
  No detailed.
  */
void BinaryRadixTreeBvh::constructBvh(System& system,
                                      const std::vector<Object>& object_list,
                                      std::vector<BvhNode>& tree) const noexcept
{
  const auto num_of_nodes = object_list.size() * 2 - 1;
  tree.resize(num_of_nodes);
  constructBinaryRadixTreeBvh(system, object_list, tree);
}

/*!
  \details
  No detailed.
  */
template <bool threading>
void BinaryRadixTreeBvh::splitInMortonCode(System& system,
                                           const uint64 bit,
                                           const uint32 index,
                                           std::vector<BvhNode>& tree,
                                           MortonCodeIterator first,
                                           MortonCodeIterator begin,
                                           MortonCodeIterator end) noexcept
{
  using zisc::cast;

  // Check if node is leaf node
  if ((tree.size() >> 1) <= index) {
    tree[index] = std::move(*std::get<0>(*begin));
    return;
  }

  // Split leaf node list in the morton code
  auto split_position = findSplitPosition(bit, begin, end);
  if (split_position == begin || split_position == end) {
    if (bit != 1) {
      const auto next_bit = bit - 1;
      splitInMortonCode<threading>(system, next_bit, index, tree, first, begin, end);
      return;
    }
    else {
      const auto length = std::distance(begin, end);
      split_position = begin + (length >> 1);
    }
  }

  const uint64 next_bit = (bit != 1) ? bit - 1 : bit;
  uint32 right_child_index = cast<uint32>(std::distance(first, split_position));
  uint32 left_child_index = cast<uint32>(right_child_index - 1);
  left_child_index = (std::distance(begin, split_position) == 1)
      ? left_child_index + cast<uint32>(tree.size() >> 1)
      : left_child_index;
  right_child_index = (std::distance(split_position, end) == 1)
      ? right_child_index + cast<uint32>(tree.size() >> 1)
      : right_child_index;
  ZISC_ASSERT(left_child_index < tree.size(), "BVH buffer is overrun!.");
  ZISC_ASSERT(right_child_index < tree.size(), "BVH buffer is overrun!.");

  auto split_in_left_morton_code =
  [&system, next_bit, left_child_index, &tree, first, begin, split_position]()
  {
    splitInMortonCode<false>(
        system, next_bit, left_child_index, tree, first, begin, split_position);
  };
  auto split_in_right_morton_code =
  [&system, next_bit, right_child_index, &tree, first, split_position, end]()
  {
    splitInMortonCode<false>(
        system, next_bit, right_child_index, tree, first, split_position, end);
  };

  if (threading) {
    auto& thread_pool = system.threadPool();
    auto left_result = thread_pool.enqueue<void>(split_in_left_morton_code);
    auto right_result = thread_pool.enqueue<void>(split_in_right_morton_code);
    left_result.get();
    right_result.get();
  }
  else {
    split_in_left_morton_code();
    split_in_right_morton_code();
  }
  ZISC_ASSERT(tree[left_child_index].parentIndex() == BvhNode::nonObjectIndex(),
              "BVH node rewrite.");
  ZISC_ASSERT(tree[right_child_index].parentIndex() == BvhNode::nonObjectIndex(),
              "BVH node rewrite.");

  // Left child node
  tree[index].setLeftChildIndex(left_child_index);
  tree[left_child_index].setParentIndex(index);

  // Right child node
  tree[index].setRightChildIndex(right_child_index);
  tree[right_child_index].setParentIndex(index);
}

} // namespace nanairo
