/*!
  \file vector.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vector.hpp"
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/utility.hpp"
#include "zisc/vector.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! Check if the vector contains the specified value
bool hasValue(const Vector3& vector, const Float value) noexcept
{
  const auto& array = vector.data();
  return array.hasValue(value);
}

//! Check if the vector is unit vector
bool isUnitVector(const Vector3& vector) noexcept
{
  constexpr Float error = 1.0e-6;
  return zisc::isInClosedBounds(vector.squareNorm(), 1.0 - error, 1.0 + error);
}

//! Check if the vector is zero vector
bool isZeroVector(const Vector3& vector) noexcept
{
  const auto& array = vector.data();
  return array.isAllZero();
}

} // namespace nanairo
