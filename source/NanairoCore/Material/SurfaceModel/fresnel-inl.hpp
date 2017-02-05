/*!
  \file fresnel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Vector3 Fresnel::calcReflectionDirection(const Vector3& vin,
                                         const Vector3& normal) noexcept
{
  const Float cos_ni = -zisc::dot(normal, vin);
  const auto vout = vin + (2.0 * cos_ni) * normal;
  ZISC_ASSERT(isUnitVector(vout), "The vout isn't unit vector.");
  return vout;
}

/*!
  \details
  No detailed.
  */
inline
Vector3 Fresnel::calcRefractionDirection(const Vector3& vin,
                                         const Vector3& normal,
                                         const Float n,
                                         const Float g) noexcept
{
  const Float cos_ni = -zisc::dot(normal, vin);
  const auto vout = (vin + (cos_ni - g) * normal) * zisc::invert(n);
  ZISC_ASSERT(isUnitVector(vout), "The vout isn't unit vector.");
  return vout;
}

/*!
  \details
  No detailed.
  */
inline
std::tuple<bool, Float> Fresnel::evalG(const Float n,
                                       const Float cos_theta) noexcept
{
  const Float g2 = zisc::power<2>(n) + zisc::power<2>(cos_theta) - 1.0;
  return (0.0 < g2)
      ? std::make_tuple(true, zisc::sqrt(g2))
      : std::make_tuple(false, 0.0);
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution Fresnel::evalFresnel0(
    const SpectralDistribution& eta,
    const SpectralDistribution& eta_k) noexcept
{
  SpectralDistribution one;
  one.fill(1.0);

  const auto eta2_a = (eta - one) * (eta - one);
  const auto eta2_b = (eta + one) * (eta + one);
  const auto etak2 = eta_k * eta_k;
  auto fresnel_0deg = (eta2_a + etak2) / (eta2_b + etak2);
  ZISC_ASSERT(fresnel_0deg.isAllInBounds(0.0, 1.0),
              "Reflectances aren't [0, 1].");
  return fresnel_0deg;
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra Fresnel::evalFresnel(
    const Float cos_theta,
    const SampledSpectra& fresnel_0deg) noexcept
{
  const auto& wavelengths = fresnel_0deg.wavelengths();
  const SampledSpectra one{wavelengths, 1.0};
  const Float tmp = zisc::power<5>(1.0 - cos_theta);
  const auto reflectance = fresnel_0deg + (one - fresnel_0deg) * tmp;
  ZISC_ASSERT(reflectance.isAllInBounds(0.0, 1.0), "Reflectances aren't [0, 1].");
  return reflectance;
}

/*!
  */
inline
Float Fresnel::evalFresnel(const Float n, const Float cos_theta) noexcept
{
  const auto g_result = evalG(n, cos_theta);
  const bool is_perfect_reflection = !std::get<0>(g_result);
  const Float g = std::get<1>(g_result);
  const Float fresnel = (!is_perfect_reflection)
      ? evalFresnelFromG(cos_theta, g)
      : 1.0; // Perfect reflection
  ZISC_ASSERT(zisc::isInClosedBounds(fresnel, 0.0, 1.0),
              "The fresnel isn't [0, 1].");
  return fresnel;
}

/*!
  \details
  No detailed.
  */
inline
Float Fresnel::evalFresnelFromG(const Float cos_theta, const Float g) noexcept
{
  const Float a = g + cos_theta,
              b = g - cos_theta;
  const Float tmp1 = b / a,
              tmp2 = (cos_theta * a - 1.0) / (cos_theta * b + 1.0);
  const Float fresnel = (0.5 * zisc::power<2>(tmp1)) *
                        (1.0 + zisc::power<2>(tmp2));
  ZISC_ASSERT(zisc::isInClosedBounds(fresnel, 0.0, 1.0),
              "The fresnel isn't [0, 1].");
  return fresnel;
}

} // namespace refelct

#endif // NANAIRO_FRESNEL_INL_HPP
