/*!
  \file photon_map-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_MAP_INL_HPP
#define NANAIRO_PHOTON_MAP_INL_HPP

#include "photon_map.hpp"
// Standard C++ library
#include <vector>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "photon_map_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
uint PhotonMap::numOfNodes() const noexcept
{
  uint num_of_nodes = 0;
  for (const auto& node_list : thread_node_list_)
    num_of_nodes += zisc::cast<uint>(node_list.size());
  return num_of_nodes;
}

/*!
  */
inline
uint PhotonMap::numOfThreads() const noexcept
{
  return zisc::cast<uint>(thread_node_list_.size());
}

/*!
  */
inline
constexpr bool PhotonMap::threadingIsEnabled() noexcept
{
  return true;
}

} // namespace nanairo

#endif // NANAIRO_PHOTON_MAP_INL_HPP
