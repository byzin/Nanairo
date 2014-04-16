/*!
  \file value.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_VALUE_HPP_
#define _NANAIRO_VALUE_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! Get the index from the wavelength
constexpr uint getIndex(const uint16 wavelength);

//! Get the wavelength from the index
constexpr uint16 getWavelength(const uint index);

//! \} Core

} // namespace nanairo

#include "value-inl.hpp"

#endif // _NANAIRO_VALUE_HPP_
