/*!
  \file knn_photon_list-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_KNN_PHOTON_LIST_INL_HPP_
#define _NANAIRO_KNN_PHOTON_LIST_INL_HPP_

#include "knn_photon_list.hpp"
// Standard C++ library
#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/photon_cache.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
KnnPhotonList<kSampleSize>::KnnPhotonList()
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto KnnPhotonList<kSampleSize>::operator[](const uint index) const 
    -> const PhotonPoint&
{
  return photon_list_[index];
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void KnnPhotonList<kSampleSize>::clear()
{
  photon_list_.clear();
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void KnnPhotonList<kSampleSize>::insert(const Float distance2, const Cache* photon)
{
  const auto compare = [](const PhotonPoint& a, const PhotonPoint& b)
  {
    return std::get<0>(a) < std::get<0>(b);
  };

  if (photon_list_.size() != k_) {
    photon_list_.emplace_back(distance2, photon);
  }
  else {
    if (distance2 < std::get<0>(photon_list_.front()))
      return;
    std::pop_heap(photon_list_.begin(), photon_list_.end(), compare);
    photon_list_.back() = std::make_tuple(distance2, photon);
  }
  std::push_heap(photon_list_.begin(), photon_list_.end(), compare);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
Float KnnPhotonList<kSampleSize>::inverseLongestDistance() const
{
  return zisc::invSqrt(std::get<0>(photon_list_.front()));
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
uint KnnPhotonList<kSampleSize>::k() const
{
  return k_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void KnnPhotonList<kSampleSize>::setK(const uint k)
{
  clear();
  k_ = k;
  photon_list_.reserve(k);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
uint KnnPhotonList<kSampleSize>::size() const
{
  return photon_list_.size();
}

} // namespace nanairo

#endif // _NANAIRO_KNN_PHOTON_LIST_INL_HPP_
