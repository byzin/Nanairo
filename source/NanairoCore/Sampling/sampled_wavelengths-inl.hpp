/*!
  \file sampled_wavelengths-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP
#define NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP

#include "sampled_wavelengths.hpp"
// Standard C++ library
#include <cstdint>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "Sampler/sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

namespace nanairo {

// Forward declaration
class World;

/*!
  \details
  No detailed.
  */
inline
SampledWavelengths::SampledWavelengths() noexcept
{
}

/*!
  */
inline
constexpr uint SampledWavelengths::size() noexcept
{
  return CoreConfig::wavelengthSampleSize();
}

/*!
  \details
  No detailed.
  */
inline
const IntensitySamples& SampledWavelengths::inverseProbabilities() const noexcept
{
  return inverse_probabilities_;
}

/*!
  \details
  No detailed.
  */
inline
void SampledWavelengths::selectPrimaryWavelength(
    Sampler& sampler,
    const PathState& path_state) noexcept
{
  constexpr Float n = zisc::cast<Float>(size());
  const Float r = sampler.draw1D(path_state);
  const uint selected_index = zisc::cast<uint>(n * r);
  ZISC_ASSERT(zisc::isInBounds(selected_index, 0u, size()),
              "The sampled index is out of range.");
  wavelengths_.setPrimaryWavelength(selected_index);
}

/*!
  \details
  No detailed.
  */
inline
void SampledWavelengths::set(const uint index,
                             const uint16 wavelength,
                             const Float inverse_probability) noexcept
{
  wavelengths_[index] = wavelength;
  inverse_probabilities_[index] = inverse_probability;
}

/*!
  \details
  No detailed.
  */
inline
const WavelengthSamples& SampledWavelengths::wavelengths() const noexcept
{
  return wavelengths_;
}

} // namespace nanairo

#endif // NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP
