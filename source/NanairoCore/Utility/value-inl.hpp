/*!
  \file value-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_VALUE_INL_HPP
#define NANAIRO_VALUE_INL_HPP

#include "value.hpp"
// Standard C++ library
#include <cmath>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
constexpr uint getIndex(const uint16 wavelength) noexcept
{
  using zisc::cast;
  return cast<uint>(wavelength - CoreConfig::shortestWavelength()) /
         CoreConfig::wavelengthResolution();
}

/*!
  \details
  No detailed.
  */
inline
constexpr uint16 getWavelength(const uint index) noexcept
{
  using zisc::cast;
  return CoreConfig::shortestWavelength() +
         cast<uint16>(index * CoreConfig::wavelengthResolution());
}

/*!
  */
template <uint kN> inline
bool hasInf(const zisc::ArithmeticArray<Float, kN>& array) noexcept
{
  bool result = false;
  for (uint index = 0; (index < kN) && !result; ++index)
    result = std::isinf(array[index]);
  return result;
}

/*!
  */
template <uint kN> inline
bool hasNan(const zisc::ArithmeticArray<Float, kN>& array) noexcept
{
  bool result = false;
  for (uint index = 0; (index < kN) && !result; ++index)
    result = std::isnan(array[index]);
  return result;
}

/*!
  */
template <uint kN> inline
bool hasNegative(const zisc::ArithmeticArray<Float, kN>& array) noexcept
{
  bool result = false;
  for (uint index = 0; (index < kN) && !result; ++index)
    result = (array[index] < 0.0);
  return result;
}

} // namespace nanairo

#endif // NANAIRO_VALUE_INL_HPP
