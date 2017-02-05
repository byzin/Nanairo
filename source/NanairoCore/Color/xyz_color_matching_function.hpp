/*!
  \file xyz_color_matching_function.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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

// Forward declaration
class QJsonObject;

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
  XyzColorMatchingFunction(const QJsonObject& settings) noexcept;


  //! Return the standard illuminant
  const SpectralDistribution& illuminant() const noexcept;

  //! Convert spectrums to XYZ for emitter.
  XyzColor toXyzForEmitter(const SpectralDistribution& spectra) const noexcept;

  //! Convert spectrums to XYZ for reflector.
  XyzColor toXyzForReflector(const SpectralDistribution& spectra) const noexcept;

  //! Return the observer x bar
  const SpectralDistribution& xBar() const noexcept;

  //! Return the observer y bar
  const SpectralDistribution& yBar() const noexcept;

  //! Return the observer z bar
  const SpectralDistribution& zBar() const noexcept;

 private:
  //! Initialize
  void initialize(const QJsonObject& settings) noexcept;

  //! Load standard illuminant
  void loadStandardIlluminant(const QJsonObject& settings) const noexcept;

  //! Set standard obverser
  void loadStandardObserver(const QJsonObject& settings) const noexcept;


  std::unique_ptr<SpectralDistribution> illuminant_;
  std::array<std::unique_ptr<SpectralDistribution>, 3> bar_;
};

//! \} Core

} // namespace nanairo

#include "xyz_color_matching_function-inl.hpp"

#endif // NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_HPP
