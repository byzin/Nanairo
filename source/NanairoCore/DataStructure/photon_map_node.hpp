/*!
  \file photon_map_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
  enum class NodeType : uint
  {
    kXAxisSplit = 0,
    kYAxisSplit,
    kZAxisSplit,
    kLeaf
  };


  using Spectra = SampledSpectra<kSampleSize>;
  using Cache = PhotonCache<kSampleSize>;

  //! Create a photon map node
  PhotonMapNode(const Spectra& energy,
                const Point3& point,
                const Vector3& vin,
                const bool wavelength_is_selected) noexcept;

  //! Create a photon map node
  PhotonMapNode(const Cache& cache) noexcept;

  //! Create a photon map node
  PhotonMapNode(PhotonMapNode&& other) noexcept;


  //! Return the photon cache of the node
  const Cache& cache() const noexcept;

  //! Return the type of node
  NodeType nodeType() const noexcept;

  //! Return the point of node
  const Point3& point() const noexcept;

  //! Set the type of the node
  void setNodeType(const NodeType type) noexcept;

  //! Set the type of the node
  void setNodeType(const uint type) noexcept;

 private:
  Cache cache_;
  NodeType type_;
};

//! \} Core

} // namespace nanairo

#include "photon_map_node-inl.hpp"

#endif // NANAIRO_PHOTON_MAP_NODE_HPP
