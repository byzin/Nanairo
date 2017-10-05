/*!
  \file morton_code.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MORTON_CODE_HPP
#define NANAIRO_MORTON_CODE_HPP

// Standard C++ library
#include <vector>
// Nanairo
#include "bvh_building_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core 
//! \{

/*!
  */
class MortonCode
{
 public:
  using CodeType = uint64;
  using Iterator = std::vector<MortonCode>::iterator;
  using ConstIterator = std::vector<MortonCode>::const_iterator;


  //! Create an invalid morton code
  MortonCode() noexcept;

  //! Create an invalid morton code
  MortonCode(const BvhBuildingNode* node, const CodeType code) noexcept;

  //! Move a morton code from other
  MortonCode(MortonCode&& other) noexcept;


  //! Move a morton code from other
  MortonCode& operator=(MortonCode&& other) noexcept;


  //! Calculate 63bit morton code
  static uint64 calc63bitCode(const Point3& position) noexcept;

  //! Return the morton code
  CodeType code() const noexcept;

  //! Find the split position
  static Iterator findSplit(const uint bit,
                            Iterator begin,
                            Iterator end) noexcept;

  //! Return the code for invalid morton code
  static constexpr CodeType invalidCode() noexcept;

  //! Make a morton code list
  static std::vector<MortonCode> makeList(
      const std::vector<BvhBuildingNode>& node_list) noexcept;

  //! Return the bvh node
  const BvhBuildingNode* node() const noexcept;

  //! Set the morton code
  void setCode(const CodeType code) noexcept;

  //! Set the node
  void setNode(const BvhBuildingNode* node) noexcept;

 private:
  const BvhBuildingNode* node_;
  CodeType code_;
};

//! Compare two morton codes
bool operator<(const MortonCode& lhs, const MortonCode& rhs) noexcept;

//! Swap two morton codes
void swap(MortonCode& a, MortonCode& b) noexcept;

//! \} Core

} // namespace nanairo

#include "morton_code-inl.hpp"

#endif // NANAIRO_MORTON_CODE_HPP
