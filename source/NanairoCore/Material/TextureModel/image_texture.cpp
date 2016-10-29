/*!
  \file image_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "image_texture.hpp"
// Standard C++ library
#include <cmath>
#include <cstddef>
#include <iterator>
#include <vector>
// Qt
#include <QColor>
#include <QImage>
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Color/rgb_color.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ImageTexture::ImageTexture(const System& system, const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
Float ImageTexture::floatValue(const Point2& coordinate) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = width_resolution_ * y + x;
  return float_table_[color_index_[index]];
}

/*!
  \details
  No detailed.
  */
Float ImageTexture::reflectiveValue(const Point2& coordinate,
                                    const uint16 wavelength) const noexcept
{
  const auto x = zisc::cast<uint>(coordinate[0] * width_);
  const auto y = zisc::cast<uint>(coordinate[1] * height_);
  const uint index = width_resolution_ * y + x;
  return reflective_value_table_[color_index_[index]].getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
TextureType ImageTexture::type() const noexcept
{
  return TextureType::Image;
}

/*!
  \details
  No detailed.
  */
void ImageTexture::initialize(const System& system,
                              const QJsonObject& settings) noexcept
{
  using zisc::cast;
  // Initialize image resolution
  const auto image_file_path = SceneValue::toString(settings,
                                                    keyword::imageFilePath);
  const QImage image{image_file_path};
  const int width = image.width();
  const int height = image.height();
  width_resolution_ = cast<uint>(width);
  width_ = cast<Float>(width);
  height_ = cast<Float>(height);
  // Initialize reflective value
  initializeReflectiveValueTable(system, image, system.gamma());
  // Initialize emissive value
  initializeEmissiveValueTable();
}

/*!
  */
void ImageTexture::initializeEmissiveValueTable() noexcept
{
  // Calc the average energy of the texture
  Float energy = 0.0;
  {
    zisc::CompensatedSummation<Float> e;
    for (const auto index : color_index_) {
      const auto& spectra = reflective_value_table_[index];
      e.add(spectra.sum());
    }
    energy = e.get() / zisc::cast<Float>(color_index_.size());
  }
  // Scale by the average energy
  const uint table_size = zisc::cast<uint>(reflective_value_table_.size());
  emissive_value_table_.resize(table_size);
  for (uint i = 0; i < table_size; ++i)
    emissive_value_table_[i] = reflective_value_table_[i] * energy;
}

/*!
  \details
  No detailed.
  */
void ImageTexture::initializeReflectiveValueTable(const System& system,
                                                  const QImage& image,
                                                  const Float gamma) noexcept
{
  using zisc::cast;

  const int width = image.width();
  const int height = image.height();
  uint size = cast<uint>(width * height);
  // Make a color table
  std::vector<QRgb> color_table;
  color_table.resize(size);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const uint index = y * width + x;
      color_table[index] = image.pixel(x, y);
    }
  }
  std::sort(color_table.begin(), color_table.end());
  auto table_end = std::unique(color_table.begin(), color_table.end());
  zisc::toBinaryTree(color_table.begin(), table_end);
  // Make a color index table
  auto get_color_index = [&color_table, &table_end](const QRgb color)
  {
    auto position = zisc::searchBinaryTree(color_table.begin(), table_end, color);
    return cast<uint>(std::distance(color_table.begin(), position));
  };
  color_index_.resize(size);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const int index = width * y + x;
      color_index_[index] = get_color_index(image.pixel(x, y));
    }
  }
  // Make a reflective value table
  size = cast<uint>(std::distance(color_table.begin(), table_end));
  float_table_.resize(size);
  reflective_value_table_.resize(size);
  const auto to_xyz_matrix = getRgbToXyzMatrix(system.colorSpace());
  for (uint i = 0; i < size; ++i) {
    auto rgb = RgbColor{QColor{color_table[i]}};
    rgb.correctGamma(gamma);
    const Float y = rgb.toXyz(to_xyz_matrix).y();
    float_table_[i] = zisc::clamp(y, 0.0, 1.0);
    reflective_value_table_[i] = system.isRgbRenderingMode()
        ? SpectralDistribution::toRgbSpectra(rgb)
        : SpectralDistribution::toSpectra(system, rgb);
    ZISC_ASSERT(reflective_value_table_[i].isAllInBounds(0.0, 1.0),
                "Texture value must be [0, 1].");
  }
}

} // namespace nanairo
