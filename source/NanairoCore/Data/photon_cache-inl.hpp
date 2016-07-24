/*!
  \file photon_cache-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_PHOTON_CACHE_INL_HPP_
#define _NANAIRO_PHOTON_CACHE_INL_HPP_

#include "photon_cache.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
PhotonCache<kSampleSize>::PhotonCache(const Spectra& energy,
                                      const Point3& point,
                                      const Vector3& vin,
                                      const bool wavelength_is_selected) :
    energy_{energy},
    point_{point},
    vin_{vin},
    wavelength_is_selected_{wavelength_is_selected}
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
const Vector3& PhotonCache<kSampleSize>::incidentDirection() const
{
  return vin_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
const Point3& PhotonCache<kSampleSize>::point() const
{
  return point_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto PhotonCache<kSampleSize>::energy() const -> const Spectra&
{
  return energy_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
bool PhotonCache<kSampleSize>::wavelengthIsSelected() const
{
  return wavelength_is_selected_;
}

} // namespace nanairo

#endif // _NANAIRO_PHOTON_CACHE_INL_HPP_
