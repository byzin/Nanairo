/*!
  \file spectral_transport.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectral_transport.hpp"
// Standard C++ library
#include <cmath>
#include <vector>
// Zisc
#include "zisc/array.hpp"
#include "zisc/arithmetic_array.hpp"
#include "zisc/error.hpp"
#include "zisc/matrix.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "color.hpp"
#include "color_conversion.hpp"
#include "spectral_distribution.hpp"
#include "xyz_color.hpp"
#include "xyz_color_matching_function.hpp"
#include "yxy_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_transport_parameters.hpp"

namespace nanairo {

/*!
  */
constexpr zisc::Array<int, 2> SpectralTransport::gridResolution() noexcept
{
  using zisc::cast;
  return zisc::Array<int, 2>{cast<int>(spectral_transport::kGridResolution[0]),
                             cast<int>(spectral_transport::kGridResolution[1])};
}

/*!
  */
constexpr int SpectralTransport::gridSize() noexcept
{
  using zisc::cast;
  return cast<int>(spectral_transport::kGridResolution[0] *
                   spectral_transport::kGridResolution[1]);
}

/*!
  */
SpectralDistribution SpectralTransport::toSpectra(const XyzColor& xyz) noexcept
{
  using zisc::cast;

  SpectralDistribution spectra;
  const auto yxy = ColorConversion::toYxy(xyz);
  const auto uv = toUv(yxy); // Rotate to align with grid
  constexpr auto grid_res = gridResolution();
  if (zisc::isInBounds(uv[0], 0.0, cast<Float>(grid_res[0])) &&
      zisc::isInBounds(uv[1], 0.0, cast<Float>(grid_res[1]))) {
    zisc::Array<int, 2> uvi{cast<int>(uv[0]), cast<int>(uv[1])};
    const int cell_index = uvi[0] + grid_res[0] * uvi[1];
    ZISC_ASSERT(zisc::isInBounds(cell_index, 0, gridSize()),
                "The cell index is out of range.");

    for (uint i = 0; i < CoreConfig::spectraSize(); ++i) {
      const uint16 lambda = CoreConfig::shortestWavelength() +
                            cast<uint16>(i) * CoreConfig::wavelengthResolution();
      Float spectrum = toSpectrum(lambda, cell_index, uv);
      // Now we have a spectrum which corresponds to the xy chromaticities of the input.
      // Need to scale according to the input brightness X+Y+Z now
      spectrum = spectrum * (xyz.x() + xyz.y() + xyz.z());
      // The division by equal energy reflectance is here to make sure
      // that xyz = (1, 1, 1) maps to a spectrum that is constant 1.
      spectrum = spectrum * spectral_transport::kInvEqualEnergyReflectance;

      spectra[i] = spectrum;
    }
  }
  return spectra;
}

/*!
  */
inline
zisc::ArithmeticArray<Float, 3> SpectralTransport::hom(
    const zisc::ArithmeticArray<Float, 2>& x) noexcept
{
  return zisc::ArithmeticArray<Float, 3>{x[0], x[1], 1.0};
}

/*!
  */
inline
zisc::ArithmeticArray<Float, 2> SpectralTransport::dehom(
    const zisc::ArithmeticArray<Float, 3>& x) noexcept
{
  return zisc::ArithmeticArray<Float, 2>{x[0] / x[2], x[1] / x[2]};
}

/*!
  */
