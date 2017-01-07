/*!
  \file fresnel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FRESNEL_HPP
#define NANAIRO_FRESNEL_HPP

// Standard C++ library
#include <tuple>
#include <utility>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
template <uint> class SampledSpectra;

//! \addtogroup Core
//! \{

/*!
  */
class Fresnel
{
 public:
  //! Calculate the fresnel reflection direction
  static Vector3 calcReflectionDirection(const Vector3& vin,
                                         const Vector3& normal) noexcept;

  //! Calculate the fresnel refraction direction
  static Vector3 calcRefractionDirection(const Vector3& vin,
                                         const Vector3& normal,
                                         const Float n,
                                         const Float g) noexcept;

  //! Calculate the fresnel dielectric g term
  static std::tuple<bool, Float> evalG(const Float n,
                                       const Float cos_theta) noexcept;

  //! Calculate the fresnel 0degree reflectance for the conductor
  static SpectralDistribution evalFresnel0(
      const SpectralDistribution& eta,
      const SpectralDistribution& eta_k) noexcept;

  //! Solve fresnel conductor equation
  template <uint kSampleSize>
  static SampledSpectra<kSampleSize> evalFresnel(
      const Float cos_theta,
      const SampledSpectra<kSampleSize>& fresnel_0deg) noexcept;

  //! Solve the fresnel dielectric equation
  static Float evalFresnel(const Float n, const Float cos_theta) noexcept;

  //! Solve the fresnel dielectric equation
  static Float evalFresnelFromG(const Float cos_theta, const Float g) noexcept;
};

//! \} Core

} // namespace nanairo

#include "fresnel-inl.hpp"

#endif // NANAIRO_FRESNEL_HPP
