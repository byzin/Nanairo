/*!
  \file xyz_color_matching_function.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_HPP
#define NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_HPP

// Standard C++ library
#include <array>
#include <memory>
// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "spectral_distribution.hpp"
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class XyzColorMatchingFunction
{
 public:
  //! Create a xyz color matching function
  XyzColorMatchingFunction() noexcept;


  //! Convert spectrums to XYZ for emitter.
  XyzColor toXyzForEmitter(const SpectralDistribution& spectra) const noexcept;

  //! Convert spectrums to XYZ for reflector.
  XyzColor toXyzForReflector(const SpectralDistribution& spectra) const noexcept;

  //! Return the observer x bar
  SpectralDistribution& xBar() noexcept;

  //! Return the observer x bar
  const SpectralDistribution& xBar() const noexcept;

  //! Return the observer y bar
  SpectralDistribution& yBar() noexcept;

  //! Return the observer y bar
  const SpectralDistribution& yBar() const noexcept;

  //! Return the observer z bar
  SpectralDistribution& zBar() noexcept;

  //! Return the observer z bar
  const SpectralDistribution& zBar() const noexcept;

 private:
  //! Calculate the XYZ CMF component by the wavelength and parameters
  Float calcValue(const Float wavelength,
                  const Float alpha,
                  const Float beta,
                  const Float gamma,
                  const Float delta) const noexcept;

  //! Calculate the X value by the wavelength
  Float calcX(const Float wavelength) const noexcept;

  //! Calculate the Y value by the wavelength
  Float calcY(const Float wavelength) const noexcept;

  //! Calculate the Z value by the wavelength
  Float calcZ(const Float wavelength) const noexcept;

  //! Initialize
  void initialize() noexcept;


  std::array<std::unique_ptr<SpectralDistribution>, 3> bar_;
};

//! \} Core

} // namespace nanairo

#include "xyz_color_matching_function-inl.hpp"

#endif // NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_HPP
