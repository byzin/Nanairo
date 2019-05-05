/*!
  \file sampled_wavelengths.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_WAVELENGTHS_HPP
#define NANAIRO_SAMPLED_WAVELENGTHS_HPP

// Standard C++ library
#include <cstdint>
// Zisc
#include "zisc/arith_array.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

namespace nanairo {

// Forward declaration
class PathState;
class Sampler;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SampledWavelengths
{
 public:
  //! Create a sampled wavelengths
  SampledWavelengths() noexcept;


  //! Return the inverse probability array
  const IntensitySamples& inverseProbabilities() const noexcept;

  //! Select primary wavelength randomly
  void selectPrimaryWavelength(Sampler& sampler,
                               const PathState& path_state) noexcept;

  //! Set wavelength
  void set(const uint index,
           const uint16 wavelength,
           const Float inverse_probability) noexcept;

  //! Return the sample size
  static constexpr uint size() noexcept;

  //! Return the wavelength array
  const WavelengthSamples& wavelengths() const noexcept;

 private:
  IntensitySamples inverse_probabilities_;
  WavelengthSamples wavelengths_;
};

//! \} Core

} // namespace nanairo

#include "sampled_wavelengths-inl.hpp"

#endif // NANAIRO_SAMPLED_WAVELENGTHS_HPP
