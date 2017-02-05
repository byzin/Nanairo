/*!
  \file tone_mapping_operator.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "tone_mapping_operator.hpp"
// Standard C++ library
#include <vector>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/matrix.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "filmic.hpp"
#include "reinhard.hpp"
#include "uncharted2_filmic.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_conversion.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/yxy_color.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ToneMappingOperator::ToneMappingOperator(const System& system,
                                         const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
ToneMappingOperator::~ToneMappingOperator() noexcept
{
}

/*!
  \details
  No detailed.
  */
void ToneMappingOperator::map(System& system,
                              const HdrImage& hdr_image,
                              QImage* ldr_image) const noexcept
{
  using zisc::cast;

  auto pixel_array = getPixelArray(ldr_image);
  auto map_luminance = [this, &system, &hdr_image, pixel_array](const int thread_id)
  {
    // Set the calculation range
    const auto range = system.calcThreadRange(hdr_image.numOfPixels(), thread_id);
    const auto begin = std::get<0>(range);
    const auto end = std::get<1>(range);
    // Apply tonemap to each pixel
    for (uint index = begin; index < end; ++index) {
      // Set black if the radiance isn't positive
      if (0.0 < hdr_image[index].y()) {
        // Tone mapping
        auto yxy = ColorConversion::toYxy(hdr_image[index]);
        const Float l = tonemap(exposure() * yxy.Y());
        yxy.Y() = zisc::clamp(l, 0.0, 1.0);
        const auto xyz = ColorConversion::toXyz(yxy);
        // Convert XYZ to RGB
        auto rgb = ColorConversion::toRgb(xyz, getXyzToRgbMatrix(system.colorSpace()));
        rgb.clampAll(0.0, 1.0);
        rgb.correctGamma(inverseGamma());
        // Set RGB to the pixel
        pixel_array[index] = ColorConversion::toQRgb(rgb);
      }
      else {
        const QColor background_color{Qt::black};
        pixel_array[index] = background_color.rgb();
      }
    }
  };

  auto& thread_pool = system.threadPool();
  constexpr uint begin = 0;
  const uint end = thread_pool.numOfThreads();
  auto result = thread_pool.enqueueLoop(map_luminance, begin, end);
  result.get();
}

/*!
  \details
  No detailed.
  */
void ToneMappingOperator::initialize(const System& system,
                                     const QJsonObject& settings) noexcept
{
  // Gamma
  inverse_gamma_ = zisc::invert(system.gamma());
  // Exposure
  exposure_ = SceneValue::toFloat<Float>(settings, keyword::exposure);
}

/*!
  \details
  No detailed.
  */
UniquePointer<ToneMappingOperator> ToneMappingOperator::makeOperator(
    const System& system,
    const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  ToneMappingOperator* method = nullptr;

  const auto type = SceneValue::toString(settings, keyword::toneMapping);
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::reinhard): {
    method = new Reinhard{system, settings};
    break;
   }
   case toHash32(keyword::filmic): {
    method = new Filmic{system, settings};
    break;
   }
   case toHash32(keyword::uncharted2Filmic): {
    method = new Uncharted2Filmic{system, settings};
    break;
   }
   default: {
    zisc::raiseError("ToneMappingError: Unsupprted type is specified.");
   }
  }
  return UniquePointer<ToneMappingOperator>{method};
}

} // namespace nanairo
