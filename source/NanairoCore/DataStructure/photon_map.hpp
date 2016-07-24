/*!
  \file photon_map.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_PHOTON_MAP_HPP_
#define _NANAIRO_PHOTON_MAP_HPP_

// Standard C++ library
#include <future>
#include <list>
#include <vector>
#include <utility>
// Nanairo
#include "photon_map_node.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

// Forward declaration
template <uint> class KnnPhotonList;
template <uint> class SampledSpectra;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class PhotonMap
{
 public:
  using Cache = PhotonCache<kSampleSize>;
  using MapNode = PhotonMapNode<kSampleSize>;
  using PhotonList = KnnPhotonList<kSampleSize>;
  using Spectra = SampledSpectra<kSampleSize>;


  //! Create a photon map
  PhotonMap(System& system);


  //! Clear the contents
  void clear();

  //! Construct the photon map
  void construct(System& system);

  //! Reserve storage
  void reserve(System& system, const uint size);

  //! Search photons inside the circle
  void search(const Point3& point, 
              const Vector3& normal,
              const Float square_radius, 
              PhotonList& photon_list) const;

  //! Store a photon cache
  void store(const int thread_id,
             const Point3& point,
             const Vector3& vin,
             const Spectra& photon_energy,
             const bool wavelength_is_selected);

 private:
  using NodeIterator = typename std::vector<MapNode*>::iterator;


  //! Return the level of tree
  uint getTreeLevel(uint num_of_nodes) const;

  //! Return the longest axis
  uint getLongestAxis(NodeIterator begin, NodeIterator end) const;

  //! Initialize
  void initialize(System& system);

  //! Split at median
  void splitMedian(System& system, 
                   const uint number, 
                   NodeIterator begin, 
                   NodeIterator end);


  static constexpr uint kNonThreadLevel = 8;

  std::vector<std::vector<Cache>> thread_photon_cache_list_;
  std::vector<std::vector<MapNode>> thread_node_list_;
  std::vector<MapNode*> node_reference_list_;
  std::vector<const MapNode*> tree_;
  std::vector<std::future<void>> result_list_;
};

//! \} Core

} // namespace nanairo

#include "photon_map-inl.hpp"

#endif // _NANAIRO_PHOTON_MAP_HPP_
