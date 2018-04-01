/*!
  \file value.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_VALUE_HPP
#define NANAIRO_VALUE_HPP

// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! Get the index from the wavelength
constexpr uint getIndex(const uint16 wavelength) noexcept;

//! Get the wavelength from the index
constexpr uint16 getWavelength(const uint index) noexcept;

//! Check if the array has inf value
template <uint kN>
bool hasInf(const zisc::ArithmeticArray<Float, kN>& array) noexcept;

//! Check if the array has nan value
template <uint kN>
bool hasNan(const zisc::ArithmeticArray<Float, kN>& array) noexcept;

//! Check if the array has negative value
template <uint kN>
bool hasNegative(const zisc::ArithmeticArray<Float, kN>& array) noexcept;

//! \} Core

} // namespace nanairo

#include "value-inl.hpp"

#endif // NANAIRO_VALUE_HPP
