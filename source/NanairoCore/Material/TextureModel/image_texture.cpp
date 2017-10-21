/*!
  \file image_texture.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "image_texture.hpp"
// Standard C++ library
#include <cmath>
#include <cstddef>
#include <iterator>
#include <limits>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_conversion.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/Color/rgb_color.hpp"
#include "NanairoCore/Color/rgba_32.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/texture_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ImageTexture::ImageTexture(const System& system,
                           const SettingNodeBase* settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
SampledSpectra ImageTexture::emissiveValue(
    const Point2& uv,
    const WavelengthSamples& wavelengths) const noexcept
{
  const uint index = getColorIndex(uv);
  return sample(emissive_value_table_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
Float ImageTexture::grayScaleValue(const Point2& uv) const noexcept
{
  const uint index = getColorIndex(uv);
  return gray_scale_table_[index];
}

/*!
  \details
  No detailed.
  */
Float ImageTexture::reflectiveValue(const Point2& uv,
                                    const uint16 wavelength) const noexcept
{
  const uint index = getColorIndex(uv);
  return reflective_value_table_[index].getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra ImageTexture::reflectiveValue(
    const Point2& uv,
    const WavelengthSamples& wavelengths) const noexcept
{
  const uint index = getColorIndex(uv);
  return sample(reflective_value_table_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
Float ImageTexture::spectraValue(const Point2& uv,
                                 const uint16 wavelength) const noexcept
{
  const uint index = getColorIndex(uv);
  return reflective_value_table_[index].getByWavelength(wavelength);
}

/*!
  \details
  No detailed.
  */
SampledSpectra ImageTexture::spectraValue(
    const Point2& uv,
    const WavelengthSamples& wavelengths) const noexcept
{
  const uint index = getColorIndex(uv);
  return sample(reflective_value_table_[index], wavelengths);
}

/*!
  \details
  No detailed.
  */
TextureType ImageTexture::type() const noexcept
{
  return TextureType::kImage;
}

/*!
  */
inline
uint ImageTexture::getColorIndex(const Point2& uv) const noexcept
{
  const uint pixel_index = getPixelIndex(uv);
  const uint index = color_index_table_[pixel_index];
  return index;
}

/*!
  */
inline
uint ImageTexture::getPixelIndex(const Point2& uv) const noexcept
{
  const auto x = zisc::cast<uint>(uv[0] * zisc::cast<Float>(resolution_[0]));
  const auto y = zisc::cast<uint>((1.0 - uv[1]) * zisc::cast<Float>(resolution_[1]));
  return x + y * resolution_[0];
}

/*!
  \details
  No detailed.
  */
void ImageTexture::initialize(const System& system,
                              const SettingNodeBase* settings) noexcept
{
  const auto texture_settings = castNode<TextureSettingNode>(settings);

  const auto& parameters = texture_settings->imageTextureParameters();
  {
    resolution_ = parameters.image_.resolution();
  }
  initializeTables(system, parameters.image_);
}

/*!
  \details
  No detailed.
  */
void ImageTexture::initializeTables(const System& system, const LdrImage& image)
    noexcept
{
  uint table_size = resolution_[0] * resolution_[1];

  // Make a color table
  auto color_table = image.data();
  std::sort(color_table.begin(), color_table.end());
  auto table_end = std::unique(color_table.begin(), color_table.end());
  zisc::toBinaryTree(color_table.begin(), table_end);

  // Make a color index table
  {
    color_index_table_.resize(table_size);
    auto table_begin = color_table.begin();
    for (uint index = 0; index < table_size; ++index) {
      auto position = zisc::searchBinaryTree(table_begin, table_end, image[index]);
      const auto color_index = std::distance(table_begin, position);
      color_index_table_[index] = zisc::cast<uint>(color_index);
    }
  }

  // Make a value tables
  table_size = zisc::cast<uint>(std::distance(color_table.begin(), table_end));
  emissive_value_table_.resize(table_size);
  reflective_value_table_.resize(table_size);
  gray_scale_table_.resize(table_size);
  const auto to_xyz_matrix = getRgbToXyzMatrix(system.colorSpace());
  for (uint index = 0; index < table_size; ++index) {
    auto rgb = ColorConversion::toFloatRgb(color_table[index]);
    rgb.correctGamma(system.gamma());
    rgb.clampAll(std::numeric_limits<Float>::epsilon(), rgb.max());
    // Float value
    {
      const Float y = ColorConversion::toXyz(rgb, to_xyz_matrix).y();
      gray_scale_table_[index] = zisc::clamp(y, 0.0, 1.0);
    }
    // Spectra values
    {
      SpectralDistribution value;
      value.setByWavelength(CoreConfig::blueWavelength(), rgb.blue());
      value.setByWavelength(CoreConfig::greenWavelength(), rgb.green());
      value.setByWavelength(CoreConfig::redWavelength(), rgb.red());
      value = value.computeSystemColor(system);
      emissive_value_table_[index] = value.toEmissiveColor();
      reflective_value_table_[index] = value.toReflectiveColor();
    }
  }
}

} // namespace nanairo
