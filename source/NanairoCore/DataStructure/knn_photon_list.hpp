/*!
  \file knn_photon_list.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_KNN_PHOTON_LIST_HPP_
#define _NANAIRO_KNN_PHOTON_LIST_HPP_

// Standard C++ library
#include <utility>
#include <tuple>
#include <vector>
// Nanairo
#include "NanairoCore/Data/photon_cache.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class KnnPhotonList
{
 public:
  using Cache = PhotonCache<kSampleSize>;
  using PhotonPoint = std::tuple<Float, const Cache*>;
  using PhotonList = std::vector<PhotonPoint>;


  //! Create knn photon list
  KnnPhotonList();


  //! Return the photon cache and distance by the index
  const PhotonPoint& operator[](const uint index) const;


  //! Clear the contents
  void clear();

  //! Insert a photon point if the distance is included in knn
  void insert(const Float distance2, const Cache* photon);

  //! Return the inverse longest distance of contents
  Float inverseLongestDistance() const;

  //! Return the k
  uint k() const;

  //! Set k
  void setK(const uint k);

  //! Return the size of list
  uint size() const;

 private:
  PhotonList photon_list_;
  uint k_;
};

//! \}

} // namespace nanairo

#include "knn_photon_list-inl.hpp"

#endif // _NANAIRO_KNN_PHOTON_LIST_HPP_
