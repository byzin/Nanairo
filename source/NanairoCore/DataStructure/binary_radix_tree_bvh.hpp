/*!
  \file binary_radix_tree_bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BINARY_RADIX_TREE_BVH_HPP
#define NANAIRO_BINARY_RADIX_TREE_BVH_HPP

// Standard C++ library
#include <vector>
#include <utility>
// Zisc
#include "zisc/memory_resource.hpp"
// Nanairo
#include "bvh.hpp"
#include "bvh_building_node.hpp"
#include "morton_code.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class System;

//! \addtogroup Core 
//! \{

/*!
  \details
  For the details of this algorithm,
  please see the paper entitled 
  "Maximizing Parallelism in the Construction of BVHs, Octrees, and k-d Trees"
  https://research.nvidia.com/sites/default/files/publications/karras2012hpg_paper.pdf
  */
class BinaryRadixTreeBvh : public Bvh
{
 public:
  //! Create a binary radix BVH
  BinaryRadixTreeBvh(System& system,
                     const SettingNodeBase* settings) noexcept;


  //! Build a binary radix tree BVH
  static void constructBinaryRadixTreeBvh(
      System& system,
      const zisc::pmr::vector<Object>& object_list,
      zisc::pmr::vector<BvhBuildingNode>& tree) noexcept;

 private:
  //! Build a binary radix tree BVH
  void constructBvh(
      System& system,
      const zisc::pmr::vector<Object>& object_list,
      zisc::pmr::vector<BvhBuildingNode>& tree) const noexcept override;

  //! Split leaf node list using the morton code
  template <bool threading = false>
  static void split(System& system,
                    uint bit,
                    const uint32 index,
                    zisc::pmr::vector<BvhBuildingNode>& tree,
                    MortonCode::Iterator first,
                    MortonCode::Iterator begin,
                    MortonCode::Iterator end) noexcept;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_BINARY_RADIX_TREE_BVH_HPP
