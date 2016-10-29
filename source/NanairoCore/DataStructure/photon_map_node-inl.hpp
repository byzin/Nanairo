/*!
  \file photon_map_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
template <uint kSampleSize> inline
PhotonMapNode<kSampleSize>::PhotonMapNode(
    const Spectra& energy,
    const Point3& point,
    const Vector3& vin,
    const bool wavelength_is_selected) noexcept :
        cache_{energy, point, vin, wavelength_is_selected},
        type_{NodeType::kXAxisSplit}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
PhotonMapNode<kSampleSize>::PhotonMapNode(const Cache& cache) noexcept :
    cache_{cache},
    type_{NodeType::kXAxisSplit}
{
}

/*!
  */
template <uint kSampleSize> inline
PhotonMapNode<kSampleSize>::PhotonMapNode(PhotonMapNode&& other) noexcept :
    cache_{std::move(other.cache_)},
    type_{other.type_}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto PhotonMapNode<kSampleSize>::cache() const noexcept -> const Cache&
{
  return cache_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto PhotonMapNode<kSampleSize>::nodeType() const noexcept -> NodeType
{
  return type_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
const Point3& PhotonMapNode<kSampleSize>::point() const noexcept
{
  return cache_.point();
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void PhotonMapNode<kSampleSize>::setNodeType(const NodeType type) noexcept
{
  type_ = type;
}

/*!
  */
template <uint kSampleSize> inline
void PhotonMapNode<kSampleSize>::setNodeType(const uint type) noexcept
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
