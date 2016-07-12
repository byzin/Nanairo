/*!
  \file floating_point.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FLOATING_POINT_HPP
#define NANAIRO_FLOATING_POINT_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
template <uint> class Color;
template <uint> class SampledSpectra;
class SpectralDistribution;

//! \addtogroup Core 
//! \{

//! Check if the float value is not between 0 and 1
bool isBetweenZeroAndOneFloat(const Float value) noexcept;

//! Check if the sample contains the value that is not between 0 and 1
template <uint kSampleSize>
bool isBetweenZeroAndOneFloat(const SampledSpectra<kSampleSize>& sample) noexcept;

//! Check if the sample contains the value that is not between 0 and 1
bool isBetweenZeroAndOneFloat(const SpectralDistribution& distribution) noexcept;

//! Check if the color values are not between 0 and 1
template <uint kN>
bool isBetweenZeroAndOneFloat(const Color<kN>& color) noexcept;

//! Check if the float value is inf
bool isInfFloat(const Float value) noexcept;

//! Check if the sample contains inf value
template <uint kSampleSize>
bool hasInfFloat(const SampledSpectra<kSampleSize>& sample) noexcept;

//! Check if spectra data contains inf value
bool hasInfFloat(const SpectralDistribution& sample) noexcept;

//! Check if the float value is nan
bool isNanFloat(const Float value) noexcept;

//! Check if the sample contains nan value
template <uint kSampleSize>
bool hasNanFloat(const SampledSpectra<kSampleSize>& sample) noexcept;

//! Check if the spectra data contains nan value
bool hasNanFloat(const SpectralDistribution& property) noexcept;

//! Check if the float value is negative
bool isNegativeFloat(const Float value) noexcept;

//! Check if the sample contains negative value
template <uint kSampleSize>
bool hasNegativeFloat(const SampledSpectra<kSampleSize>& sample) noexcept;

//! Check if the spectra data contains negative value
bool hasNegativeFloat(const SpectralDistribution& property) noexcept;

//! Check if the float value is 0
bool isZeroFloat(const Float value) noexcept;

//! Check if the sample contains zero value 
template <uint kSampleSize>
bool hasZeroFloat(const SampledSpectra<kSampleSize>& sample) noexcept;

//! Check if the spectra data contains zero value
bool hasZeroFloat(const SpectralDistribution& property) noexcept;

//! Check if the vector has zero value
bool hasZeroFloat(const Vector3& vector) noexcept;

//! Check if the vector is unit vector
bool isUnitVector(const Vector3& vector) noexcept;

//! Check if the vector is zero vector
bool isZeroVector(const Vector3& vector) noexcept;

//! \} Core

} // namespace nanairo

#include "floating_point-inl.hpp"

#endif // NANAIRO_FLOATING_POINT_HPP
