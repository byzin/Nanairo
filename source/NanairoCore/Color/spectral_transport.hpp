/*!
  \file spectral_transport.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRAL_TRANSPORT_HPP
#define NANAIRO_SPECTRAL_TRANSPORT_HPP

// Standard C++ library
#include <vector>
#include <utility>
// Zisc
#include "zisc/array.hpp"
#include "zisc/matrix.hpp"
// Nanairo
#include "color.hpp"
#include "spectral_distribution.hpp"
#include "xyz_color.hpp"
#include "yxy_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
class SpectralTransport
{
 public:
  //! Convert XYZ to spectra
  static SpectralDistribution toSpectra(const XyzColor& xyz) noexcept;

 private:
  using XystarColor = Color<2>;
  using UvColor = Color<2>;


  //! Convert 3-elements to 2-elements
  static zisc::ArithmeticArray<Float, 2> dehom(
      const zisc::ArithmeticArray<Float, 3>& x) noexcept;

  //! Convert 3-elements to 2-elements
  static zisc::ArithmeticArray<Float, 3> hom(
      const zisc::ArithmeticArray<Float, 2>& x) noexcept;

  //! Return the grid resolution
  static constexpr zisc::Array<int, 2> gridResolution() noexcept;

  //! Return the grid size
  static constexpr int gridSize() noexcept;

  //! Convert XYZ to a spectrum
  static Float toSpectrum(const uint16 lambda,
                          const int cell_index,
                          const UvColor& uv) noexcept;

  //! Convert Yxy to UV
  static UvColor toUv(const YxyColor& yxy) noexcept;

  //! Convert Yxy to XYstar
  static XystarColor toXystar(const YxyColor& yxy) noexcept;
};

} // namespace nanairo

#endif // NANAIRO_SPECTRAL_TRANSPORT_HPP
