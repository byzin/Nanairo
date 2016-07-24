/*!
  \file modified_reinhard.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "modified_reinhard.hpp"
// Standard C++ library
#include <functional>
#include <future>
#include <vector>
#include <utility>
// Qt
#include <QJsonObject>
#include <QImage>
#include <QString>
// Zisc
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "tone_mapping_method.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/rgb_color.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/xyz_color.hpp"
#include "NanairoCore/Color/yxy_color.hpp"
#include "NanairoCore/Utility/floating_point.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ModifiedReinhard::ModifiedReinhard(const System& system,
                                   const QJsonObject& settings) noexcept :
    ToneMappingMethod(system, settings)
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void ModifiedReinhard::toneMap(System& system,
                               const HdrImage& hdr_image, 
                               QImage& ldr_image) noexcept
{
  using zisc::cast;

  setPixelLuminance(system, hdr_image, pixel_luminance_);
  const auto white_point = getWhitePoint(pixel_luminance_);
  const auto k = 1.0 / (white_point.Y() * white_point.Y());

  auto pixel_array = getPixelArray(ldr_image);
  auto tone_map_image = [this, &ldr_image, pixel_array, k](const int y)
  {
    uint index = cast<uint>(y * ldr_image.width());
    for (int x = 0; x < ldr_image.width(); ++x) {
      auto& yxy = pixel_luminance_[index];
      // Set black if the radiance is zero
      if (yxy.Y() != 0.0) {
        // Apply modified_reinhard method
        yxy.Y() = (yxy.Y() / (1.0 + yxy.Y())) * (1.0 + yxy.Y() * k);
        yxy.Y() = zisc::clamp(yxy.Y(), 0.0, 1.0);
        const auto xyz = yxy.toXyz();
        auto rgb = xyz.toRgb(toRgbMatrix());
  
        if (1.0 < rgb.max())
          rgb.scale();
        rgb.clamp(0.0, 1.0);
        rgb.correctGamma(inverseGamma());
        pixel_array[index] = rgb.toQRgb();
      }
      else {
        const QColor background_color{Qt::black};
        pixel_array[index] = background_color.rgb();
      }
      ++index;
    }
  };

  auto& thread_pool = system.threadPool();
  auto result = thread_pool.enqueueLoop(tone_map_image, 0, ldr_image.height());
  result.get();
}

/*!
  \details
  No detailed.
  */
void ModifiedReinhard::initialize(const System& system, 
                                  const QJsonObject& /* settings */) noexcept
{
  const int width = system.imageWidthResolution(); 
  const int height = system.imageHeightResolution();
  pixel_luminance_.resize(width * height);
}

} // namespace nanairo
