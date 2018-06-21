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
  */
inline
PhotonCache::PhotonCache() noexcept
{
}

/*!
  */
inline
PhotonCache::PhotonCache(const SampledSpectra& energy,
                         const Point3& point,
                         const Vector3& vin,
                         const bool wavelength_is_selected) noexcept :
    energy_{energy},
    point_{point},
    vin_{vin},
    wavelength_is_selected_{wavelength_is_selected}
{
}

/*!
  */
inline
const SampledSpectra& PhotonCache::energy() const noexcept
{
  return energy_;
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& PhotonCache::incidentDirection() const noexcept
{
  return vin_;
}

/*!
  */
inline
Float PhotonCache::inversePdf() const noexcept
{
  return inverse_pdf_;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& PhotonCache::point() const noexcept
{
  return point_;
}

/*!
  */
inline
void PhotonCache::setEnergy(const SampledSpectra& e) noexcept
{
  energy_ = e;
}

/*!
  */
inline
void PhotonCache::setIncidentDirection(const Vector3& v) noexcept
{
  vin_ = v;
}

/*!
  */
inline
void PhotonCache::setInversePdf(const Float inverse_pdf) noexcept
{
  inverse_pdf_ = inverse_pdf;
}

/*!
  */
inline
void PhotonCache::setPoint(const Point3& p) noexcept
{
  point_ = p;
}

/*!
  */
inline
void PhotonCache::setWavelengthIsSelected(const bool is_selected) noexcept
{
  wavelength_is_selected_ = is_selected;
}

/*!
  */
inline
bool PhotonCache::wavelengthIsSelected() const noexcept
{
  return wavelength_is_selected_;
}

} // namespace nanairo

#endif // NANAIRO_PHOTON_CACHE_INL_HPP
