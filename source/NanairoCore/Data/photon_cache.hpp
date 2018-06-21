/*!
  \file photon_cache.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
  PhotonCache() noexcept;

  //! Create a photon cache
  PhotonCache(const SampledSpectra& energy,
              const Point3& point,
              const Vector3& vin,
              const bool wavelength_is_selected) noexcept;


  //! Return a cached radiance
  const SampledSpectra& energy() const noexcept;

  //! Return an incident direction to the cached point
  const Vector3& incidentDirection() const noexcept;

  //! Return an inverse path sampling pdf
  Float inversePdf() const noexcept;

  //! Return a cached point
  const Point3& point() const noexcept;

  //! Set a radiance
  void setEnergy(const SampledSpectra& e) noexcept;

  //! Set an incident direction to the cached point
  void setIncidentDirection(const Vector3& v) noexcept;

  //! Set an inverse path sampling pdf
  void setInversePdf(const Float inverse_pdf) noexcept;

  //! Set a radiance
  void setPoint(const Point3& p) noexcept;

  //! Set a wavelength selection state
  void setWavelengthIsSelected(const bool is_selected) noexcept;

  //! Check if wavelength selection occured
  bool wavelengthIsSelected() const noexcept;

 private:
  SampledSpectra energy_;
  Point3 point_;
  Vector3 vin_;
  Float inverse_pdf_;
  bool wavelength_is_selected_;
};

//! \} Core

} // namespace nanairo

#include "photon_cache-inl.hpp"

#endif // NANAIRO_PHOTON_CACHE_HPP
