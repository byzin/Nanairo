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
#include <functional>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class CumulativeSpectralDistribution;
class Sampler;
class World;

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

  //! Return the wavelength array
  const Wavelengths& wavelengths() const noexcept;

 private:
  Intensities inverse_probabilities_;
  Wavelengths wavelengths_;

 public:
  //! Return the size
  static constexpr uint size() noexcept
  {
    return kSampleSize;
  }
};

template <uint kSampleSize>
using WavelengthSampler = std::function<SampledWavelengths<kSampleSize> (Sampler&)>;

//! Combine all light pdf
SpectralDistribution calculateLightPdf(const World& world) noexcept;

//! Make light based wavelngth importance sampler
template <uint kSampleSize>
WavelengthSampler<kSampleSize> makeLightsBasedImportanceSampler(const World& world) noexcept;

//! Make wavelngth importance sampler
template <uint kSampleSize>
WavelengthSampler<kSampleSize> makeImportanceSampler(
    const SpectralDistribution& inverse_pdf,
    const CumulativeSpectralDistribution& cdf) noexcept;

//! Make a RGB sampler
template <uint kSampleSize>
WavelengthSampler<kSampleSize> makeRgbSampler() noexcept;

//! Make a wavelengths sampler
template <uint kSampleSize>
WavelengthSampler<kSampleSize> makeWavelengthSampler(
    const World& world,
    const QJsonObject& settings) noexcept;

//! Sample RGB wavelengths
template <uint kSampleSize>
SampledWavelengths<kSampleSize> sampleRgbWavelengths(Sampler& sampler) noexcept;

//! Sample wavelengths using stratified sampling method
template <uint kSampleSize>
SampledWavelengths<kSampleSize> sampleStratifiedWavelengths(Sampler& sampler) noexcept;

//! Sample wavelengths randomly
template <uint kSampleSize>
SampledWavelengths<kSampleSize> sampleWavelengthsRandomly(Sampler& sampler) noexcept;

//! Sample wavelengths regularly
template <uint kSampleSize>
SampledWavelengths<kSampleSize> sampleWavelengthsRegularly(Sampler& sampler) noexcept;

//! \} Core 

} // namespace nanairo

#include "sampled_wavelengths-inl.hpp"

#endif // NANAIRO_SAMPLED_WAVELENGTHS_HPP