Float SpectralTransport::toSpectrum(const uint16 lambda,
                                    const int cell_index,
                                    const UvColor& uv) noexcept
{
  using zisc::cast;
  const auto& cell = spectral_transport::kGrid[cell_index];
  // Get Linearly interpolated spectral power for the corner vertices.
  std::vector<Float> p;
  p.resize(cell.num_points_, 0.0);
  // This clamping is only necessary if lambda is not sure to be [min, max].
  constexpr int spectra_size = cast<int>(CoreConfig::spectraSize());
  const Float sb = 
      cast<Float>(lambda - CoreConfig::shortestWavelength()) /
      cast<Float>(CoreConfig::longestWavelength()-CoreConfig::shortestWavelength()) *
      cast<Float>(spectra_size - 1);
  const int sb0 = cast<int>(sb);
  const int sb1 = (sb0 + 1 < spectra_size) ? sb0 + 1 : sb0;
  ZISC_ASSERT(zisc::isInBounds(sb0, 0, spectra_size), "The sb0 is out of range.");
  ZISC_ASSERT(zisc::isInBounds(sb1, 0, spectra_size), "The sb1 is out of range.");
  const Float sbf = sb - cast<Float>(sb0);
  for (int i = 0; i < cast<int>(p.size()); ++i) {
    ZISC_ASSERT(0 <= cell.indices_[i], "The index is minus.");
    const auto& point = spectral_transport::kDataPoints[cell.indices_[i]];
    p[i] = (1.0 - sbf) * point.spectra_[sb0] + sbf * point.spectra_[sb1];
  }

  Float interpolated_p = 0.0;
  if (cell.inside_) {
    // fast path for normal inner quads
    const UvColor uvf{uv[0] - std::floor(uv[0]), uv[1] - std::floor(uv[1])};
    ZISC_ASSERT(zisc::isInBounds(uvf[0], 0.0, 1.0), "The uvf[0] is out of range.");
    ZISC_ASSERT(zisc::isInBounds(uvf[1], 0.0, 1.0), "The uvf[1] is out of range.");
    // The layout of the vertices in the quad is:
    // 2 3
    // 0 1
    interpolated_p =
        p[0] * (1.0 - uvf[0]) * (1.0 - uvf[1]) +
        p[2] * (1.0 - uvf[0]) * uvf[1] +
        p[3] * uvf[0] * uvf[1] +
        p[1] * uvf[0] * (1.0 - uvf[1]);
  }
  else {
    // Need to go through triangulation
    // We get the indices in such an order that they form a triangle fan around idx[0]
    // Compute barycentric coordinates of our xy* point for all triangles in the fan:
    const auto& point0 = spectral_transport::kDataPoints[cell.indices_[0]];
    const auto& point1 = spectral_transport::kDataPoints[cell.indices_[1]];
    const auto e = uv.data() - point0.uv_.data();
    auto e0 = point1.uv_.data() - point0.uv_.data();
    Float uu = e0[0] * e[1] - e[0] * e0[1];
    for (int i = 0; i < cast<int>(cell.num_points_ - 1); ++i) {
      zisc::ArithmeticArray<Float, 2> e1{0.0, 0.0};
      if (i == cast<int>(cell.num_points_ - 2)) // Close the circle
        e1 = point1.uv_.data() - point0.uv_.data();
      else
        e1 = spectral_transport::kDataPoints[cell.indices_[i + 2]].uv_.data() - point0.uv_.data();
      const Float vv = e[0] * e1[1] - e1[0] * e[1];
      //! \todo with some sign magic, this division could be deferred to the last iteration!
      const Float area = e0[0] * e1[1] - e1[0] * e0[1];
      // normalize
      const Float u = uu / area;
      const Float v = vv / area;
      const Float w = 1.0 - (u + v);
      if ((u < 0.0) || (v < 0.0) || (w < 0.0)) {
        uu = -vv;
        e0 = e1;
        continue;
      }
      // This seems to be triangle we've been looking for .
      interpolated_p = p[0] * w +
                       p[i + 1] * v +
                       p[(i == cast<int>(cell.num_points_ - 2)) ? 1 : (i+2)] * u;
      break;
    }
  }
  return interpolated_p;
}

/*!
  */
inline
auto SpectralTransport::toUv(const YxyColor& yxy) noexcept -> UvColor
{
  zisc::Matrix<Float, 3, 1> v3{yxy.x(),
                               yxy.y(),
                                   1.0};
  const auto v2 = spectral_transport::kToUvFromXy * v3;
  return UvColor{v2(0, 0), v2(1, 0)};
}

/*!
  */
inline
auto SpectralTransport::toXystar(const YxyColor& yxy) noexcept -> XystarColor
{
  zisc::Matrix<Float, 3, 1> v3{yxy.x(),
                               yxy.y(),
                                   1.0};
  const auto v2 = spectral_transport::kToXystarFromXy * v3;
  return XystarColor{v2(0, 0), v2(1, 0)};
}

} // namespace nanairo
