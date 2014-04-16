/*!
  \file binary_radix_tree_bvh.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_BINARY_RADIX_TREE_BVH_HPP_
#define _NANAIRO_BINARY_RADIX_TREE_BVH_HPP_

// Standard C++ library
#include <vector>
#include <utility>
// Nanairo
#include "bvh.hpp"
#include "bvh_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class SceneSettings;
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
  BinaryRadixTreeBvh(const SceneSettings& settings, const QString& prefix);


  //! Build a binary radix tree BVH
  static void constructBinaryRadixTreeBvh(System& system,
                                          const std::vector<Object>& object_list,
                                          std::vector<BvhNode>& tree);

 private:
  //! Build a binary radix tree BVH
  void constructBvh(System& system,
                    const std::vector<Object>& object_list,
                    std::vector<BvhNode>& tree) const override;

  //! Split leaf node list in the morton code
  template <bool multithreading>
  static void splitInMortonCode(System& system,
                                const uint64 bit,
                                const uint32 index,
                                std::vector<BvhNode>& tree,
                                MortonCodeIterator first,
                                MortonCodeIterator begin,
                                MortonCodeIterator end);
};

//! \} Core

} // namespace nanairo

#endif // _NANAIRO_BINARY_RADIX_TREE_BVH_HPP_
