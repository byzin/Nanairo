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
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
template <uint> class SampledSpectra;

//! Calculate fresnel 0 degree reflectance for conductor
SpectralDistribution calculateFresnelConductorReflectance0(
    const SpectralDistribution& eta,
    const SpectralDistribution& eta_k) noexcept;

//! Calculate the fresnel nanairoion direction
Vector3 getFresnelReflectionDirection(const Vector3& vin,
                                      const Vector3& normal,
                                      const Float cos_theta_ni) noexcept;

//! Calculate the fresnel refraction direction
Vector3 getFresnelRefractionDirection(const Vector3& vin,
                                      const Vector3& normal,
                                      const Float cos_theta_ni,
                                      const Float n,
                                      const Float g) noexcept;

//! Calculate fresnel dielectric g term
std::tuple<bool, Float> evaluateFresnelG(const Float n, 
                                         const Float cos_theta_ni) noexcept;

//! Solve fresnel conductor equation
template <uint kSampleSize>
SampledSpectra<kSampleSize> solveFresnelConductorEquation(
    const Float cos_theta_ni,
    const SampledSpectra<kSampleSize>& reflectance_0deg) noexcept;

//! Solve fresnel dielectric equation
Float solveFresnelDielectricEquation(const Float cos_theta_ni, 
                                     const Float g) noexcept;

} // namespace nanairo

#include "fresnel-inl.hpp"

#endif // NANAIRO_FRESNEL_HPP
