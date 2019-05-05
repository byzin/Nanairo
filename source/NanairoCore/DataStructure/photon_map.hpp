/*!
  \file photon_map.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_MAP_HPP
#define NANAIRO_PHOTON_MAP_HPP

// Standard C++ library
#include <atomic>
#include <cstddef>
#include <mutex>
#include <vector>
#include <utility>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "photon_map_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

// Forward declaration
class KnnPhotonList;
class SampledSpectra;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class PhotonMap : public zisc::NonCopyable<PhotonMap>
{
 public:
  //! Create a photon map
  PhotonMap() noexcept;


  //! Construct the photon map
  void construct(System& system) noexcept;

  //! Initialize node lists
  void initialize(System& system,
                  const std::size_t estimated_num_of_nodes) noexcept;

  //! Reset node lists
  void reset() noexcept;

  //! Search photons inside the circle on the same face
  void search(const Point3& point,
              const Vector3& normal,
              const Float radius2,
              const bool is_frontside_culling,
              const bool is_backside_culling,
              KnnPhotonList* photon_list) const noexcept;

  //! Store a photon cache
  void store(const Point3& point,
             const Vector3& vin,
             const SampledSpectra& photon_energy,
             const Float inverse_sampling_pdf,
             const bool wavelength_is_selected) noexcept;

 private:
  using NodeIterator = typename zisc::pmr::vector<PhotonMapNode*>::iterator;


  //! Return the longest axis
  uint getLongestAxis(NodeIterator begin, NodeIterator end) const noexcept;

  //!
  uint nextSearchIndex(const Point3& point,
                       const Float radius2,
                       uint index) const noexcept;

  //! Split at median
  template <bool threading>
  void splitAtMedian(System& system,
                     const uint number,
                     NodeIterator begin,
                     NodeIterator end) noexcept;

  //! Test if the node is in the circle
  void testInsideCircle(const Point3& point,
                        const Vector3& normal,
                        const Float radius2,
                        const PhotonMapNode* node,
                        const bool is_frontside_culling,
                        const bool is_backside_culling,
                        KnnPhotonList* photon_list) const noexcept;

  //! Check if the multithreading is enabled
  static constexpr bool threadingIsEnabled() noexcept;


  std::mutex lock_;
  zisc::UniqueMemoryPointer<zisc::pmr::vector<PhotonMapNode*>> node_list_;
  zisc::UniqueMemoryPointer<zisc::pmr::vector<PhotonMapNode>> node_body_list_;
  zisc::UniqueMemoryPointer<zisc::pmr::vector<const PhotonMapNode*>> tree_;
  std::atomic<std::size_t> num_of_nodes_;
};

//! \} Core

} // namespace nanairo

#include "photon_map-inl.hpp"

#endif // NANAIRO_PHOTON_MAP_HPP
