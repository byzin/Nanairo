/*!
  \file bvh_tree_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_TREE_NODE_INL_HPP
#define NANAIRO_BVH_TREE_NODE_INL_HPP

#include "bvh_tree_node.hpp"
// Standard C++ library
#include <limits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "bvh_building_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
BvhTreeNode::BvhTreeNode() noexcept :
    object_info_{0},
    failure_next_index_{0}
{
}

/*!
  \details
  No detailed.
  */
inline
const Aabb& BvhTreeNode::boundingBox() const noexcept
{
  return bounding_box_;
}

/*!
  \details
  No detailed.
  */
inline
bool BvhTreeNode::isLeafNode() const noexcept
{
  const bool is_leaf_node = objectInfo() != 0;
  return is_leaf_node;
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhTreeNode::failureNextIndex() const noexcept
{
  return failure_next_index_;
}

/*!
  \details
  No detailed.
  */
inline
uint BvhTreeNode::numOfObjects() const noexcept
{
  const uint32 num_of_objects = objectInfo() >> numOfObjectsShift();
  return zisc::cast<uint>(num_of_objects);
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhTreeNode::objectIndex() const noexcept
{
  const uint32 object_index = objectInfo() & objectIndexMask();
  return object_index;
}

/*!
  \details
  No detailed.
  */
inline
void BvhTreeNode::setBoundingBox(const Aabb& bounding_box) noexcept
{
  bounding_box_ = bounding_box;
}

/*!
  \details
  No detailed.
  */
inline
void BvhTreeNode::setFailureNextIndex(const uint32 failure_next_index) noexcept
{
  failure_next_index_ = failure_next_index;
}

/*!
  */
inline
void BvhTreeNode::setObjectInfo(const uint32 object_index,
                                const uint num_of_objects) noexcept
{
  ZISC_ASSERT(object_index <= objectIndexMask(),
              "The object index is exceeded the limit.");
  const uint32 objects = zisc::cast<uint32>(num_of_objects) << numOfObjectsShift();
  object_info_ = objects | object_index;
}

/*!
  */
inline
constexpr uint32 BvhTreeNode::numOfObjectsMask() noexcept
{
  uint32 mask = objectIndexMask();
  mask = ~mask;
  return mask;
}

/*!
  */
inline
constexpr uint32 BvhTreeNode::numOfObjectsShift() noexcept
{
  uint32 shift = 0;
  uint32 mask = numOfObjectsMask();
  while ((mask & zisc::cast<uint32>(0b01)) != zisc::cast<uint32>(0b01)) {
    ++shift;
    mask = mask >> 1;
  }
  return shift;
}

/*!
  */
inline
constexpr uint32 BvhTreeNode::objectIndexMask() noexcept
{
  constexpr uint32 mask = BvhBuildingNode::maxNumOfLeafs();
  return mask;
}

/*!
  */
inline
uint32 BvhTreeNode::objectInfo() const noexcept
{
  return object_info_;
}

} // namespace nanairo

#endif // NANAIRO_BVH_TREE_NODE_INL_HPP
