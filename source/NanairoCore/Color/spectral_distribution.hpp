/*!
  \file spectral_distribution.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRAL_DISTRIBUTION_HPP
#define NANAIRO_SPECTRAL_DISTRIBUTION_HPP

// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward decralation
class QString;

namespace nanairo {

// Forward declaration
class RgbColor;
class SceneSettings;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SpectralDistribution
{
 public:
  //! Create empty distribution 
  SpectralDistribution() noexcept;

  //! Create a distribution
  SpectralDistribution(const zisc::ArithmeticArray<Float, kSpectraSize>& property) noexcept;


  //! Apply addition operation to each element
  SpectralDistribution operator+(const SpectralDistribution& other) const noexcept;

  //! Apply subtraction operation to each element
  SpectralDistribution operator-(const SpectralDistribution& other) const noexcept;

  //!
  SpectralDistribution operator*(const Float scalar) const noexcept;

  //! Apply multiplication operation to each element
  SpectralDistribution operator*(const SpectralDistribution& other) const noexcept;

  //!
  SpectralDistribution operator/(const Float scalar) const noexcept;

  //! Apply division operation to each element
  SpectralDistribution operator/(const SpectralDistribution& other) const noexcept;

  //!
  SpectralDistribution& operator+=(const SpectralDistribution& other) noexcept;

  //!
  SpectralDistribution& operator-=(const SpectralDistribution& other) noexcept;

  //! Apply multiplication operation to each element and asign the result
  SpectralDistribution& operator*=(const Float scalar) noexcept;

  //!
  SpectralDistribution& operator*=(const SpectralDistribution& other) noexcept;

  //!
  SpectralDistribution& operator/=(const Float scalar) noexcept;

  //!
  SpectralDistribution& operator/=(const SpectralDistribution& other) noexcept;

  //! Return the point of the distribution by the index
  Float& operator[](const uint index) noexcept;

  //! Return the point of the distribution by the index
  const Float& operator[](const uint index) const noexcept;


  //! Clamp all elements to the range [min, max]
  void clamp(const Float min, const Float max) noexcept;

  //! Apply gamma correction
  void correctGamma(const Float gamma) noexcept;

  //! Fill all elements by the value
  void fill(const Float value) noexcept;

  //! Return the element by the index
  Float get(const uint index) const noexcept;

  //! Return the element by the wavelength
  Float getByWavelength(const uint16 wavelength) const noexcept;

  //! Check if all components are zero
  bool isZero() const noexcept;

  //! Return the largest element
  Float max() const noexcept;

  //! Return the smallest element
  Float min() const noexcept;

  //! Return the normalized distribution
  SpectralDistribution normalized() const noexcept;

  //! Return the spectral distribution 
  const zisc::ArithmeticArray<Float, kSpectraSize>& distribution() const noexcept;

  //! Set value by the index
  void set(const uint index, const Float intensity) noexcept;

  //! Set value by the wavelength
  void setByWavelength(const uint16 wavelength, const Float intensity) noexcept;

  //! Get the size of property
  static constexpr uint size() noexcept;

  //! Get sum of intensities
  Float sum() const noexcept;

  //! Return the emissive xyz color
  XyzColor toEmissiveXyz(const System& system) const noexcept;

  //! Return the nanairoive xyz color
  XyzColor toReflectiveXyz(const System& system) const noexcept;

 private:

  zisc::ArithmeticArray<Float, kSpectraSize> distribution_;
};

//! Make a emissive spectra
SpectralDistribution makeEmissiveDistribution(const System& system,
                                              const SceneSettings& settings,
                                              const QString& prefix) noexcept;

//! Make a nanairoive spectra
SpectralDistribution makeReflectiveDistribution(const System& system,
                                                const SceneSettings& settings,
                                                const QString& prefix) noexcept;

SpectralDistribution makeEmissiveSpectra(const SceneSettings& settings, 
                                         const QString& prefix) noexcept;

SpectralDistribution makeReflectiveSpectra(const SceneSettings& settings, 
                                           const QString& prefix) noexcept;

//! Make a spectral property
SpectralDistribution makeSpectra(const SceneSettings& settings, 
                                 const QString& prefix) noexcept;

//! Make a spectral property
SpectralDistribution makeSpectra(const QString& file_path) noexcept;

//! Convert RGB to RGB spectra
SpectralDistribution toRgbSpectra(const RgbColor& color) noexcept;

//! Convert RGB to spectra
SpectralDistribution toSpectra(const System& system, const RgbColor& color) noexcept;

//! \} Core

} // namespace nanairo

#include "spectral_distribution-inl.hpp"

#endif // NANAIRO_SPECTRAL_DISTRIBUTION_HPP
