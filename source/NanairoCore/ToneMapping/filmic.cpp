/*!
  \file filmic.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "filmic.hpp"
// Standard C++ library
#include <functional>
#include <future>
#include <vector>
#include <utility>
// Qt
#include <QString>
#include <QImage>
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

namespace nanairo {

/*!
  \details
  No detailed.
  */
Filmic::Filmic(const System& system, const SceneSettings& settings) noexcept :
    ToneMappingMethod(system, settings)
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void Filmic::toneMap(System& system,
                     const HdrImage& hdr_image, 
                     QImage& ldr_image) noexcept
{
  using zisc::cast;

  auto pixel_array = getPixelArray(ldr_image);
  std::function<void (const int)> tone_map_image{
  [this, &hdr_image, &ldr_image, pixel_array](const int y)
  {
    uint index = cast<uint>(y * ldr_image.width());
    for (int x = 0; x < ldr_image.width(); ++x) {
      // Set black if the radiance is zero
      if (hdr_image[index].y() != 0.0) {
        // Apply filmic method
        auto yxy = hdr_image[index].toYxy();
        yxy.Y() = filmicToneMap(yxy.Y());
        yxy.Y() = zisc::clamp(yxy.Y(), 0.0, 1.0);
        const auto xyz = yxy.toXyz();
        auto rgb = xyz.toRgb(toRgbMatrix());
  
        if (1.0 < rgb.max())
          rgb.scale();
        rgb.clamp(0.0, 1.0);
        pixel_array[index] = rgb.toQRgb();
      }
      else {
        const QColor background_color{Qt::black};
        pixel_array[index] = background_color.rgb();
      }
      ++index;
    }
  }};

  auto& thread_pool = system.threadPool();
  auto result = thread_pool.loop(std::move(tone_map_image), 0, ldr_image.height());
  result.get();
}

/*!
  \details
  No detailed.
  */
void Filmic::initialize(const System& system, const SceneSettings& /* settings */) noexcept
{
  const int width = system.imageWidthResolution();
  const int height = system.imageHeightResolution();
  pixel_luminance_.resize(width * height);
}

/*!
  \details
  No detailed.
  */
Float Filmic::filmicToneMap(Float x) const noexcept
{
  constexpr Float A = 6.2;
  constexpr Float B = 0.5;
  constexpr Float C = 1.7;
  constexpr Float D = 0.06;

  x = zisc::max(0.0, x - 0.004);
  return (x * (A * x + B)) / (x * (A * x + C) + D);
}

} // namespace nanairo
