/*!
  \file spectral_transport.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRAL_TRANSPORT_HPP
#define NANAIRO_SPECTRAL_TRANSPORT_HPP

// Standard C++ library
#include <array>
#include <utility>
// Zisc
#include "zisc/arith_array.hpp"
#include "zisc/memory_resource.hpp"
// Nanairo
#include "color.hpp"
#include "xyz_color.hpp"
#include "yxy_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "SpectralDistribution/spectral_distribution.hpp"

namespace nanairo {

/*!
  */
class SpectralTransport
{
 public:
  //! Convert XYZ to spectra
  static void toSpectra(const XyzColor& xyz,
                        SpectralDistribution* spectra,
                        zisc::pmr::memory_resource* work_resource) noexcept;

 private:
  using XystarColor = Color<2>;
  using UvColor = Color<2>;


  //! Convert 3-elements to 2-elements
  static zisc::ArithArray<Float, 2> dehom(
      const zisc::ArithArray<Float, 3>& x) noexcept;

  //! Convert 3-elements to 2-elements
  static zisc::ArithArray<Float, 3> hom(
      const zisc::ArithArray<Float, 2>& x) noexcept;

  //! Return the grid resolution
  static constexpr std::array<int, 2> gridResolution() noexcept;

  //! Return the grid size
  static constexpr int gridSize() noexcept;

  //! Convert XYZ to a spectrum
  static Float toSpectrum(const uint16 lambda,
                          const int cell_index,
                          const UvColor& uv,
                          zisc::pmr::memory_resource* work_resource) noexcept;

  //! Convert Yxy to UV
  static UvColor toUv(const YxyColor& yxy) noexcept;

  //! Convert Yxy to XYstar
  static XystarColor toXystar(const YxyColor& yxy) noexcept;
};

} // namespace nanairo

#endif // NANAIRO_SPECTRAL_TRANSPORT_HPP
