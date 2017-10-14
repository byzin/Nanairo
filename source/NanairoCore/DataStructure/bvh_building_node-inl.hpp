/*!
  \file bvh_building_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_BUILDING_NODE_INL_HPP
#define NANAIRO_BVH_BUILDING_NODE_INL_HPP

#include "bvh_building_node.hpp"
// Standard C++ library
#include <array>
#include <limits>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Shape/shape.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
BvhBuildingNode::BvhBuildingNode() noexcept :
    parent_index_{nullIndex()},
    left_child_index_{nullIndex()},
    right_child_index_{nullIndex()},
    cost_{std::numeric_limits<float>::max()}
{
  object_list_.fill(nullptr);
}

/*!
  \details
  No detailed.
  */
inline
BvhBuildingNode::BvhBuildingNode(const Object* object) noexcept :
    bounding_box_{object->shape().boundingBox()},
    parent_index_{BvhBuildingNode::nullIndex()},
    left_child_index_{BvhBuildingNode::nullIndex()},
    right_child_index_{BvhBuildingNode::nullIndex()},
    cost_{std::numeric_limits<float>::max()}
{
  object_list_.fill(nullptr);
  object_list_[0] = object;
}


/*!
  \details
  No detailed.
  */
inline
void BvhBuildingNode::addObject(const Object* object) noexcept
{
  ZISC_ASSERT(object_list_.back() == nullptr, 
              "The number of objects exceed the limit.");

  const auto index = numOfObjects();
  object_list_[index] = object;
  const auto num_of_objects = index + 1;
  if (num_of_objects == 1)
    setBoundingBox(object->shape().boundingBox());
  else
    setBoundingBox(combine(boundingBox(), object->shape().boundingBox()));
}

/*!
  \details
  No detailed.
  */
inline
void BvhBuildingNode::addObjectWithoutBoundingBox(const Object* object) noexcept 
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
const Aabb& BvhBuildingNode::boundingBox() const noexcept
{
  return bounding_box_;
}

/*!
  \details
  No detailed.
  */
inline
bool BvhBuildingNode::isLeafNode() const noexcept
{
  return object_list_[0] != nullptr;
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhBuildingNode::leftChildIndex() const noexcept
{
  return left_child_index_;
}

/*!
  */
inline
constexpr uint32 BvhBuildingNode::maxIndex() noexcept
{
  constexpr uint32 max_node_index = std::numeric_limits<uint32>::max() - 1;
  return max_node_index;
}

/*!
  */
inline
constexpr uint32 BvhBuildingNode::maxNumOfLeafs() noexcept
{
  uint32 node_objects = CoreConfig::maxNumOfNodeObjects() >> 1;
  uint32 max_leaf_nodes = std::numeric_limits<uint32>::max() >> 1;
  while (0 < node_objects) {
    node_objects = node_objects >> 1;
    max_leaf_nodes = max_leaf_nodes >> 1;
  }
  return max_leaf_nodes;
}

/*!
  */
inline
constexpr uint32 BvhBuildingNode::nullIndex() noexcept
{
  return std::numeric_limits<uint32>::max();
}

/*!
  \details
  No detailed.
  */
inline
uint BvhBuildingNode::numOfObjects() const noexcept
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
auto BvhBuildingNode::objectList() const noexcept -> const ObjectList&
{
  return object_list_;
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhBuildingNode::parentIndex() const noexcept
{
  return parent_index_;
}

/*!
  \details
  No detailed.
  */
inline
uint32 BvhBuildingNode::rightChildIndex() const noexcept
{
  return right_child_index_;
}

/*!
  \details
  No detailed.
  */
inline
Float BvhBuildingNode::sahCost() const noexcept
{
  return zisc::cast<Float>(cost_);
}

/*!
  \details
  No detailed.
  */
inline
void BvhBuildingNode::setBoundingBox(const Aabb& bounding_box) noexcept
{
  bounding_box_ = bounding_box;
}

/*!
  \details
  No detailed.
  */
inline
void BvhBuildingNode::setLeftChildIndex(const uint32 left_child_index) noexcept
{
  left_child_index_ = left_child_index;
}

/*!
  \details
  No detailed.
  */
inline
void BvhBuildingNode::setParentIndex(const uint32 parent_index) noexcept
{
  parent_index_ = parent_index;
}

/*!
  \details
  No detailed.
  */
inline
void BvhBuildingNode::setRightChildIndex(const uint32 right_child_index) noexcept
{
  right_child_index_ = right_child_index;
}

/*!
  \details
  No detailed.
  */
inline
void BvhBuildingNode::setSahCost(const Float cost) noexcept
{
  cost_ = zisc::cast<float>(cost);
}

} // namespace nanairo

#endif // NANAIRO_BVH_BUILDING_NODE_INL_HPP
