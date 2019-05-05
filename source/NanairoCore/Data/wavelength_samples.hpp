/*!
  \file wavelength_samples.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_WAVELENGTH_SAMPLE_HPP
#define NANAIRO_WAVELENGTH_SAMPLE_HPP

// Standard C++ library
#include <array>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class WavelengthSamples
{
 public:
  //! Create wavelengths
  WavelengthSamples() noexcept;


  //! Return the wavelength by the index
  uint16& operator[](const uint index) noexcept;

  //! Return the wavelength by the index
  const uint16& operator[](const uint index) const noexcept;


  //! Return the inverse probability of selection of primary wavelength
  static constexpr Float primaryInverseProbability() noexcept;

  //! Return the probability of selection of primary wavelength
  static constexpr Float primaryProbability() noexcept;

  //! Return the primary wavelength index
  uint primaryWavelengthIndex() const noexcept;

  //! Set primary wavelength
  void setPrimaryWavelength(const uint index) noexcept;

  //! Return the size
  static constexpr uint size() noexcept;

 private:
  uint16 primary_index_; //!< The index of primary wavelength
  std::array<uint16, CoreConfig::wavelengthSampleSize()> wavelengths_;
};

//! \} Core

} // namespace nanairo

#include "wavelength_samples-inl.hpp"

#endif // NANAIRO_WAVELENGTH_SAMPLE_HPP
