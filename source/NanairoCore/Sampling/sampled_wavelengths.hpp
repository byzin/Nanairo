/*!
  \file sampled_wavelengths.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SAMPLED_WAVELENGTHS_HPP_
#define _NANAIRO_SAMPLED_WAVELENGTHS_HPP_

// Standard C++ library
#include <cstdint>
#include <functional>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

namespace nanairo {

// Forward declaration
class CumulativeSpectralDistribution;
class Sampler;
class SceneSettings;
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
  SampledWavelengths();


  //! Return the inverse probability array
  const Intensities& inverseProbabilities() const;

  //! Select primary wavelength randomly
  template <uint kMax>
  void selectPrimaryWavelength(Sampler& sampler);

  //! Set wavelength
  void set(const uint index, 
           const uint16 wavelength, 
           const Float inverse_probability);

  //! Return the wavelength array
  const Wavelengths& wavelengths() const;

 private:
  Intensities inverse_probabilities_;
  Wavelengths wavelengths_;

 public:
  //! Return the size
  static constexpr uint size() 
  {
    return kSampleSize;
  }
};

template <uint kSampleSize>
using WavelengthSampler = std::function<SampledWavelengths<kSampleSize> (Sampler&)>;

//! Combine all light pdf
SpectralDistribution calculateLightPdf(const World& world);

//! Make light based wavelngth importance sampler
template <uint kSampleSize>
WavelengthSampler<kSampleSize> makeLightsBasedImportanceSampler(const World& world);

//! Make wavelngth importance sampler
template <uint kSampleSize>
WavelengthSampler<kSampleSize> makeImportanceSampler(
    const SpectralDistribution& inverse_pdf,
    const CumulativeSpectralDistribution& cdf);

//! Make a RGB sampler
template <uint kSampleSize>
WavelengthSampler<kSampleSize> makeRgbSampler();

//! Make a wavelengths sampler
template <uint kSampleSize>
WavelengthSampler<kSampleSize> makeWavelengthSampler(const SceneSettings& settings,
                                                     const World& world);

//! Sample RGB wavelengths
template <uint kSampleSize>
SampledWavelengths<kSampleSize> sampleRgbWavelengths(Sampler& sampler);

//! Sample wavelengths using stratified sampling method
template <uint kSampleSize>
SampledWavelengths<kSampleSize> sampleStratifiedWavelengths(Sampler& sampler);

//! Sample wavelengths randomly
template <uint kSampleSize>
SampledWavelengths<kSampleSize> sampleWavelengthsRandomly(Sampler& sampler);

//! Sample wavelengths regularly
template <uint kSampleSize>
SampledWavelengths<kSampleSize> sampleWavelengthsRegularly(Sampler& sampler);

//! \} Core 

} // namespace nanairo

#include "sampled_wavelengths-inl.hpp"

#endif // _NANAIRO_SAMPLED_WAVELENGTHS_HPP_
