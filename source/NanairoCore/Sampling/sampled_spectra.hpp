/*!
  \file sampled_spectra.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
class SampledWavelengths;
class SpectralDistribution;

//! \addtogroup Core 
//! \{

/*!
  \details
  No detailed.
  */
class SampledSpectra
{
 public:
  //! Create samples
  SampledSpectra() noexcept;

  //! Create samples
  SampledSpectra(const WavelengthSamples& wavelengths) noexcept;

  //! Create samples
  SampledSpectra(const WavelengthSamples& wavelengths,
                 const Float intensity) noexcept;

  //! Create samples
  SampledSpectra(const WavelengthSamples& wavelengths,
                 const IntensitySamples& intensities) noexcept;


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
  static constexpr uint size() noexcept;

  //! Set the intensity by the index
  void setIntensity(const uint index, const Float intensity) noexcept;

  //! Get the wavelength by the index
  uint16 wavelength(const uint index) const noexcept;

  //! Get the sampled wavelength array
  const WavelengthSamples& wavelengths() const noexcept;

 private:
  IntensitySamples intensities_;
  const WavelengthSamples* wavelengths_;
};

//! Multiply each sample with a scalar
SampledSpectra operator*(const Float scalar,
                         const SampledSpectra& samples) noexcept;

//! Make a sampled spectra
SampledSpectra makeSampledSpectra(
    const SampledWavelengths& sampled_wavelengths) noexcept;

//! Return the sampled spectra
SampledSpectra sample(const SpectralDistribution& distribution,
                      const WavelengthSamples& wavelengths) noexcept;

//! \} Core

} // namespace nanairo

#include "sampled_spectra-inl.hpp"

#endif // NANAIRO_SAMPLED_SPECTRA_HPP
