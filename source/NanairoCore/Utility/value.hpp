/*!
  \file value.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_VALUE_HPP
#define NANAIRO_VALUE_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! Get the index from the wavelength
constexpr uint getIndex(const uint16 wavelength) noexcept;

//! Get the wavelength from the index
constexpr uint16 getWavelength(const uint index) noexcept;

//! \} Core

} // namespace nanairo

#include "value-inl.hpp"

#endif // NANAIRO_VALUE_HPP
