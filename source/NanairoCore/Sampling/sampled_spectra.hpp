/*!
  \file sampled_spectra.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_SPECTRA_HPP
#define NANAIRO_SAMPLED_SPECTRA_HPP

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
  SampledSpectra() noexcept;

  //! Create samples
  SampledSpectra(const Wavelengths& wavelengths) noexcept;

  //! Create samples
  SampledSpectra(const Wavelengths& wavelengths, const Float intensity) noexcept;

  //! Create samples
  SampledSpectra(const Wavelengths& wavelengths, const Intensities& intensities) noexcept;


  //! Apply addition operation
  SampledSpectra operator+(const Float scalar) const noexcept;

  //! Apply addition operation to each sample
  SampledSpectra operator+(const SampledSpectra& samples) const noexcept;

  //! Apply subtraction operation
  SampledSpectra operator-(const Float scalar) const noexcept;

  //! Apply subtraction operation to each sample
  SampledSpectra operator-(const SampledSpectra& samples) const noexcept;

  //! Multiply each sample with a scalar
  SampledSpectra operator*(const Float scalar) const noexcept;

  //! Apply multiplication operation to each sample
  SampledSpectra operator*(const SampledSpectra& samples) const noexcept;

  //! Apply division operation
  SampledSpectra operator/(const SampledSpectra& samples) const noexcept;

  //! Add samples
  SampledSpectra& operator+=(const SampledSpectra& samples) noexcept;

  //!
  SampledSpectra& operator-=(const SampledSpectra& samples) noexcept;

  //!
  SampledSpectra& operator*=(const Float scalar) noexcept;

  //! Multiply samples
  SampledSpectra& operator*=(const SampledSpectra& samples) noexcept;


  // Get the average of intensities
  Float average() const noexcept;

  //! Check if the samples contain inf value
  bool hasInf() const noexcept;

  //! Check if the samples contain nan value
  bool hasNan() const noexcept;

  //! Check if the samples contain negative value
  bool hasNegative() const noexcept;

  //! Check if the samples contain the specified value
  bool hasValue(const Float value) const noexcept;

  //! Get the intensity of wavelength by the index.
  Float intensity(const uint index) const noexcept;

  //! Check if the each sample is between [ \p lower , \p upper )
  bool isAllInBounds(const Float lower, const Float upper) const noexcept;

  //! Check if the each sample is between [ \p lower , \p upper ]
  bool isAllInClosedBounds(const Float lower, const Float upper) const noexcept;

  //! Check whether intensities of all elements are zero
  bool isAllZero() const noexcept;

  //! Get max intensity
  Float max() const noexcept;

  //! Get the number of samples
  static constexpr uint size() noexcept 
  {
    return kSampleSize;
  }

  //! Set the intensity by the index
  void setIntensity(const uint index, const Float intensity) noexcept;

  //! Get the wavelength by the index
  uint16 wavelength(const uint index) const noexcept;

  //! Get the sampled wavelength array
  const Wavelengths& wavelengths() const noexcept;

 private:
  Intensities intensities_;
  const Wavelengths* wavelengths_;
};

//! Multiply each sample with a scalar
template <uint kSampleSize>
SampledSpectra<kSampleSize> operator*(const Float scalar, 
                                      const SampledSpectra<kSampleSize>& samples) noexcept;

//! Make a sampled spectra
template <uint kSampleSize>
SampledSpectra<kSampleSize> makeSampledSpectra(
    const SampledWavelengths<kSampleSize>& sampled_wavelengths) noexcept;

//! Return the sqrt of sample
//SampledSpectra sqrt(const SampledSpectra& samples);

//! Return the sampled spectra
template <uint kSampleSize>
SampledSpectra<kSampleSize> sample(const SpectralDistribution& distribution,
                                   const WavelengthSamples<kSampleSize>& wavelengths) noexcept;

//! \} Core

} // namespace nanairo

#include "sampled_spectra-inl.hpp"

#endif // NANAIRO_SAMPLED_SPECTRA_HPP
