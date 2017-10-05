/*!
  \file bvh_tree_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_TREE_NODE_HPP
#define NANAIRO_BVH_TREE_NODE_HPP

// Standard C++ library
#include <limits>
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \brief BvhTreeNode is the optimized version of BvhNode class
  \details
  The data structure of BvhTreeNode is optimized for ray traversal.
  */
class BvhTreeNode
{
 public:
  //! Create a empty node
  BvhTreeNode() noexcept;


  //! Return the bounding box
  const Aabb& boundingBox() const noexcept;

  //! Check if the node is leaf node
  bool isLeafNode() const noexcept;

  //! Return the next node index if this node and a ray intersection test failed.
  uint32 failureNextIndex() const noexcept;

  //! Return the number of objects
  uint numOfObjects() const noexcept;

  //! Return the first object index
  uint32 objectIndex() const noexcept;

  //! Set the bounding box
  void setBoundingBox(const Aabb& bounding_box) noexcept;

  //! Set the failure next index
  void setFailureNextIndex(const uint32 failure_next_index) noexcept;

  //! Set the object info
  void setObjectInfo(const uint32 object_index, const uint num_of_objects) noexcept;

 private:
  //! Initialize the node
  void initialize() noexcept;

  //! Return the number of objects mask
  static constexpr uint32 numOfObjectsMask() noexcept;

  //! Return the number of objects shift
  static constexpr uint32 numOfObjectsShift() noexcept;

  //! Return the object index mask
  static constexpr uint32 objectIndexMask() noexcept;

  //! Return the bits of the object info
  uint32 objectInfo() const noexcept;


  Aabb bounding_box_;
  uint32 object_info_;
  uint32 failure_next_index_;
};

//! \} Core

} // namespace nanairo

#include "bvh_tree_node-inl.hpp"

#endif // NANAIRO_BVH_TREE_NODE_HPP
