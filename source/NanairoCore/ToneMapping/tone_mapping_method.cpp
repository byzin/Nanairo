/*!
  \file tone_mapping_method.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "tone_mapping_method.hpp"
// Standard C++ library
#include <functional>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/matrix.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "filmic.hpp"
#include "modified_reinhard.hpp"
#include "reinhard.hpp"
#include "uncharted2_filmic.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/yxy_color.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ToneMappingMethod::ToneMappingMethod(const System& system,
                                     const QJsonObject& settings) noexcept
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
ToneMappingMethod::~ToneMappingMethod() noexcept
{
}

/*!
  \details
  No detailed.
  */
YxyColor ToneMappingMethod::getWhitePoint(
    const std::vector<YxyColor>& pixel_luminance) const noexcept
{
  const YxyColor* white_point = &pixel_luminance[0];
  for (uint index = 1; index < pixel_luminance.size(); ++index) {
    const auto& pixel = pixel_luminance[index];
    if (white_point->Y() < pixel.Y())
      white_point = &pixel;
  }
  return *white_point;
}

/*!
  \details
  No detailed.
  */
void ToneMappingMethod::setPixelLuminance(System& system,
                                          const HdrImage& hdr_image,
                                          std::vector<YxyColor>& pixel_luminance) const noexcept
{
  using zisc::cast;

  auto set_pixel_luminance = [&hdr_image, &pixel_luminance](const uint y)
  {
    uint index = cast<uint>(y) * hdr_image.widthResolution();
    for (uint x = 0; x < hdr_image.widthResolution(); ++x) {
      pixel_luminance[index] = (hdr_image[index].y() == 0.0)
          ? YxyColor{}
          : hdr_image[index].toYxy();
      ++index;
    }
  };

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  const auto height = hdr_image.heightResolution();
  auto result = thread_pool.enqueueLoop(set_pixel_luminance, start, height);
  result.get();
}

/*!
  \details
  No detailed.
  */
void ToneMappingMethod::initialize(const System& system,
                                   const QJsonObject& /* settings */) noexcept
{
  inverse_gamma_ = 1.0 / system.gamma();
  const auto color_space = system.colorSpace();
  to_rgb_matrix_ = getXyzToRgbMatrix(color_space);
  to_xyz_matrix_ = getRgbToXyzMatrix(color_space);
}

/*!
  \details
  No detailed.
  */
UniquePointer<ToneMappingMethod> makeToneMappingMethod(const System& system,
                                                       const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  ToneMappingMethod* method = nullptr;

  const auto type = stringValue(settings, keyword::toneMapping);
  switch (keyword::toHash32(type)) {
    case toHash32(keyword::reinhard): {
      method = new Reinhard{system, settings};
      break;
    }
    case toHash32(keyword::modifiedReinhard): {
      method = new ModifiedReinhard{system, settings};
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
    default:
      zisc::raiseError("ToneMappingError: Unsupprted type is specified.");
  }

  return UniquePointer<ToneMappingMethod>{method};
}

} // namespace nanairo
