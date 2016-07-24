/*!
  \file photon_cache.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_PHOTON_CACHE_HPP_
#define _NANAIRO_PHOTON_CACHE_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class PhotonCache
{
 public:
  using Spectra = SampledSpectra<kSampleSize>;


  //! Create a photon cache
  PhotonCache(const Spectra& energy,
              const Point3& point, 
              const Vector3& vin, 
              const bool wavelength_is_selected);


  //! Return the incident direction to the cached point
  const Vector3& incidentDirection() const;

  //! Return the cached point
  const Point3& point() const;

  //! Return the cached radiance
  const Spectra& energy() const;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const;

 private:
  const Spectra energy_;
  const Point3 point_;
  const Vector3 vin_;
  bool wavelength_is_selected_;
};

//! \} Core

} // namespace nanairo

#include "photon_cache-inl.hpp"

#endif // _NANAIRO_PHOTON_CACHE_HPP_
