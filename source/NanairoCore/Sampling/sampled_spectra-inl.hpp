/*!
  \file sampled_spectra-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_SPECTRA_INL_HPP
#define NANAIRO_SAMPLED_SPECTRA_INL_HPP

#include "sampled_spectra.hpp"
// Standard C++ library
#include <cmath>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampled_wavelengths.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SampledSpectra::SampledSpectra() noexcept
{
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra::SampledSpectra(const WavelengthSamples& wavelengths) noexcept :
    wavelengths_{&wavelengths}
{
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra::SampledSpectra(const WavelengthSamples& wavelengths,
                               const Float intensity) noexcept :
    wavelengths_{&wavelengths}
{
  intensities_.fill(intensity);
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra::SampledSpectra(const WavelengthSamples& wavelengths,
                               const IntensitySamples& intensities) noexcept :
    intensities_{intensities},
    wavelengths_{&wavelengths}
{
}

/*!
  */
inline
constexpr uint SampledSpectra::size() noexcept
{
  return CoreConfig::wavelengthSampleSize();
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra SampledSpectra::operator+(const Float scalar) const noexcept
{
  return *this + SampledSpectra{*wavelengths_, scalar};
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra SampledSpectra::operator+(
    const SampledSpectra& samples) const noexcept
{
  return SampledSpectra{*wavelengths_,  intensities_ + samples.intensities_};
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra SampledSpectra::operator-(const Float scalar) const noexcept
{
  return *this - SampledSpectra{*wavelengths_, scalar};
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra SampledSpectra::operator-(
    const SampledSpectra& samples) const noexcept
{
  return SampledSpectra{*wavelengths_, intensities_ - samples.intensities_};
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra SampledSpectra::operator*(const Float scalar) const noexcept
{
  return SampledSpectra{*wavelengths_, intensities_ * scalar};
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra SampledSpectra::operator*(
    const SampledSpectra& samples) const noexcept
{
  return SampledSpectra{*wavelengths_, intensities_ * samples.intensities_};
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra SampledSpectra::operator/(
    const SampledSpectra& samples) const noexcept
{
  return SampledSpectra{*wavelengths_, intensities_ / samples.intensities_};
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra& SampledSpectra::operator+=(
    const SampledSpectra& samples) noexcept
{
  intensities_ += samples.intensities_;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra& SampledSpectra::operator-=(
    const SampledSpectra& samples) noexcept
{
  intensities_ -= samples.intensities_;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra& SampledSpectra::operator*=(
    const SampledSpectra& samples) noexcept
{
  intensities_ *= samples.intensities_;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
Float SampledSpectra::average() const noexcept
{
  using zisc::cast;

  constexpr Float averager = zisc::invert(cast<Float>(size()));
  return intensities_.sum() * averager;
}

/*!
  */
inline
bool SampledSpectra::hasInf() const noexcept
{
  return ::nanairo::hasInf(intensities_);
}

/*!
  */
inline
bool SampledSpectra::hasNan() const noexcept
{
  return ::nanairo::hasNan(intensities_);
}

/*!
  */
inline
bool SampledSpectra::hasNegative() const noexcept
{
  return ::nanairo::hasNegative(intensities_);
}

/*!
  */
inline
bool SampledSpectra::hasValue(const Float value) const noexcept
{
  return intensities_.hasValue(value);
}

/*!
  \details
  No detailed.
  */
inline
Float SampledSpectra::intensity(const uint index) const noexcept
{
  return intensities_[index];
}

/*!
  */
inline
bool SampledSpectra::isAllInBounds(const Float lower,
                                   const Float upper) const noexcept
{
  return intensities_.isAllInBounds(lower, upper);
}

/*!
  */
inline
bool SampledSpectra::isAllInClosedBounds(const Float lower,
                                         const Float upper) const noexcept
{
  return intensities_.isAllInClosedBounds(lower, upper);
}

/*!
  \details
  No detailed.
  */
inline
bool SampledSpectra::isAllZero() const noexcept
{
  return intensities_.isAllZero();
}

/*!
  \details
  No detailed.
  */
inline
Float SampledSpectra::max() const noexcept
{
  return intensities_.max();
}

/*!
  \details
  No detailed.
  */
inline
uint16 SampledSpectra::wavelength(const uint index) const noexcept
{
  return (*wavelengths_)[index];
}

/*!
  \details
  No detailed.
  */
inline
const WavelengthSamples& SampledSpectra::wavelengths() const noexcept
{
  return *wavelengths_;
}

/*!
  \details
  No detailed.
  */
inline
void SampledSpectra::setIntensity(const uint index,
                                  const Float intensity) noexcept
{
  intensities_.set(index, intensity);
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra operator*(const Float scalar,
                         const SampledSpectra& samples) noexcept
{
  return samples * scalar;
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra makeSampledSpectra(
    const SampledWavelengths& sampled_wavelengths) noexcept
{
  return SampledSpectra{sampled_wavelengths.wavelengths(),
                        sampled_wavelengths.inverseProbabilities()};
}

/*!
  \details
  No detailed.
  */
inline
SampledSpectra sample(const SpectralDistribution& distribution,
                      const WavelengthSamples& wavelengths) noexcept
{
  IntensitySamples intensities;
  for (uint index = 0; index < SampledSpectra::size(); ++index)
    intensities.set(index, distribution.getByWavelength(wavelengths[index]));
  return SampledSpectra{wavelengths, intensities};
}

} // namespace nanairo

#endif // _NANAIRO_SAMPLED_SPECTRA_INL_HPP_
