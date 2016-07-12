/*!
  \file floating_point.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "floating_point.hpp"
// Standard C++ library
#include <cmath>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
bool isBetweenZeroAndOneFloat(const Float value) noexcept
{
  return 0.0 <= value && value <= 1.0;
}

/*!
  \details
  No detailed.
  */
bool isBetweenZeroAndOneFloat(const SpectralDistribution& distribution) noexcept
{
  for (uint index = 0; index < distribution.size(); ++index) {
    if (!isBetweenZeroAndOneFloat(distribution[index]))
        return false;
  }
  return true;
}

/*!
  \details
  No detailed.
  */
bool isInfFloat(const Float value) noexcept
{
  return std::isinf(value);
}

/*!
  \details
  No detailed.
  */
bool hasInfFloat(const SpectralDistribution& distribution) noexcept
{
  for (uint index = 0; index < distribution.size(); ++index) {
    const Float intensity = distribution[index];
    if (isInfFloat(intensity))
      return true;
  }
  return false;
}

/*!
  \details
  No detailed.
  */
bool isNanFloat(const Float value) noexcept
{
  return std::isnan(value);
}

/*!
  \details
  No detailed.
  */
bool hasNanFloat(const SpectralDistribution& distribution) noexcept
{
  for (uint index = 0; index < distribution.size(); ++index) {
    const Float intensity = distribution[index];
    if (isNanFloat(intensity))
      return true;
  }
  return false;
}

/*!
  \details
  No detailed.
  */
bool isNegativeFloat(const Float value) noexcept
{
  return value < 0.0;
}

/*!
  \details
  No detailed.
  */
bool hasNegativeFloat(const SpectralDistribution& distribution) noexcept
{
  for (uint index = 0; index < distribution.size(); ++index) {
    const Float intensity = distribution[index];
    if (isNegativeFloat(intensity))
      return true;
  }
  return false;
}

/*!
  \details
  No detailed.
  */
bool isZeroFloat(const Float value) noexcept
{
  return value == 0.0;
}

/*!
  \details
  No detailed.
  */
bool hasZeroFloat(const SpectralDistribution& distribution) noexcept
{
  for (uint index = 0; index < distribution.size(); ++index) {
    const Float intensity = distribution[index];
    if (isZeroFloat(intensity))
      return true;
  }
  return false;
}

/*!
  \details
  No detailed.
  */
bool isUnitVector(const Vector3& vector) noexcept
{
  constexpr Float error = 1.0e-6;
  const Float norm = vector.norm();
  return (1.0 - error) < norm && norm < (1.0 + error);
}

/*!
  \details
  No detailed.
  */
bool isZeroVector(const Vector3& vector) noexcept
{
  return (vector.squareNorm() == 0.0);
}

} // namespace nanairo
