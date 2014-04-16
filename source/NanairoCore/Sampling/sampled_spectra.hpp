/*!
  \file sampled_spectra.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SAMPLED_SPECTRA_HPP_
#define _NANAIRO_SAMPLED_SPECTRA_HPP_

// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

namespace nanairo {

// Forward declaration
template <uint> class SampledWavelengths;
class SpectralDistribution;

//! \addtogroup Core 
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class SampledSpectra
{
 public:
  using Intensities = IntensitySamples<kSampleSize>;
  using Wavelengths = WavelengthSamples<kSampleSize>;


  //! Create samples
  SampledSpectra();

  //! Create samples
  SampledSpectra(const Wavelengths& wavelengths);

  //! Create samples
  SampledSpectra(const Wavelengths& wavelengths, const Float intensity);

  //! Create samples
  SampledSpectra(const Wavelengths& wavelengths, const Intensities& intensities);


  //! Apply addition operation
  SampledSpectra operator+(const Float scalar) const;

  //! Apply addition operation to each sample
  SampledSpectra operator+(const SampledSpectra& samples) const;

  //! Apply subtraction operation
  SampledSpectra operator-(const Float scalar) const;

  //! Apply subtraction operation to each sample
  SampledSpectra operator-(const SampledSpectra& samples) const;

  //! Multiply each sample with a scalar
  SampledSpectra operator*(const Float scalar) const;

  //! Apply multiplication operation to each sample
  SampledSpectra operator*(const SampledSpectra& samples) const;

  //! Apply division operation
  SampledSpectra operator/(const SampledSpectra& samples) const;

  //! Add samples
  SampledSpectra& operator+=(const SampledSpectra& samples);

  //!
  SampledSpectra& operator-=(const SampledSpectra& samples);

  //!
  SampledSpectra& operator*=(const Float scalar);

  //! Multiply samples
  SampledSpectra& operator*=(const SampledSpectra& samples);


  // Get the average of intensities
  Float average() const;

  //! Get the intensity of wavelength by the index.
  Float intensity(const uint index) const;

  //! Check whether intensities of all elements are zero
  bool isZero() const;

  //! Get max intensity
  Float max() const;

  //! Get the number of samples
  static constexpr uint size() 
  {
    return kSampleSize;
  }

  //! Set the intensity by the index
  void setIntensity(const uint index, const Float intensity);

  //! Get the wavelength by the index
  uint16 wavelength(const uint index) const;

  //! Get the sampled wavelength array
  const Wavelengths& wavelengths() const;

 private:
  Intensities intensities_;
  const Wavelengths* wavelengths_;
};

//! Multiply each sample with a scalar
template <uint kSampleSize>
SampledSpectra<kSampleSize> operator*(const Float scalar, 
                                      const SampledSpectra<kSampleSize>& samples);

//! Make a sampled spectra
template <uint kSampleSize>
SampledSpectra<kSampleSize> makeSampledSpectra(
    const SampledWavelengths<kSampleSize>& sampled_wavelengths);

//! Return the sqrt of sample
//SampledSpectra sqrt(const SampledSpectra& samples);

//! Return the sampled spectra
template <uint kSampleSize>
SampledSpectra<kSampleSize> sample(const SpectralDistribution& distribution,
                                   const WavelengthSamples<kSampleSize>& wavelengths);

//! \} Core

} // namespace nanairo

#include "sampled_spectra-inl.hpp"

#endif // _NANAIRO_SAMPLED_SPECTRA_HPP_
