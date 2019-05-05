/*!
  \file value-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_VALUE_INL_HPP
#define NANAIRO_VALUE_INL_HPP

#include "value.hpp"
// Standard C++ library
#include <cmath>
// Zisc
#include "zisc/arith_array.hpp"
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
constexpr bool hasNegative(const zisc::ArithArray<Float, kN>& array) noexcept
{
  bool result = false;
  for (uint index = 0; (index < kN) && !result; ++index)
    result = zisc::isNegative(array[index]);
  return result;
}

} // namespace nanairo

#endif // NANAIRO_VALUE_INL_HPP
