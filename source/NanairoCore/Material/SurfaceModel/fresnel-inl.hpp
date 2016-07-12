/*!
  \file fresnel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FRESNEL_INL_HPP
#define NANAIRO_FRESNEL_INL_HPP

#include "fresnel.hpp"
// Standard C++ library
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/floating_point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution calculateFresnelConductorReflectance0(
    const SpectralDistribution& eta,
    const SpectralDistribution& eta_k) noexcept
{
  SpectralDistribution one;
  one.fill(1.0);

  const auto eta2_a = (eta - one) * (eta - one);
  const auto eta2_b = (eta + one) * (eta + one);
  const auto etak2 = eta_k * eta_k;
  auto reflectance_0deg = (eta2_a + etak2) / (eta2_b + etak2);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(reflectance_0deg),
              "Reflectance must be [0, 1].");
  return reflectance_0deg;
}

/*!
  \details
  No detailed.
  */
inline
Vector3 getFresnelReflectionDirection(const Vector3& vin, 
                                      const Vector3& normal,
                                      const Float cos_theta_ni) noexcept
{
  const auto vout = vin + (2.0 * cos_theta_ni) * normal;
  ZISC_ASSERT(isUnitVector(vout), "The vout is must be unit vector.");
  return vout;
}

/*!
  \details
  No detailed.
  */
inline
Vector3 getFresnelRefractionDirection(const Vector3& vin,
                                      const Vector3& normal,
                                      const Float cos_theta_ni,
                                      const Float n,
                                      const Float g) noexcept
{
  const auto vout = (vin + (cos_theta_ni - g) * normal) * (1.0 / n);
  ZISC_ASSERT(isUnitVector(vout), "The vout is must be unit vector.");
  return vout;
}

/*!
  \details
  No detailed.
  */
inline
std::tuple<bool, Float> evaluateFresnelG(const Float n, 
                                         const Float cos_theta_ni) noexcept
{
  const Float g2 = n * n + cos_theta_ni * cos_theta_ni - 1.0;
  return (g2 < 0.0) 
      ? std::make_tuple(false, 0.0) 
      : std::make_tuple(true, zisc::sqrt(g2));
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledSpectra<kSampleSize> solveFresnelConductorEquation(
    const Float cos_theta_ni,
    const SampledSpectra<kSampleSize>& reflectance_0deg) noexcept
{
  const auto& wavelengths = reflectance_0deg.wavelengths();
  const SampledSpectra<kSampleSize> one{wavelengths, 1.0};
  const Float tmp = zisc::power<5>(1.0 - cos_theta_ni);
  const auto reflectance = reflectance_0deg + 
                           (one - reflectance_0deg) * tmp;
  ZISC_ASSERT(isBetweenZeroAndOneFloat(reflectance), 
              "Reflectance must be [0, 1].");
  return reflectance;
}

/*!
  \details
  No detailed.
  */
inline
Float solveFresnelDielectricEquation(const Float cos_theta_ni, 
                                     const Float g) noexcept
{
  const Float a = g + cos_theta_ni,
              b = g - cos_theta_ni;
  const Float tmp1 = b / a,
              tmp2 = (cos_theta_ni * a - 1.0) / (cos_theta_ni * b + 1.0);
  const Float reflectance = 0.5 * tmp1 * tmp1 * (1.0 + tmp2 * tmp2);
  ZISC_ASSERT(isBetweenZeroAndOneFloat(reflectance), 
              "Reflectance must be [0, 1].");
  return reflectance;
}

} // namespace refelct

#endif // NANAIRO_FRESNEL_INL_HPP
