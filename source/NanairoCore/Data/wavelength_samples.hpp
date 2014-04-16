/*!
  \file wavelength_samples.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_WAVELENGTH_SAMPLE_HPP_
#define _NANAIRO_WAVELENGTH_SAMPLE_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class WavelengthSamples
{
 public:
  //! Create wavelengths
  WavelengthSamples();


  //! Return the wavelength by the index
  uint16& operator[](const uint index);

  //! Return the wavelength by the index
  uint16 operator[](const uint index) const;


  //! Return the inverse probability of selection of primary wavelength
  static constexpr Float primaryInverseProbability();

  //! Return the primary wavelength index
  uint primaryWavelengthIndex() const;

  //! Set primary wavelength
  void setPrimaryWavelength(const uint index);

  //! Return the size
  static constexpr uint size();

 private:
  uint16 primary_index_; //!< The index of primary wavelength
  uint16 wavelengths_[kSampleSize];
};

//! \} Core

} // namespace nanairo

#include "wavelength_samples-inl.hpp"

#endif // _NANAIRO_WAVELENGTH_SAMPLE_HPP_
