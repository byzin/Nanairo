/*!
  \file photon_map_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_MAP_NODE_HPP
#define NANAIRO_PHOTON_MAP_NODE_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
class PhotonCache;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class PhotonMapNode
{
 public:
  enum class NodeType : uint
  {
    kXAxisSplit = 0,
    kYAxisSplit,
    kZAxisSplit,
    kLeaf
  };


  //! Create a photon map node
  PhotonMapNode() noexcept;

  //! Create a photon map node
  PhotonMapNode(const SampledSpectra& energy,
                const Point3& point,
                const Vector3& vin,
                const bool wavelength_is_selected) noexcept;

  //! Create a photon map node
  PhotonMapNode(const PhotonCache& cache) noexcept;


  //! Return a photon cache of the node
  PhotonCache& cache() noexcept;

  //! Return a photon cache of the node
  const PhotonCache& cache() const noexcept;

  //! Return a type of node
  NodeType nodeType() const noexcept;

  //! Return a point of node
  const Point3& point() const noexcept;

  //! Set a type of the node
  void setNodeType(const NodeType type) noexcept;

  //! Set a type of the node
  void setNodeType(const uint type) noexcept;

 private:
  PhotonCache cache_;
  NodeType type_;
};

//! \} Core

} // namespace nanairo

#include "photon_map_node-inl.hpp"

#endif // NANAIRO_PHOTON_MAP_NODE_HPP
