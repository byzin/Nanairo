/*!
  \file bvh_tree_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_TREE_NODE_INL_HPP
#define NANAIRO_BVH_TREE_NODE_INL_HPP

#include "bvh_tree_node.hpp"
// Standard C++ library
#include <limits>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
BvhTreeNode::BvhTreeNode() noexcept
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
  return numOfObjects() != 0;
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
  return zisc::cast<uint>(num_of_objects_);
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhTreeNode::objectIndex() const noexcept
{
  return object_index_;
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
  \details
  No detailed.
  */
inline
void BvhTreeNode::setNumOfObjects(const uint num_of_objects) noexcept
{
  num_of_objects_ = zisc::cast<uint32>(num_of_objects);
}

/*!
  \details
  No detailed.
  */
inline
void BvhTreeNode::setObjectIndex(const uint32 object_index) noexcept
{
  object_index_ = object_index;
}

} // namespace nanairo

#endif // NANAIRO_BVH_TREE_NODE_INL_HPP
