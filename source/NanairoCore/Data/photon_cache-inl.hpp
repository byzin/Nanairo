/*!
  \file photon_cache-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_CACHE_INL_HPP
#define NANAIRO_PHOTON_CACHE_INL_HPP

#include "photon_cache.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
PhotonCache::PhotonCache(const SampledSpectra& energy,
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
inline
const Vector3& PhotonCache::incidentDirection() const
{
  return vin_;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& PhotonCache::point() const
{
  return point_;
}

/*!
  \details
  No detailed.
  */
inline
const SampledSpectra& PhotonCache::energy() const
{
  return energy_;
}

/*!
  \details
  No detailed.
  */
inline
bool PhotonCache::wavelengthIsSelected() const
{
  return wavelength_is_selected_;
}

} // namespace nanairo

#endif // NANAIRO_PHOTON_CACHE_INL_HPP
