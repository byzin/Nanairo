/*!
  \file spectral_distribution.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SPECTRAL_DISTRIBUTION_HPP_
#define _NANAIRO_SPECTRAL_DISTRIBUTION_HPP_

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
  SpectralDistribution();

  //! Create a distribution
  SpectralDistribution(const zisc::ArithmeticArray<Float, kSpectraSize>& property);


  //! Apply addition operation to each element
  SpectralDistribution operator+(const SpectralDistribution& other) const;

  //! Apply subtraction operation to each element
  SpectralDistribution operator-(const SpectralDistribution& other) const;

  //!
  SpectralDistribution operator*(const Float scalar) const;

  //! Apply multiplication operation to each element
  SpectralDistribution operator*(const SpectralDistribution& other) const;

  //!
  SpectralDistribution operator/(const Float scalar) const;

  //! Apply division operation to each element
  SpectralDistribution operator/(const SpectralDistribution& other) const;

  //!
  SpectralDistribution& operator+=(const SpectralDistribution& other);

  //!
  SpectralDistribution& operator-=(const SpectralDistribution& other);

  //! Apply multiplication operation to each element and asign the result
  SpectralDistribution& operator*=(const Float scalar);

  //!
  SpectralDistribution& operator*=(const SpectralDistribution& other);

  //!
  SpectralDistribution& operator/=(const Float scalar);

  //!
  SpectralDistribution& operator/=(const SpectralDistribution& other);

  //! Return the point of the distribution by the index
  Float& operator[](const uint index);

  //! Return the point of the distribution by the index
  const Float& operator[](const uint index) const;


  //! Clamp all elements to the range [min, max]
  void clamp(const Float min, const Float max);

  //! Apply gamma correction
  void correctGamma(const Float gamma);

  //! Fill all elements by the value
  void fill(const Float value);

  //! Return the element by the index
  Float get(const uint index) const;

  //! Return the element by the wavelength
  Float getByWavelength(const uint16 wavelength) const;

  //! Check if all components are zero
  bool isZero() const;

  //! Return the largest element
  Float max() const;

  //! Return the smallest element
  Float min() const;

  //! Return the normalized distribution
  SpectralDistribution normalized() const;

  //! Return the spectral distribution 
  const zisc::ArithmeticArray<Float, kSpectraSize>& distribution() const;

  //! Set value by the index
  void set(const uint index, const Float intensity);

  //! Set value by the wavelength
  void setByWavelength(const uint16 wavelength, const Float intensity);

  //! Get the size of property
  static constexpr uint size();

  //! Get sum of intensities
  Float sum() const;

  //! Return the emissive xyz color
  XyzColor toEmissiveXyz(const System& system) const; 

  //! Return the nanairoive xyz color
  XyzColor toReflectiveXyz(const System& system) const;

 private:

  zisc::ArithmeticArray<Float, kSpectraSize> distribution_;
};

//! Make a emissive spectra
SpectralDistribution makeEmissiveDistribution(const System& system,
                                              const SceneSettings& settings,
                                              const QString& prefix);

//! Make a nanairoive spectra
SpectralDistribution makeReflectiveDistribution(const System& system,
                                                const SceneSettings& settings,
                                                const QString& prefix);

SpectralDistribution makeEmissiveSpectra(const SceneSettings& settings, 
                                         const QString& prefix);

SpectralDistribution makeReflectiveSpectra(const SceneSettings& settings, 
                                           const QString& prefix);

//! Make a spectral property
SpectralDistribution makeSpectra(const SceneSettings& settings, 
                                 const QString& prefix);

//! Make a spectral property
SpectralDistribution makeSpectra(const QString& file_path);

//! Convert RGB to RGB spectra
SpectralDistribution toRgbSpectra(const RgbColor& color);

//! Convert RGB to spectra
SpectralDistribution toSpectra(const System& system, const RgbColor& color);

//! \} Core

} // namespace nanairo

#include "spectral_distribution-inl.hpp"

#endif // _NANAIRO_SPECTRAL_DISTRIBUTION_HPP_
