/*!
  \file xyz_color_matching_function.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_HPP
#define NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_HPP

// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "spectral_distribution.hpp"
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class SceneSettings;

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
  XyzColorMatchingFunction(const SceneSettings& settings) noexcept;


  //! In the emissive case, convert spectrums to XYZ using color matching function.
  XyzColor toXyzInEmissiveCase(const SpectralDistribution& spectra) const noexcept;

  //! In the nanairoive or transmissive case, convert spectrums to XYZ
  XyzColor toXyzInReflectiveCase(const SpectralDistribution& spectra) const noexcept;

  //! Return the x bar
  const SpectralDistribution& xBar() const noexcept;

  //! Return the y bar
  const SpectralDistribution& yBar() const noexcept;

  //! Return the z bar
  const SpectralDistribution& zBar() const noexcept;

 private:
  //! Initialize
  void initialize(const SceneSettings& settings) noexcept;

  //! Set standard illuminant
  void setStandardIlluminant(const SceneSettings& settings) noexcept;

  //! Set standard obverser
  void setStandardObserver(const SceneSettings& settings) noexcept;


  SpectralDistribution standard_illuminant_;
  SpectralDistribution x_bar_,
                       y_bar_,
                       z_bar_;
  Float k_;
};

//! \} Core

} // namespace nanairo

#include "xyz_color_matching_function-inl.hpp"

#endif // NANAIRO_XYZ_COLOR_MATCHING_FUNCTION_HPP
