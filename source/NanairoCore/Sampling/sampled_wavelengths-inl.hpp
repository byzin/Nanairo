/*!
  \file sampled_wavelengths-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP
#define NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP

#include "sampled_wavelengths.hpp"
// Standard C++ library
#include <array>
#include <cstdint>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

namespace nanairo {

// Forward declaration
class World;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
SampledWavelengths<kSampleSize>::SampledWavelengths() noexcept
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SampledWavelengths<kSampleSize>::inverseProbabilities() const noexcept
    -> const Intensities&
{
  return inverse_probabilities_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> template <uint kMax> inline
void SampledWavelengths<kSampleSize>::selectPrimaryWavelength(Sampler& sampler) noexcept
{
  constexpr uint min = 0;
  const uint selected_index = sampler.sample(min, kMax);
  wavelengths_.setPrimaryWavelength(selected_index);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void SampledWavelengths<kSampleSize>::set(const uint index, 
                                          const uint16 wavelength,
                                          const Float inverse_probability) noexcept
{
  wavelengths_[index] = wavelength;
  inverse_probabilities_[index] = inverse_probability;
}

/*!
  */
template <uint kSampleSize> inline
constexpr uint SampledWavelengths<kSampleSize>::size() noexcept
{
  return kSampleSize;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
auto SampledWavelengths<kSampleSize>::wavelengths() const noexcept -> const Wavelengths&
{
  return wavelengths_;
}

} // namespace nanairo

#endif // NANAIRO_SAMPLED_WAVELENGTHS_INL_HPP
