/*!
  \file photon_map_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_MAP_NODE_INL_HPP
#define NANAIRO_PHOTON_MAP_NODE_INL_HPP

#include "photon_map_node.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  */
inline
PhotonMapNode::PhotonMapNode() noexcept :
    type_{NodeType::kXAxisSplit}
{
}

/*!
  */
inline
PhotonMapNode::PhotonMapNode(const SampledSpectra& energy,
                             const Point3& point,
                             const Vector3& vin,
                             const bool wavelength_is_selected) noexcept :
    cache_{energy, point, vin, wavelength_is_selected},
    type_{NodeType::kXAxisSplit}
{
}

/*!
  */
inline
PhotonMapNode::PhotonMapNode(const PhotonCache& cache) noexcept :
    cache_{cache},
    type_{NodeType::kXAxisSplit}
{
}

/*!
  */
inline
PhotonCache& PhotonMapNode::cache() noexcept
{
  return cache_;
}

/*!
  */
inline
const PhotonCache& PhotonMapNode::cache() const noexcept
{
  return cache_;
}

/*!
  */
inline
auto PhotonMapNode::nodeType() const noexcept -> NodeType
{
  return type_;
}

/*!
  */
inline
const Point3& PhotonMapNode::point() const noexcept
{
  return cache_.point();
}

/*!
  */
inline
void PhotonMapNode::setNodeType(const NodeType type) noexcept
{
  type_ = type;
}

/*!
  */
inline
void PhotonMapNode::setNodeType(const uint type) noexcept
{
  switch (type) {
   case zisc::cast<uint>(NodeType::kXAxisSplit): {
    type_ = NodeType::kXAxisSplit;
    break;
   }
   case zisc::cast<uint>(NodeType::kYAxisSplit): {
    type_ = NodeType::kYAxisSplit;
    break;
   }
   case zisc::cast<uint>(NodeType::kZAxisSplit): {
    type_ = NodeType::kZAxisSplit;
    break;
   }
   case zisc::cast<uint>(NodeType::kLeaf): {
    type_ = NodeType::kLeaf;
    break;
   }
   default: {
    zisc::raiseError("PhotonMapError: Unsupported node type is specified.");
    break;
   }
  }
}

} // namespace nanairo

#endif // NANAIRO_PHOTON_MAP_NODE_INL_HPP
