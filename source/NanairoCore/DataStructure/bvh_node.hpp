/*!
  \file bvh_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_NODE_HPP
#define NANAIRO_BVH_NODE_HPP

// Standard C++ library
#include <array>
#include <tuple>
#include <vector>
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class Object;

//! \addtogroup Core 
//! \{

/*!
 \brief BVH node
 \details
 No detailed.
 */
class BvhNode
{
 public:
  //! Create empty node
  BvhNode() noexcept;

  //! Create a leaf node
  BvhNode(const Object* object) noexcept;


  //! Add object
  void addObject(const Object* object) noexcept;

  //! Add object without updating the BoundingBox
  void addObjectWithoutBoundingBox(const Object* object) noexcept;

  //! Return the bounding box
  const Aabb& boundingBox() const noexcept;

  //! Check whether node is leaf node
  bool isLeafNode() const noexcept;

  //! Return the left child index
  uint32 leftChildIndex() const noexcept;

  //! Return the max num of objects
  static constexpr uint32 maxNumOfObjects() noexcept;

  //! Return the max node index
  static constexpr uint32 maxNodeIndex() noexcept;

  //! Return the non object index
  static constexpr uint32 nonObjectIndex() noexcept;

  //! Return the number of objects
  uint numOfObjects() const noexcept;

  //! Return the parent node index
  uint32 parentIndex() const noexcept;

  //! Return the right child node index
  uint32 rightChildIndex() const noexcept;

  //! Return the object list
  const std::array<const Object*, CoreConfig::maxNumOfNodeObjects()>&
      objectList() const noexcept;

  //! Return the SAH cost
  Float sahCost() const noexcept;

  //! Set the bounding box
  void setBoundingBox(const Aabb& bounding_box) noexcept;

  //! Set left child node index
  void setLeftChildIndex(const uint32 left_child_index) noexcept;

  //! Set parent node index
  void setParentIndex(const uint32 parent_index) noexcept;

  //! Set right child node index
  void setRightChildIndex(const uint32 right_child_index) noexcept;

  //! Set the SAH cost
  void setSahCost(const Float cost) noexcept;

 private:
  Aabb bounding_box_;
  std::array<const Object*, CoreConfig::maxNumOfNodeObjects()> object_list_;
  uint32 parent_index_,
         left_child_index_,
         right_child_index_;
  float cost_;
};

using MortonCode = std::tuple<BvhNode*, uint64>;
using MortonCodeIterator = std::vector<MortonCode>::iterator;
using NodeIterator = std::vector<BvhNode*>::iterator;

//! Calculate the morton code of the node
uint64 calc63bitMortonCode(const Float x, const Float y, const Float z) noexcept;

//! Get the bounding box overlapping nodes
Aabb combineBoundingBoxs(std::vector<BvhNode>::const_iterator begin,
                         std::vector<BvhNode>::const_iterator end) noexcept;

//! Get the bounding box overlapping nodes
Aabb combineBoundingBoxs(NodeIterator begin, NodeIterator end) noexcept;

//! Find the split position
MortonCodeIterator findSplitPosition(const uint64 bit,
                                     MortonCodeIterator begin,
                                     MortonCodeIterator end) noexcept;

//! Make a morton code list
std::vector<MortonCode> makeMortonCodeList(std::vector<BvhNode>& leaf_node_list) noexcept;

//! \} Core

} // namespace nanairo

#include "bvh_node-inl.hpp"

#endif // NANAIRO_BVH_NODE_HPP
