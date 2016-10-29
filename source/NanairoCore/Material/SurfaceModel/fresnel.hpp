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

/*!
  */
class Fresnel
{
 public:
  //! Calculate the fresnel 0degree reflectance for the conductor
  static SpectralDistribution calcConductorReflectance0(
      const SpectralDistribution& eta,
      const SpectralDistribution& eta_k) noexcept;

  //! Calculate the fresnel reflection direction
  static Vector3 calcReflectionDirection(const Vector3& vin,
                                         const Vector3& normal,
                                         const Float cos_ni) noexcept;

  //! Calculate the fresnel refraction direction
  static Vector3 calcRefractionDirection(const Vector3& vin,
                                         const Vector3& normal,
                                         const Float cos_ni,
                                         const Float n,
                                         const Float g) noexcept;

  //! Calculate the fresnel dielectric g term
  static std::tuple<bool, Float> evalG(const Float n, const Float cos_ni) noexcept;

  //! Solve fresnel conductor equation
  template <uint kSampleSize>
  static SampledSpectra<kSampleSize> evalConductorEquation(
      const Float cos_ni,
      const SampledSpectra<kSampleSize>& reflectance_0deg) noexcept;

  //! Solve the fresnel dielectric equation
  static Float evalDielectricEquation(const Float cos_ni, const Float g) noexcept;
};

} // namespace nanairo

#include "fresnel-inl.hpp"

#endif // NANAIRO_FRESNEL_HPP
