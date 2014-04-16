/*!
  \file bvh_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_BVH_NODE_INL_HPP_
#define _NANAIRO_BVH_NODE_INL_HPP_

#include "bvh_node.hpp"
// Standard C++ library
#include <array>
#include <vector>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Data/ray.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
void BvhNode::addObject(const Object* object)
{
  ZISC_ASSERT(object_list_.back() != nullptr, 
              "The number of objects exceed the limit.");

  const auto index = numOfObjects();
  object_list_[index] = object;
  const auto num_of_objects = index + 1;
  if (num_of_objects == 1)
    setBoundingBox(object->geometry().boundingBox());
  else
    setBoundingBox(combine(boundingBox(), object->geometry().boundingBox()));
}

/*!
  \details
  No detailed.
  */
inline
void BvhNode::addObjectWithoutBoundingBox(const Object* object)
{
  ZISC_ASSERT(object_list_.back() == nullptr, 
              "The number of objects exceed the limit.");

  const auto index = numOfObjects();
  object_list_[index] = object;
}

/*!
  \details
  No detailed.
  */
inline
const Aabb& BvhNode::boundingBox() const
{
  return bounding_box_;
}

/*!
  \details
  No detailed.
  */
inline
bool BvhNode::isLeafNode() const
{
  return object_list_[0] != nullptr;
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhNode::leftChildIndex() const 
{
  return left_child_index_;
}

/*!
  \details
  No detailed.
  */
inline
uint BvhNode::numOfObjects() const
{
  uint index = 0;
  for (auto object : object_list_) {
    if (object != nullptr)
      ++index;
  }
  return index;
}

/*!
 \details
 No detailed.
 */
inline
auto BvhNode::objectList() const 
    -> const std::array<const Object*, kMaxNumOfObjectsPerNode>& 
{
  return object_list_;
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhNode::parentIndex() const
{
  return parent_index_;
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhNode::rightChildIndex() const
{
  return right_child_index_;
}

/*!
  \details
  No detailed.
  */
inline
Float BvhNode::sahCost() const
{
  return zisc::cast<Float>(cost_);
}

/*!
  \details
  No detailed.
  */
inline
void BvhNode::setBoundingBox(const Aabb& bounding_box)
{
  bounding_box_ = bounding_box;
}

/*!
  \details
  No detailed.
  */
inline
void BvhNode::setLeftChildIndex(const uint32 left_child_index)
{
  left_child_index_ = left_child_index;
}

/*!
  \details
  No detailed.
  */
inline
void BvhNode::setParentIndex(const uint32 parent_index)
{
  parent_index_ = parent_index;
}

/*!
  \details
  No detailed.
  */
inline
void BvhNode::setRightChildIndex(const uint32 right_child_index)
{
  right_child_index_ = right_child_index;
}

/*!
  \details
  No detailed.
  */
inline
void BvhNode::setSahCost(const Float cost)
{
  cost_ = zisc::cast<float>(cost);
}

} // namespace nanairo

#endif // _NANAIRO_BVH_NODE_INL_HPP_
