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
#include "color_space.hpp"
#include "rgb_color.hpp"
#include "rgb_color_matching_function.hpp"
#include "xyz_color.hpp"
#include "xyz_color_matching_function.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Utility/floating_point.hpp"
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

//! Check if the data is RGB
bool isRgbData(const QJsonObject& settings) noexcept;

//! Load spectra data from a spectra file
zisc::LinearInterp<Float> loadSpectraData(const QString& file_path) noexcept;

//! Make a RGB
RgbColor makeRgb(const System& system, const QJsonObject& settings) noexcept;

//! Convert emissive spectra to RGB spectra
SpectralDistribution toEmissiveRgbSpectra(const System& system,
                                          const SpectralDistribution& spectra) noexcept;

//! Convert nanairoive spectra to RGB spectra
SpectralDistribution toReflectiveRgbSpectra(const System& system,
                                            const SpectralDistribution& spectra) noexcept;

/*!
  \details
  No detailed.
  */
void SpectralDistribution::clamp(const Float min, const Float max) noexcept
{
  distribution_.clamp(min, max);
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
XyzColor SpectralDistribution::toEmissiveXyz(const System& system) const noexcept
{
  if (system.isRgbRenderingMode()) {
    const RgbColor rgb{getByWavelength(CoreConfig::redWavelength()),
                       getByWavelength(CoreConfig::greenWavelength()),
                       getByWavelength(CoreConfig::blueWavelength())};
    return rgb.toXyz(getRgbToXyzMatrix(system.colorSpace()));
  }
  else {
    const auto& cmf = system.xyzColorMatchingFunction();
    return cmf.toXyzInEmissiveCase(*this);
  }
}

/*!
  \details
  No detailed.
  */
XyzColor SpectralDistribution::toReflectiveXyz(const System& system) const noexcept
{
  if (system.isRgbRenderingMode()) {
    const RgbColor rgb{getByWavelength(CoreConfig::redWavelength()),
                       getByWavelength(CoreConfig::greenWavelength()),
                       getByWavelength(CoreConfig::blueWavelength())};
    return rgb.toXyz(getRgbToXyzMatrix(system.colorSpace()));
  }
  else {
    const auto& cmf = system.xyzColorMatchingFunction();
    return cmf.toXyzInReflectiveCase(*this);
  }
}

/*!
  \details
  No detailed.
  */
bool isRgbData(const QJsonObject& settings) noexcept
{
  return SceneValue::toString(settings, keyword::colorMode) == keyword::rgb;
}

/*!
  \details
  No detailed.
  */
zisc::LinearInterp<Float> loadSpectraData(const QString& file_path) noexcept
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
  \details
  No detailed.
  */
std::unique_ptr<SpectralDistribution> makeEmissiveDistribution(
    const System& system,
    const QJsonObject& settings) noexcept
{
  auto distribution = std::make_unique<SpectralDistribution>();
  if (isRgbData(settings)) {
    const auto rgb = makeRgb(system, settings);
    *distribution = (system.isRgbRenderingMode())
        ? toRgbSpectra(rgb)
        : toSpectra(system, rgb);
  }
  else {
    auto spectra = makeEmissiveSpectra(settings);
    *distribution = (system.isRgbRenderingMode())
        ? toEmissiveRgbSpectra(system, spectra)
        : spectra;
  }
  *distribution = distribution->normalized();
  return distribution;
}

/*!
  \details
  No detailed.
  */
std::unique_ptr<SpectralDistribution> makeReflectiveDistribution(
    const System& system,
    const QJsonObject& settings) noexcept
{
  auto distribution = std::make_unique<SpectralDistribution>();
  if (isRgbData(settings)) {
    const auto rgb = makeRgb(system, settings);
    *distribution = (system.isRgbRenderingMode())
        ? toRgbSpectra(rgb)
        : toSpectra(system, rgb);
  }
  else {
    auto spectra = makeReflectiveSpectra(settings);
    *distribution = (system.isRgbRenderingMode())
        ? toReflectiveRgbSpectra(system, spectra)
        : spectra;
  }
  return distribution;
}

/*!
  \details
  No detailed.
  */
RgbColor makeRgb(const System& system, const QJsonObject& settings) noexcept
{
  const auto color = SceneValue::toArray(settings, keyword::value);
  auto rgb = RgbColor{SceneValue::toFloat<Float>(color[0]),
                      SceneValue::toFloat<Float>(color[1]),
                      SceneValue::toFloat<Float>(color[2])};
  rgb.correctGamma(system.gamma());
  rgb.clamp(0.0, 1.0);
  return rgb;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution makeEmissiveSpectra(const QJsonObject& settings) noexcept
{
  auto spectra = makeSpectra(settings);
  spectra.clamp(0.0, spectra.max());
  spectra = spectra.normalized();
  return spectra;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution makeReflectiveSpectra(const QJsonObject& settings) noexcept
{
  auto spectra = makeSpectra(settings);
  spectra.clamp(0.0, 1.0);
  return spectra;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution makeSpectra(const QJsonObject& settings) noexcept
{
  const auto file_path = SceneValue::toString(settings, keyword::value);
  return makeSpectra(file_path);
}

/*!
  \details
  No detailed.
  */
SpectralDistribution makeSpectra(const QString& file_path) noexcept
{
  using zisc::cast;

  auto spectra_data = loadSpectraData(file_path);
  SpectralDistribution spectra;
  for (uint index = 0; index < CoreConfig::spectraSize(); ++index) {
    const Float lambda = cast<Float>(getWavelength(index));
    spectra.set(index, spectra_data(lambda));
  }
  return spectra;
}

/*!
  \details
  No detailed.
  */
SpectralDistribution toEmissiveRgbSpectra(const System& system,
                                          const SpectralDistribution& spectra) noexcept
{
  const auto& cmf = system.xyzColorMatchingFunction();
  const auto xyz = cmf.toXyzInEmissiveCase(spectra);
  auto rgb = xyz.toRgb(getXyzToRgbMatrix(system.colorSpace()));
  rgb.clamp(0.0, 1.0);
  return toRgbSpectra(rgb);
}

/*!
  \details
  No detailed.
  */
SpectralDistribution toReflectiveRgbSpectra(const System& system,
                                            const SpectralDistribution& spectra) noexcept
{
  const auto& cmf = system.xyzColorMatchingFunction();
  const auto xyz = cmf.toXyzInReflectiveCase(spectra);
  auto rgb = xyz.toRgb(getXyzToRgbMatrix(system.colorSpace()));
  rgb.clamp(0.0, 1.0);
  return toRgbSpectra(rgb);
}

/*!
  \details
  No detailed.
  */
SpectralDistribution toRgbSpectra(const RgbColor& rgb) noexcept
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
SpectralDistribution toSpectra(const System& system, const RgbColor& color) noexcept
{
  using zisc::cast;

  constexpr Float sigma_min = 80.0;
  constexpr Float sigma_max = 150.0;
  const Float chi_rg = (color.red() + color.green() != 0.0)
      ? zisc::abs(color.red() - color.green()) / (color.red() + color.green())
      : 0.0;
  const Float chi_bg = (color.blue() + color.green() != 0.0)
      ? zisc::abs(color.blue() - color.green()) / (color.blue() + color.green())
      : 0.0;
  const Float sigma1 = chi_rg * sigma_min + (1.0 - chi_rg) * sigma_max,
              sigma2 = chi_bg * sigma_min + (1.0 - chi_bg) * sigma_max,
              sigma3 = zisc::min(sigma1, sigma2);
  const Float tmp1[] = {1.0 / (2.0 * sigma1 * sigma1),
                        1.0 / (2.0 * sigma2 * sigma2),
                        1.0 / (2.0 * sigma3 * sigma3)};
  const Float tmp2[] = {1.0 / zisc::sqrt(2.0 * zisc::kPi<Float> * sigma1),
                        1.0 / zisc::sqrt(2.0 * zisc::kPi<Float> * sigma2),
                        1.0 / zisc::sqrt(2.0 * zisc::kPi<Float> * sigma3)};

  const auto f = [&tmp1, &tmp2](const uint i, const uint16 lambda)
  {
//    constexpr uint16 rgb[] = {641, 508, 426};
    constexpr uint16 rgb[] = {CoreConfig::redWavelength(),
                              CoreConfig::greenWavelength(),
                              CoreConfig::blueWavelength()};
    const int diff_lambda = cast<int>(lambda - rgb[i]);
    return zisc::exp(-cast<Float>(diff_lambda * diff_lambda) * tmp1[i]) * tmp2[i];
  };

  SpectralDistribution f_bar[3];
  for (uint i = 0; i < 3; ++i) {
    for (uint index = 0; index < CoreConfig::spectraSize(); ++index) {
      f_bar[i].set(index, f(i, getWavelength(index)));
    }
  }

  constexpr Float k = cast<Float>(CoreConfig::wavelengthResolution()) / 19.15;
  const auto& cmf = system.rgbColorMatchingFunction();
  const SpectralDistribution* bar[] = {&cmf.redBar(), &cmf.greenBar(), &cmf.blueBar()};
  zisc::Matrix<Float, 3, 3> t;
  for (uint i = 0; i < 3; ++i) {
    for (uint j = 0; j < 3; ++j) {
      t(i, j) = k * (*bar[i] * f_bar[j]).sum();
    }
  }
  const auto inverse_t = t.inverseMatrix();
  const auto x = inverse_t * color.data();


  auto spectra = f_bar[0] * x[0] + f_bar[1] * x[1] + f_bar[2] * x[2];
  spectra.clamp(0.0, 1.0);

  return spectra;
}

} // namespace nanairo
