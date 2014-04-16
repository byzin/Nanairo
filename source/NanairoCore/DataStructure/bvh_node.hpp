/*!
  \file bvh_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_BVH_NODE_HPP_
#define _NANAIRO_BVH_NODE_HPP_

// Standard C++ library
#include <array>
#include <limits>
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
  BvhNode();

  //! Create a leaf node
  BvhNode(const Object* object);


  //! Add object
  void addObject(const Object* object);

  //! Add object without updating the BoundingBox
  void addObjectWithoutBoundingBox(const Object* object);

  //! Return the bounding box
  const Aabb& boundingBox() const;

  //! Check whether node is leaf node
  bool isLeafNode() const;

  //! Return the left child index
  uint32 leftChildIndex() const;

  //! Return the number of objects
  uint numOfObjects() const;

  //! Return the parent node index
  uint32 parentIndex() const;

  //! Return the right child node index
  uint32 rightChildIndex() const;

  //! Return the object list
  const std::array<const Object*, kMaxNumOfObjectsPerNode>& objectList() const;

  //! Return the SAH cost
  Float sahCost() const;

  //! Set the bounding box
  void setBoundingBox(const Aabb& bounding_box);

  //! Set left child node index
  void setLeftChildIndex(const uint32 left_child_index);

  //! Set parent node index
  void setParentIndex(const uint32 parent_index);

  //! Set right child node index
  void setRightChildIndex(const uint32 right_child_index);

  //! Set the SAH cost
  void setSahCost(const Float cost);

 private:
  Aabb bounding_box_;
  std::array<const Object*, kMaxNumOfObjectsPerNode> object_list_;
  uint32 parent_index_,
         left_child_index_,
         right_child_index_;
  float cost_;

 public:
  //! Return the max num of objects
  static constexpr uint32 maxNumOfObjects()
  {
    return std::numeric_limits<uint32>::max() >> 1;
  }

  //! Return the max node index
  static constexpr uint32 maxNodeIndex()
  {
    return std::numeric_limits<uint32>::max() - 1;
  }

  //! Return the non object index
  static constexpr uint32 nonObjectIndex()
  {
    return std::numeric_limits<uint32>::max();
  }
};

using MortonCode = std::tuple<BvhNode*, uint64>;
using MortonCodeIterator = std::vector<MortonCode>::iterator;
using NodeIterator = std::vector<BvhNode*>::iterator;

//! Calculate the morton code of the node
uint64 calc63bitMortonCode(const Float x, const Float y, const Float z);

//! Get the bounding box overlapping nodes
Aabb combineBoundingBoxs(std::vector<BvhNode>::const_iterator begin,
                         std::vector<BvhNode>::const_iterator end);

//! Get the bounding box overlapping nodes
Aabb combineBoundingBoxs(NodeIterator begin, NodeIterator end);

//! Find the split position
MortonCodeIterator findSplitPosition(const uint64 bit,
                                     MortonCodeIterator begin,
                                     MortonCodeIterator end);

//! Make a morton code list
std::vector<MortonCode> makeMortonCodeList(std::vector<BvhNode>& leaf_node_list);

//! \} Core

} // namespace nanairo

#include "bvh_node-inl.hpp"

#endif // _NANAIRO_BVH_NODE_HPP_
