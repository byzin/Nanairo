/*!
  \file photon_map_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_PHOTON_MAP_NODE_INL_HPP_
#define _NANAIRO_PHOTON_MAP_NODE_INL_HPP_

#include "photon_map_node.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
PhotonMapNode<kSampleSize>::PhotonMapNode(const Cache* cache) :
    point_{&(cache->point())},
    cache_{cache},
    flag_{0}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
uint PhotonMapNode<kSampleSize>::flag() const
{
  return flag_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto PhotonMapNode<kSampleSize>::photonCache() const -> const Cache*
{
  return cache_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
const Point3& PhotonMapNode<kSampleSize>::point() const
{
  return *point_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMapNode<kSampleSize>::setFlag(const uint flag)
{
  flag_ = flag;
}

} // namespace nanairo

#endif // _NANAIRO_PHOTON_MAP_NODE_INL_HPP_
