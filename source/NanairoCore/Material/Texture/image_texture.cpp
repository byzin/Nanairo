/*!
  \file image_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Color/rgb_color.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ImageTexture::ImageTexture(const System& system,
                           const SceneSettings& settings, 
                           const QString& prefix) noexcept
{
  initialize(system, settings, prefix);
}

/*!
  \details
  No detailed.
  */
Float ImageTexture::floatValue(const Point2& coordinate) const noexcept
{
  using zisc::cast;

  const auto x = cast<uint>(coordinate[0] * width_);
  const auto y = cast<uint>(coordinate[1] * height_);
  const uint index = width_resolution_ * y + x;
  return float_table_[color_index_[index]];
}

/*!
  \details
  No detailed.
  */
std::size_t ImageTexture::textureSize() const noexcept
{
  return sizeof(SpectralDistribution) * spectra_table_.size() +
         sizeof(Float) * float_table_.size() +
         sizeof(uint) * color_index_.size() +
         sizeof(Float) * 2 +
         sizeof(uint);
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
Float ImageTexture::wavelengthValue(const Point2& coordinate, 
                                    const uint16 wavelength) const noexcept
{
  using zisc::cast;

  const auto x = cast<uint>(coordinate[0] * width_);
  const auto y = cast<uint>(coordinate[1] * height_);
  const uint index = width_resolution_ * y + x;
  return spectra_table_[color_index_[index]].getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
void ImageTexture::initialize(const System& system,
                              const SceneSettings& settings, 
                              const QString& prefix) noexcept
{
  using zisc::cast;

  auto p = prefix + "/" + keyword::imageTexture;

  auto key = p + "/" + keyword::imageFilePath;
  const auto file_path = settings.stringValue(key);
  const QImage image{file_path};

  const int width = image.width();
  const int height = image.height();
  width_resolution_ = cast<uint>(width);
  width_ = cast<Float>(width);
  height_ = cast<Float>(height);

  setColor(system, image, system.gamma());
}

/*!
  \details
  No detailed.
  */
void ImageTexture::setColor(const System& system,
                            const QImage& image, 
                            const Float gamma) noexcept
{
  using zisc::cast;

  const int width = image.width();
  const int height = image.height();
  uint size = cast<uint>(width * height);

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

  size = cast<uint>(std::distance(color_table.begin(), table_end));
  float_table_.resize(size);
  spectra_table_.resize(size);
  const auto to_xyz_matrix = getRgbToXyzMatrix(system.colorSpace());
  for (uint i = 0; i < size; ++i) {
    auto rgb = RgbColor{QColor{color_table[i]}};
    rgb.correctGamma(gamma);
    const Float y = rgb.toXyz(to_xyz_matrix).y();
    float_table_[i] = zisc::clamp(y, 0.0, 1.0);
    spectra_table_[i] = system.isRgbRenderingMode()
        ? toRgbSpectra(rgb)
        : toSpectra(system, rgb);
    ZISC_ASSERT(isBetweenZeroAndOneFloat(spectra_table_[i]),
                "Texture value must be [0, 1].");
  }
}

} // namespace nanairo
