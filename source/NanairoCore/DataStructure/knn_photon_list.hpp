/*!
  \file knn_photon_list.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_KNN_PHOTON_LIST_HPP
#define NANAIRO_KNN_PHOTON_LIST_HPP

// Standard C++ library
#include <tuple>
#include <utility>
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
class KnnPhotonList
{
 public:
  using PhotonPoint = std::tuple<Float, const PhotonCache*>;


  //! Create knn photon list
  KnnPhotonList() noexcept;


  //! Return the photon cache and distance by the index
  const PhotonPoint& operator[](const uint index) const noexcept;


  //! Clear the contents
  void clear() noexcept;

  //! Insert a photon point if the distance is included in knn
  void insert(const Float distance2, const PhotonCache* photon) noexcept;

  //! Return the inverse longest distance of contents
  Float inverseLongestDistance() const noexcept;

  //! Return the k value
  uint k() const noexcept;

  //! Set the k value
  void setK(const uint k) noexcept;

  //! Return the size of the knn list
  uint size() const noexcept;

 private:
  std::vector<PhotonPoint> photon_list_;
  uint k_;
};

//! \}

} // namespace nanairo

#include "knn_photon_list-inl.hpp"

#endif // NANAIRO_KNN_PHOTON_LIST_HPP
