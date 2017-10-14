/*!
  \file bvh_building_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_BUILDING_NODE_HPP
#define NANAIRO_BVH_BUILDING_NODE_HPP

// Standard C++ library
#include <array>
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
class BvhBuildingNode
{
 public:
  using ObjectList = std::array<const Object*, CoreConfig::maxNumOfNodeObjects()>;


  //! Create empty node
  BvhBuildingNode() noexcept;

  //! Create a leaf node
  BvhBuildingNode(const Object* object) noexcept;


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


  //! Return the max node index
  static constexpr uint32 maxIndex() noexcept;

  //! Return the max num of objects
  static constexpr uint32 maxNumOfLeafs() noexcept;

  //! Return the null index
  static constexpr uint32 nullIndex() noexcept;

  //! Return the number of objects
  uint numOfObjects() const noexcept;

  //! Return the parent node index
  uint32 parentIndex() const noexcept;

  //! Return the right child node index
  uint32 rightChildIndex() const noexcept;

  //! Return the object list
  const ObjectList& objectList() const noexcept;

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
  ObjectList object_list_;
  uint32 parent_index_,
         left_child_index_,
         right_child_index_;
  float cost_;
};

//! Get the bounding box overlapping nodes
Aabb combineBoundingBoxes(std::vector<BvhBuildingNode>::const_iterator begin,
                          std::vector<BvhBuildingNode>::const_iterator end) noexcept;

//! \} Core

} // namespace nanairo

#include "bvh_building_node-inl.hpp"

#endif // NANAIRO_BVH_BUILDING_NODE_HPP
