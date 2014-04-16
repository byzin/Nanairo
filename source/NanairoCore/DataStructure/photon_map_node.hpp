/*!
  \file photon_map_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_PHOTON_MAP_NODE_HPP_
#define _NANAIRO_PHOTON_MAP_NODE_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"

namespace nanairo {

// Forward declaration
template <uint> class PhotonCache;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class PhotonMapNode
{
 public:
  using Cache = PhotonCache<kSampleSize>;


  //! Create a photon map node
  PhotonMapNode(const Cache* cache);


  //! Return the flag
  uint flag() const;

  //! Return the photon cache
  const Cache* photonCache() const;

  //! Return the point
  const Point3& point() const;

  //! Set the node information
  void setFlag(const uint flag);


  static constexpr uint kXAxisFlag = 0;
  static constexpr uint kYAxisFlag = 1;
  static constexpr uint kZAxisFlag = 2;
  static constexpr uint kLeafNodeFlag = 3;

 private:
  const Point3* point_;
  const Cache* cache_;
  uint flag_;
};

//! \} Core

} // namespace nanairo

#include "photon_map_node-inl.hpp"

#endif // _NANAIRO_PHOTON_MAP_NODE_HPP_
