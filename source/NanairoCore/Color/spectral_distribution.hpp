/*!
  \file spectral_distribution.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRAL_DISTRIBUTION_HPP
#define NANAIRO_SPECTRAL_DISTRIBUTION_HPP

// Standard C++ library
#include <memory>
// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class RgbColor;
class System;

//! \addtogroup Core
//! \{

using ColorRepresentationType = RenderingColorMode;

/*!
  \details
  No detailed.
  */
class SpectralDistribution
{
 public:
  enum class ColorType : int
  {
    kEmissive,
    kReflective
  };


  //! Create empty distribution
  SpectralDistribution() noexcept;

  //! Create a distribution
  SpectralDistribution(const zisc::ArithmeticArray<Float, CoreConfig::spectraSize()>& property) noexcept;


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
  void clampAll(const Float min, const Float max) noexcept;

  //! Apply gamma correction
  void correctGamma(const Float gamma) noexcept;

  //! Fill all elements by the value
  void fill(const Float value) noexcept;

  //! Return the element by the index
  Float get(const uint index) const noexcept;

  //! Return the element by the wavelength
  Float getByWavelength(const uint16 wavelength) const noexcept;

  //! Check if the distribution has inf value
  bool hasInf() const noexcept;

  //! Check if the distribution has nan value
  bool hasNan() const noexcept;

  //! Check if the distribution has negative value
  bool hasNegative() const noexcept;

  //! Check if the distribution has the specified value
  bool hasValue(const Float value) const noexcept;

  //! Check if all components are betwenn [ \p lower , \p upper )
  bool isAllInBounds(const Float lower, const Float upper) const noexcept;

  //! Check if all components are betwenn [ \p lower , \p upper ]
  bool isAllInClosedBounds(const Float lower, const Float upper) const noexcept;

  //! Check if all components are zero
  bool isAllZero() const noexcept;

  //! Check if the distribution is RGB distribution
  bool isRgbDistribution() const noexcept;

  //! Return the largest element
  Float max() const noexcept;

  //! Return the smallest element
  Float min() const noexcept;

  //! Compute the spectra based on the system color mode
  SpectralDistribution computeSystemColor(const System& system) const noexcept;

  //! Make a spectra
  static std::unique_ptr<SpectralDistribution> makeDistribution(
      const System& system,
      const SettingNodeBase* settings) noexcept;

  //! Return the normalized distribution
  SpectralDistribution normalized() const noexcept;

  //! Return the spectral distribution
  const zisc::ArithmeticArray<Float, CoreConfig::spectraSize()>& distribution() const noexcept;

  //! Set value by the index
  void set(const uint index, const Float intensity) noexcept;

  //! Set value by the wavelength
  void setByWavelength(const uint16 wavelength, const Float intensity) noexcept;

  //! Get the size of property
  static constexpr uint size() noexcept;

  //! Get sum of intensities
  Float sum() const noexcept;

  //! Convert to emissive color
  SpectralDistribution toEmissiveColor() const noexcept;

  //! Convert to emissive color
  SpectralDistribution toReflectiveColor() const noexcept;

  //! Return the xyz color converted from the spectra to
  XyzColor toXyzForEmitter(const System& system) const noexcept;

  //! Return the xyz color converted from the spectra to
  XyzColor toXyzForReflector(const System& system) const noexcept;

 private:
  //! Load the RGB data from the settings
  void loadRgb(const System& system,
               const SettingNodeBase* settings) noexcept;

  //! Load the spectra data from the settings
  void loadSpectra(const SettingNodeBase* settings) noexcept;

  //! Convert RGB color to spectra color
  SpectralDistribution toSpectraColor(const System& system) const noexcept;

  //! Convert spectra color to RGB color
  SpectralDistribution toRgbSpectraColor(const System& system) const noexcept;

  //! 
  template <ColorType type>
  static XyzColor toXyz(const System& system,
                        const SpectralDistribution& spectra) noexcept;

  //! Return the xyz color converted from the spectra to
  template <ColorType type>
  XyzColor toXyz(const System& system) const noexcept;


  zisc::ArithmeticArray<Float, CoreConfig::spectraSize()> distribution_;
};

//! \} Core

} // namespace nanairo

#include "spectral_distribution-inl.hpp"

#endif // NANAIRO_SPECTRAL_DISTRIBUTION_HPP
