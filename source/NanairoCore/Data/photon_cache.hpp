/*!
  \file photon_cache.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_CACHE_HPP
#define NANAIRO_PHOTON_CACHE_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class PhotonCache
{
 public:
  //! Create a photon cache
  PhotonCache(const SampledSpectra& energy,
              const Point3& point,
              const Vector3& vin,
              const bool wavelength_is_selected);


  //! Return the incident direction to the cached point
  const Vector3& incidentDirection() const;

  //! Return the cached point
  const Point3& point() const;

  //! Return the cached radiance
  const SampledSpectra& energy() const;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const;

 private:
  const SampledSpectra energy_;
  const Point3 point_;
  const Vector3 vin_;
  bool wavelength_is_selected_;
};

//! \} Core

} // namespace nanairo

#include "photon_cache-inl.hpp"

#endif // NANAIRO_PHOTON_CACHE_HPP
