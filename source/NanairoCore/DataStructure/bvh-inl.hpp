/*!
  \file bvh-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
  \details
  No detailed.
  */
template <bool multithreading>
void Bvh::setBoundingBox(System& system, 
                         std::vector<BvhNode>& tree, 
                         const uint32 index) noexcept
{
  auto& node = tree[index];
  if (!node.isLeafNode()) {
    const auto left_child_index = node.leftChildIndex();
    const auto right_child_index = node.rightChildIndex();
    if (multithreading) {
      std::function<void ()> set_left_bounding_box{
      [&system, &tree, left_child_index]()
      {
        setBoundingBox<false>(system, tree, left_child_index);
      }};
      std::function<void ()> set_right_bounding_box{
      [&system, &tree, right_child_index]()
      {
        setBoundingBox<false>(system, tree, right_child_index);
      }};
      auto& thread_pool = system.threadPool();
      auto left_result = thread_pool.enqueue(std::move(set_left_bounding_box));
      auto right_result = thread_pool.enqueue(std::move(set_right_bounding_box));
      left_result.get();
      right_result.get();
    }
    else {
      setBoundingBox<false>(system, tree, left_child_index);
      setBoundingBox<false>(system, tree, right_child_index);
    }
    const auto& left_node = tree[left_child_index];
    const auto& right_node = tree[right_child_index];
    node.setBoundingBox(combine(left_node.boundingBox(), right_node.boundingBox()));
  }
  else {
    const auto& object_list = node.objectList();
    auto bounding_box = object_list[0]->geometry().boundingBox();
    for (uint i = 1; i < node.numOfObjects(); ++i) {
      const auto object = object_list[i];
      bounding_box = combine(bounding_box, object->geometry().boundingBox());
    }
    node.setBoundingBox(bounding_box);
  }
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

/*!
  \details
  No detailed.
  */
inline
void Bvh::testRayObjectsIntersection(const Ray& ray, 
                                     const BvhTreeNode& leaf_node,
                                     IntersectionInfo* intersection,
                                     Float* shortest_distance2) const noexcept
{
  IntersectionInfo current;
  const auto& object_list = objectList();
  for (uint i = 0; i < leaf_node.numOfObjects(); ++i) {
    const auto object_index = leaf_node.objectIndex() + i;
    const auto& object = object_list[object_index];
    const bool ray_hits_object = object.geometry().testIntersection(ray, &current);
    if (ray_hits_object) {
      const Float distance2 = (current.point() - ray.origin()).squareNorm();
      if (distance2 < *shortest_distance2) {
        current.setObject(&object);
        *intersection = current;
        *shortest_distance2 = distance2;
      }
    }
  }
}

} // namespace nanairo

#endif // NANAIRO_BVH_INL_HPP
