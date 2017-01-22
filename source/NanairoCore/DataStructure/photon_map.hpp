/*!
  \file photon_map.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_MAP_HPP
#define NANAIRO_PHOTON_MAP_HPP

// Standard C++ library
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <utility>
// Nanairo
#include "photon_map_node.hpp"
#include "NanairoCommon/keyword.hpp"
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
class PhotonMap
{
 public:
  //! Create a photon map
  PhotonMap(const System& system) noexcept;


  //! Clear the node list
  void clear() noexcept;

  //! Construct the photon map
  void construct(System& system) noexcept;

  //! Reserve storage
  void reserve(const uint num_of_thread_caches) noexcept;

  //! Search photons inside the circle on the same face
  void search(const Point3& point,
              const Vector3& normal,
              const Float radius2,
              KnnPhotonList* photon_list) const noexcept;

  //! Store a photon cache
  void store(const int thread_id,
             const Point3& point,
             const Vector3& vin,
             const SampledSpectra& photon_energy,
             const bool wavelength_is_selected) noexcept;

 private:
  using NodeIterator = typename std::vector<PhotonMapNode*>::iterator;


  //! Return the longest axis
  uint getLongestAxis(NodeIterator begin, NodeIterator end) const noexcept;

  //! Initialize
  void initialize(const System& system) noexcept;

  //!
  uint nextSearchIndex(const Point3& point,
                       const Float radius2,
                       uint index) const noexcept;

  //! Return the num of nodes 
  uint numOfNodes() const noexcept;

  //! Return the num of threads
  uint numOfThreads() const noexcept;

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
                        KnnPhotonList* photon_list) const noexcept;

  //! Check if the multithreading is enabled
  static constexpr bool threadingIsEnabled() noexcept;


  std::mutex lock_;
  std::vector<std::vector<PhotonMapNode>> thread_node_list_;
  std::vector<PhotonMapNode*> node_list_;
  std::vector<const PhotonMapNode*> tree_;
  std::atomic<uint> node_counter_;
};

//! \} Core

} // namespace nanairo

#include "photon_map-inl.hpp"

#endif // NANAIRO_PHOTON_MAP_HPP
