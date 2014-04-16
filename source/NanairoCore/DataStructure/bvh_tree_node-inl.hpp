/*!
  \file bvh_tree_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_BVH_TREE_NODE_INL_HPP_
#define _NANAIRO_BVH_TREE_NODE_INL_HPP_

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
BvhTreeNode::BvhTreeNode()
{
}

/*!
  \details
  No detailed.
  */
inline
const Aabb& BvhTreeNode::boundingBox() const
{
  return bounding_box_;
}

/*!
  \details
  No detailed.
  */
inline
bool BvhTreeNode::isLeafNode() const
{
  return numOfObjects() != 0;
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhTreeNode::failureNextIndex() const
{
  return failure_next_index_;
}

/*!
  \details
  No detailed.
  */
inline
uint BvhTreeNode::numOfObjects() const
{
  return zisc::cast<uint>(num_of_objects_);
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhTreeNode::objectIndex() const
{
  return object_index_;
}

/*!
  \details
  No detailed.
  */
inline
void BvhTreeNode::setBoundingBox(const Aabb& bounding_box)
{
  bounding_box_ = bounding_box;
}

/*!
  \details
  No detailed.
  */
inline
void BvhTreeNode::setFailureNextIndex(const uint32 failure_next_index)
{
  failure_next_index_ = failure_next_index;
}

/*!
  \details
  No detailed.
  */
inline
void BvhTreeNode::setNumOfObjects(const uint num_of_objects)
{
  num_of_objects_ = zisc::cast<uint32>(num_of_objects);
}

/*!
  \details
  No detailed.
  */
inline
void BvhTreeNode::setObjectIndex(const uint32 object_index)
{
  object_index_ = object_index;
}

} // namespace nanairo

#endif // _NANAIRO_BVH_TREE_NODE_INL_HPP_
