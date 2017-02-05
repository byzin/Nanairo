/*!
  \file spectral_distribution.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectral_distribution.hpp"
// Standard C++ library
#include <cmath>
#include <memory>
#include <utility>
// Qt
#include <QColor>
#include <QFile>
#include <QJsonObject>
#include <QString>
#include <QTextStream>
// Zisc
#include "zisc/arithmetic_array.hpp"
#include "zisc/csv.hpp"
#include "zisc/linear_interp.hpp"
#include "zisc/math.hpp"
#include "zisc/matrix.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "color_conversion.hpp"
#include "color_space.hpp"
#include "rgb_color.hpp"
#include "spectral_transport.hpp"
#include "xyz_color.hpp"
#include "xyz_color_matching_function.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
void SpectralDistribution::clampAll(const Float min, const Float max) noexcept
{
  distribution_.clampAll(min, max);
}

/*!
  \details
  No detailed.
  */
void SpectralDistribution::correctGamma(const Float gamma) noexcept
{
  for (uint index = 0; index < size(); ++index)
    distribution_[index] = zisc::pow(distribution_[index], gamma);
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::makeEmissive(
    const System& system,
    const QJsonObject& settings) noexcept
{
  auto distribution = makeColor<ColorType::kEmissive>(system, settings);
  distribution.clampAll(0.0, distribution.max());
  distribution = distribution.normalized();
  return distribution;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::makeReflective(
    const System& system,
    const QJsonObject& settings) noexcept
{
  auto distribution = makeColor<ColorType::kReflective>(system, settings);
  distribution.clampAll(0.0, 1.0);
  return distribution;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::makeSpectra(
    const QJsonObject& settings) noexcept
{
  const auto file_path = SceneValue::toString(settings, keyword::value);
  return makeSpectra(file_path);
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::makeSpectra(
    const QString& file_path) noexcept
{
  auto spectra_data = loadSpectraData(file_path);
  SpectralDistribution spectra;
  for (uint index = 0; index < CoreConfig::spectraSize(); ++index) {
    const Float lambda = zisc::cast<Float>(getWavelength(index));
    spectra.set(index, spectra_data(lambda));
  }
  return spectra;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::toRgbSpectra(const RgbColor& rgb) noexcept
{
  SpectralDistribution rgb_spectra;
  rgb_spectra.setByWavelength(CoreConfig::blueWavelength(), rgb.blue());
  rgb_spectra.setByWavelength(CoreConfig::greenWavelength(), rgb.green());
  rgb_spectra.setByWavelength(CoreConfig::redWavelength(), rgb.red());
  return rgb_spectra;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution SpectralDistribution::toSpectra(const System& system,
                                                     const RgbColor& color) noexcept
{
  using zisc::cast;
  const auto to_xyz = getRgbToXyzMatrix(system.colorSpace());
  const auto xyz = ColorConversion::toXyz(color, to_xyz);
  return SpectralTransport::toSpectra(xyz);
}

/*!
  */
XyzColor SpectralDistribution::toXyzForEmitter(const System& system) const noexcept
{
  return toXyz<ColorType::kEmissive>(system);
}

/*!
  */
XyzColor SpectralDistribution::toXyzForReflector(const System& system) const noexcept
{
  return toXyz<ColorType::kReflective>(system);
}

/*!
  \details
  No detailed.
  */
inline
bool SpectralDistribution::isRgbData(const QJsonObject& settings) noexcept
{
  return SceneValue::toString(settings, keyword::colorMode) == keyword::rgb;
}

/*!
  \details
  No detailed.
  */
zisc::LinearInterp<Float> SpectralDistribution::loadSpectraData(
    const QString& file_path) noexcept
{
  // Open distribution file
  QFile csv_file{file_path};
  csv_file.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream csv_text{&csv_file};

  // Remove header
  csv_text.readLine();
  csv_text.readLine();

  // Load distribution
  zisc::Csv<Float, Float> csv;
  for (auto line = csv_text.readLine(); !line.isNull(); line = csv_text.readLine())
    csv.append(line.toStdString());

  // Set distribution
  zisc::LinearInterp<Float> spectra_data;
  for (uint i = 0; i < csv.rowSize(); ++i)
    spectra_data.add(csv.get<0>(i), csv.get<1>(i));
  return spectra_data;
}

/*!
  */
template <SpectralDistribution::ColorType type>
SpectralDistribution SpectralDistribution::makeColor(
    const System& system,
    const QJsonObject& settings) noexcept
{
  SpectralDistribution distribution;
  // Make from RGB data
  if (isRgbData(settings)) {
    const auto rgb = makeRgb(system, settings);
    distribution = (system.isRgbRenderingMode())
        ? toRgbSpectra(rgb)
        : toSpectra(system, rgb);
  }
  // Make from spectra data
  else {
    const auto spectra = makeSpectra(settings);
    distribution = (system.isRgbRenderingMode())
        ? toRgbSpectra<type>(system, spectra)
        : spectra;
  }
  return distribution;
}

/*!
  \details
  No detailed.
  */
inline
RgbColor SpectralDistribution::makeRgb(const System& system,
                                       const QJsonObject& settings) noexcept
{
  const auto color = SceneValue::toArray(settings, keyword::value);
  auto rgb = RgbColor{SceneValue::toFloat<Float>(color[0]),
                      SceneValue::toFloat<Float>(color[1]),
                      SceneValue::toFloat<Float>(color[2])};
  rgb.correctGamma(system.gamma());
  rgb.clampAll(0.0, 1.0);
  return rgb;
}

/*!
  \details
  No detailed.
  */
template <SpectralDistribution::ColorType type>
SpectralDistribution SpectralDistribution::toRgbSpectra(
    const System& system,
    const SpectralDistribution& spectra) noexcept
{
  const auto xyz = toXyz<type>(system, spectra);
  auto rgb = ColorConversion::toRgb(xyz, getXyzToRgbMatrix(system.colorSpace()));
  rgb.clampAll(0.0, rgb.max());
  return toRgbSpectra(rgb);
}

/*!
  \details
  No detailed.
  */
template <SpectralDistribution::ColorType type>
XyzColor SpectralDistribution::toXyz(
    const System& system,
    const SpectralDistribution& spectra) noexcept
{
  const auto& cmf = system.xyzColorMatchingFunction();
  const auto xyz = (type == ColorType::kEmissive)
      ? cmf.toXyzForEmitter(spectra)
      : cmf.toXyzForReflector(spectra);
  return xyz;
}

/*!
  \details
  No detailed.
  */
template <SpectralDistribution::ColorType type>
XyzColor SpectralDistribution::toXyz(const System& system) const noexcept
{
  XyzColor xyz;
  if (system.isRgbRenderingMode()) {
    const RgbColor rgb{getByWavelength(CoreConfig::redWavelength()),
                       getByWavelength(CoreConfig::greenWavelength()),
                       getByWavelength(CoreConfig::blueWavelength())};
    xyz = ColorConversion::toXyz(rgb, getRgbToXyzMatrix(system.colorSpace()));
  }
  else {
    xyz = toXyz<type>(system, *this);
  }
  return xyz;
}

} // namespace nanairo
