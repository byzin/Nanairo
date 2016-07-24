/*!
  \file uncharted2_filmic.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "uncharted2_filmic.hpp"
// Standard C++ library
#include <functional>
#include <future>
#include <vector>
#include <utility>
// Qt
#include <QImage>
#include <QJsonObject>
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
Uncharted2Filmic::Uncharted2Filmic(const System& system, 
                                   const QJsonObject& settings) noexcept :
    ToneMappingMethod(system, settings)
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void Uncharted2Filmic::toneMap(System& system,
                               const HdrImage& hdr_image, 
                               QImage& ldr_image) noexcept
{
  using zisc::cast;

  setPixelLuminance(system, hdr_image, pixel_luminance_);
  const auto white_point = getWhitePoint(pixel_luminance_);
  const auto k = 1.0 / uncharted2ToneMap(white_point.Y());

  auto pixel_array = getPixelArray(ldr_image);
  auto tone_map_image = [this, &ldr_image, pixel_array, k](const int y)
  {
    constexpr Float exposure = 2.0;
    uint index = cast<uint>(y * ldr_image.width());
    for (int x = 0; x < ldr_image.width(); ++x) {
      auto& yxy = pixel_luminance_[index];
      // Set black if the radiance is zero
      if (yxy.Y() != 0.0) {
        // Apply uncharted2_filmic method
        yxy.Y() = k * uncharted2ToneMap(yxy.Y() * exposure);
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
void Uncharted2Filmic::initialize(const System& system, 
                                  const QJsonObject& /* settings */) noexcept
{
  const int width = system.imageWidthResolution();
  const int height = system.imageHeightResolution();
  pixel_luminance_.resize(width * height);
}

/*!
  \details
  No detailed.
  */
inline
Float Uncharted2Filmic::uncharted2ToneMap(const Float x) const noexcept
{
  constexpr Float A = 0.15;
  constexpr Float B = 0.50;
  constexpr Float C = 0.10;
  constexpr Float D = 0.20;
  constexpr Float E = 0.02;
  constexpr Float F = 0.30;

  return ((x * (A * x + C * B ) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

} // namespace nanairo
