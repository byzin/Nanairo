/*!
  \file value-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_VALUE_INL_HPP_
#define _NANAIRO_VALUE_INL_HPP_

#include "value.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
constexpr uint getIndex(const uint16 wavelength)
{
  using zisc::cast;
  return cast<uint>(wavelength - kShortestWavelength) / kWavelengthResolution;
}

/*!
  \details
  No detailed.
  */
inline
constexpr uint16 getWavelength(const uint index)
{
  using zisc::cast;
  return kShortestWavelength + cast<uint16>(index * kWavelengthResolution);
}

} // namespace nanairo

#endif // _NANAIRO_VALUE_INL_HPP_
