/*!
  \file fresnel.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "fresnel.hpp"
// Standard C++ library
#include <utility>
// Zisc
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  */
SampledSpectra Fresnel::evalFresnel(const SampledSpectra& n,
                                    const SampledSpectra& eta,
                                    const Float cos_theta) noexcept
{
  SampledSpectra reflectance;
  const Float cos_theta2 = zisc::power<2>(cos_theta);
  const Float sin_theta2 = 1.0 - cos_theta2;
  for (uint i = 0; i < SampledSpectra::size(); ++i) {
    const Float e = n.intensity(i);
    const Float ek = eta.intensity(i);

    const Float t1 = zisc::power<2>(e) - zisc::power<2>(ek) - sin_theta2;
    const Float ab2 = zisc::sqrt(zisc::power<2>(t1) + zisc::power<2>(2.0 * e * ek));
    const Float a = zisc::sqrt(0.5 * (ab2 + t1));

    const Float r_s = (ab2 - 2.0 * a * cos_theta + cos_theta2) /
                      (ab2 + 2.0 * a * cos_theta + cos_theta2);
    const Float t2 = 2.0 * a * cos_theta * sin_theta2;
    const Float t3 = (cos_theta2 * ab2 + zisc::power<2>(sin_theta2)) /
                     (cos_theta2 * ab2 + t2 + zisc::power<2>(sin_theta2));

    const Float r = r_s * t3;
    ZISC_ASSERT(zisc::isInClosedBounds(r, 0.0, 1.0),
                "The reflectance is out of range [0, 1].");
    reflectance.setIntensity(i, r);
  }
  return reflectance;
}

} // namespace nanairo
