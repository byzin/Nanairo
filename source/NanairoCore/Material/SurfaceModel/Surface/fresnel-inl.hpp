/*!
  \file fresnel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
  const Float cos_ni = zisc::dot(normal, vin);
  ZISC_ASSERT(0.0 <= cos_ni, "The cos is negative.");
  const auto vout = (2.0 * cos_ni) * normal - vin;
  ZISC_ASSERT(isUnitVector(vout), "The vout isn't unit vector.");
  return vout;
}

/*!
  \details
  n = (n_transmittance_side / n_incident_side)
  */
inline
Vector3 Fresnel::calcRefractionDirection(const Vector3& vin,
                                         const Vector3& normal,
                                         const Float n,
                                         const Float g) noexcept
{
  const Float cos_ni = zisc::dot(normal, vin);
  ZISC_ASSERT(0.0 <= cos_ni, "The cos is negative.");
  const auto vout = ((cos_ni - g) * normal - vin) * zisc::invert(n);
  ZISC_ASSERT(isUnitVector(vout), "The vout isn't unit vector.");
  return vout;
}

/*!
  \details
  n = (n_transmittance_side / n_incident_side)
  */
inline
bool Fresnel::checkSnellsLaw(const Float n,
                             const Float cos_i,
                             const Float cos_o) noexcept
{
  const Float sin_i2 = 1.0 - zisc::power<2>(cos_i);
  const Float sin_o2 = 1.0 - zisc::power<2>(cos_o);
  const bool is_valid = zisc::constant::isAlmostEqual(sin_i2,
                                                      zisc::power<2>(n) * sin_o2);
  return is_valid;
}

/*!
  \details
  n = (n_transmittance_side / n_incident_side)
  */
inline
Float Fresnel::evalG2(const Float n, const Float cos_theta) noexcept
{
  const Float g2 = zisc::power<2>(n) + zisc::power<2>(cos_theta) - 1.0;
  return g2;
}

/*!
  \details
  n = (n_transmittance_side / n_incident_side)
  */
inline
Float Fresnel::evalFresnel(const Float n, const Float cos_theta) noexcept
{
  const Float g2 = evalG2(n, cos_theta);
  const bool is_perfect_reflection = g2 <= 0.0;
  const Float fresnel = (!is_perfect_reflection)
      ? evalFresnelFromG(cos_theta, zisc::sqrt(g2))
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
  const Float t1 = b / a,
              t2 = (cos_theta * a - 1.0) / (cos_theta * b + 1.0);
  const Float fresnel = 0.5 * zisc::power<2>(t1) * (1.0 + zisc::power<2>(t2));
  ZISC_ASSERT(zisc::isInClosedBounds(fresnel, 0.0, 1.0),
              "The fresnel isn't [0, 1].");
  return fresnel;
}

} // namespace refelct

#endif // NANAIRO_FRESNEL_INL_HPP
