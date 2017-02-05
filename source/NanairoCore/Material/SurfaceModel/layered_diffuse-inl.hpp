/*!
  \file layered_diffuse-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LAYERED_DIFFUSE_INL_HPP
#define NANAIRO_LAYERED_DIFFUSE_INL_HPP

#include "layered_diffuse.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  */
inline
Float LayeredDiffuse::calcRi(const Float n) noexcept
{
  return toRi(n, calcRe(n));
}

/*!
  */
inline
Float LayeredDiffuse::evalBodyPdf(const Float cos_no) noexcept
{
  ZISC_ASSERT(0.0 <= cos_no, "The cos(no) is minus.");
  constexpr Float t = zisc::invert(zisc::kPi<Float>);
  const Float pdf = t * cos_no;
  return pdf;
}

/*!
  */
inline
Float LayeredDiffuse::calcTotalBodyReflectance(const Float n,
                                               const Float k_d,
                                               const Float ri) noexcept
{
  const Float rb = zisc::power<2>(n * (1.0 - ri)) * (k_d / (1.0 - k_d * ri));
  ZISC_ASSERT(zisc::isInClosedBounds(rb, 0.0, 1.0),
              "The total reflectance is out of range [0, 1]: ", rb);
  return rb;
}

/*!
  */
inline
Float LayeredDiffuse::toRi(const Float n, const Float re) noexcept
{
  const Float ri = 1.0 - (1.0 - re) / zisc::power<2>(n);
  ZISC_ASSERT(zisc::isInClosedBounds(ri, 0.0, 1.0),
              "The internal reflectance is out of range [0, 1]: ", ri);
  return ri;
}

/*!
  */
inline
Float LayeredDiffuse::toRe(const Float n, const Float ri) noexcept
{
  const Float re = 1.0 - (1.0 - ri) * zisc::power<2>(n);
  ZISC_ASSERT(zisc::isInClosedBounds(re, 0.0, 1.0),
              "The internal reflectance is out of range [0, 1]: ", re);
  return re;
}

} // namespace nanairo

#endif // NANAIRO_LAYERED_DIFFUSE_INL_HPP
