/*!
  \file spectral_distribution-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRAL_DISTRIBUTION_INL_HPP
#define NANAIRO_SPECTRAL_DISTRIBUTION_INL_HPP

#include "spectral_distribution.hpp"
// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution::SpectralDistribution() noexcept
{
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution::SpectralDistribution(
    const zisc::ArithmeticArray<Float, kSpectraSize>& distribution) noexcept
        : distribution_{distribution}
{
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution SpectralDistribution::operator+(
    const SpectralDistribution& other) const noexcept
{
  return SpectralDistribution{distribution_ + other.distribution_};
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution SpectralDistribution::operator-(
    const SpectralDistribution& other) const noexcept
{
  return SpectralDistribution{distribution_ - other.distribution_};
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution SpectralDistribution::operator*(const Float scalar) const noexcept
{
  return SpectralDistribution{distribution_ * scalar};
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution SpectralDistribution::operator*(
    const SpectralDistribution& other) const noexcept
{
  return SpectralDistribution{distribution_ * other.distribution_};
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution SpectralDistribution::operator/(const Float scalar) const noexcept
{
  const Float inverse = 1.0 / scalar;
  return SpectralDistribution{distribution_ * inverse};
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution SpectralDistribution::operator/(
    const SpectralDistribution& other) const noexcept
{
  return SpectralDistribution{distribution_ / other.distribution_};
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& SpectralDistribution::operator+=(
    const SpectralDistribution& other) noexcept
{
  distribution_ += other.distribution_;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& SpectralDistribution::operator-=(
    const SpectralDistribution& other) noexcept
{
  distribution_ -= other.distribution_;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& SpectralDistribution::operator*=(const Float scalar) noexcept
{
  distribution_ *= scalar;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& SpectralDistribution::operator*=(
    const SpectralDistribution& other) noexcept
{
  distribution_ *= other.distribution_;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& SpectralDistribution::operator/=(const Float scalar) noexcept
{
  distribution_ /= scalar;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution& SpectralDistribution::operator/=(
    const SpectralDistribution& other) noexcept
{
  distribution_ /= other.distribution_;
  return *this;
}

/*!
  \details
  No detailed.
  */
inline
Float& SpectralDistribution::operator[](const uint index) noexcept
{
  return distribution_[index];
}

/*!
  \details
  No detailed.
  */
inline
const Float& SpectralDistribution::operator[](const uint index) const noexcept
{
  return distribution_[index];
}

/*!
  \details
  No detailed.
  */
inline
void SpectralDistribution::fill(const Float value) noexcept
{
  distribution_.fill(value);
}

/*!
  \details
  No detailed.
  */
inline
Float SpectralDistribution::get(const uint index) const noexcept
{
  return distribution_[index];
}

/*!
  \details
  No detailed.
  */
inline
Float SpectralDistribution::getByWavelength(const uint16 wavelength) const noexcept
{
  return get(getIndex(wavelength));
}

/*!
  \details
  No detailed.
  */
inline
bool SpectralDistribution::isZero() const noexcept
{
  return distribution_.isZeroArray();
}

/*!
  \details
  No detailed.
  */
inline
Float SpectralDistribution::max() const noexcept
{
  return distribution_.max();
}

/*!
  \details
  No detailed.
  */
inline
Float SpectralDistribution::min() const noexcept
{
  return distribution_.min();
}

/*!
  \details
  No detailed.
  */
inline
SpectralDistribution SpectralDistribution::normalized() const noexcept
{
  const Float k = 1.0 / sum();
  return *this * k;
}

/*!
  \details
  No detailed.
  */
inline
auto SpectralDistribution::distribution() const noexcept
    -> const zisc::ArithmeticArray<Float, kSpectraSize>&
{
  return distribution_;
}

/*!
  \details
  No detailed.
  */
inline
void SpectralDistribution::set(const uint index, const Float intensity) noexcept
{
  distribution_[index] = intensity;
}

/*!
  \details
  No detailed.
  */
inline
void SpectralDistribution::setByWavelength(const uint16 wavelength,
                                           const Float intensity) noexcept
{
  return set(getIndex(wavelength), intensity);
}

/*!
  \details
  No detailed.
  */
inline
constexpr uint SpectralDistribution::size() noexcept
{
  return kSpectraSize;
}

/*!
  \details
  No detailed.
  */
inline
Float SpectralDistribution::sum() const noexcept
{
  return distribution_.sum();
}

} // namespace nanairo

#endif // NANAIRO_SPECTRAL_DISTRIBUTION_INL_HPP
