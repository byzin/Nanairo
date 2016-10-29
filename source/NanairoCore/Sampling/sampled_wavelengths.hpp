/*!
  \file sampled_wavelengths.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_WAVELENGTHS_HPP
#define NANAIRO_SAMPLED_WAVELENGTHS_HPP

// Standard C++ library
#include <cstdint>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

namespace nanairo {

// Forward declaration
class Sampler;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class SampledWavelengths
{
 public:
  using Intensities = IntensitySamples<kSampleSize>;
  using Wavelengths = WavelengthSamples<kSampleSize>;


  //! Create a sampled wavelengths
  SampledWavelengths() noexcept;


  //! Return the inverse probability array
  const Intensities& inverseProbabilities() const noexcept;

  //! Select primary wavelength randomly
  template <uint kMax>
  void selectPrimaryWavelength(Sampler& sampler) noexcept;

  //! Set wavelength
  void set(const uint index,
           const uint16 wavelength,
           const Float inverse_probability) noexcept;

  //! Return the sample size
  static constexpr uint size() noexcept;

  //! Return the wavelength array
  const Wavelengths& wavelengths() const noexcept;

 private:
  Intensities inverse_probabilities_;
  Wavelengths wavelengths_;
};

//! \} Core

} // namespace nanairo

#include "sampled_wavelengths-inl.hpp"

#endif // NANAIRO_SAMPLED_WAVELENGTHS_HPP
