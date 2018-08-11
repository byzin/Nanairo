/*!
  \file tone_mapping_operator.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "tone_mapping_operator.hpp"
// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/matrix.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "filmic.hpp"
#include "reinhard.hpp"
#include "uncharted2_filmic.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_conversion.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/Color/rgba_32.hpp"
#include "NanairoCore/Color/yxy_color.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Setting/system_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
ToneMappingOperator::ToneMappingOperator(const System& system,
                                         const SettingNodeBase* settings) noexcept
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
                              LdrImage* ldr_image) const noexcept
{
  ZISC_ASSERT(ldr_image != nullptr, "The LDR image is null.");
  ZISC_ASSERT(hdr_image.widthResolution() == ldr_image->widthResolution(),
              "The image width is difference between HDR and LDR images.");
  ZISC_ASSERT(hdr_image.heightResolution() == ldr_image->heightResolution(),
              "The image height is difference between HDR and LDR images.");
  auto map_luminance = [this, &system, &hdr_image, ldr_image](const int thread_id)
  {
    // Set the calculation range
    const auto range = system.calcThreadRange(hdr_image.numOfPixels(), thread_id);
    // Apply tonemap to each pixel
    for (uint index = range[0]; index < range[1]; ++index) {
      auto rgba32 = Rgba32{};
      if (0.0 < hdr_image[index].y()) {
        auto xyz = hdr_image[index];
        // Tone mapping
        {
          auto yxy = ColorConversion::toYxy(xyz);
          const Float l = tonemap(exposure() * yxy.Y());
          yxy.Y() = zisc::clamp(l, 0.0, 1.0);
          xyz = ColorConversion::toXyz(yxy);
        }
        // Convert XYZ to RGB
        {
          const auto to_rgb_matrix = getXyzToRgbMatrix(system.colorSpace());
          auto rgb = ColorConversion::toRgb(xyz, to_rgb_matrix);
          rgb.clampAll(0.0, 1.0);
          rgb.correctGamma(inverseGamma());
          rgba32 = ColorConversion::toIntRgb(rgb);
        }
      }
      ldr_image->get(index) = rgba32;
    }
  };

  {
    auto& threads = system.threadManager();
    auto& work_resource = system.globalMemoryManager();
    constexpr uint begin = 0;
    const uint end = threads.numOfThreads();
    auto result = threads.enqueueLoop(map_luminance, begin, end, &work_resource);
    result.wait();
  }
}

/*!
  \details
  No detailed.
  */
void ToneMappingOperator::initialize(const System& system,
                                     const SettingNodeBase* settings) noexcept
{
  const auto system_settings = castNode<SystemSettingNode>(settings);

  // Gamma
  {
    inverse_gamma_ = zisc::invert(system.gamma());
  }
  // Exposure
  {
    exposure_ = zisc::cast<Float>(system_settings->exposure());
  }
}

/*!
  \details
  No detailed.
  */
zisc::UniqueMemoryPointer<ToneMappingOperator> ToneMappingOperator::makeOperator(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto system_settings = castNode<SystemSettingNode>(settings);

  zisc::UniqueMemoryPointer<ToneMappingOperator> method;
  auto data_resource = &system.dataMemoryManager();
  switch (system_settings->toneMappingType()) {
   case ToneMappingType::kReinhard: {
    method = zisc::UniqueMemoryPointer<Reinhard>::make(data_resource,
                                                       system,
                                                       settings);
    break;
   }
   case ToneMappingType::kFilmic: {
    method = zisc::UniqueMemoryPointer<Filmic>::make(data_resource,
                                                     system,
                                                     settings);
    break;
   }
   case ToneMappingType::kUncharted2Filmic: {
    method = zisc::UniqueMemoryPointer<Uncharted2Filmic>::make(data_resource,
                                                               system,
                                                               settings);
    break;
   }
   default: {
    zisc::raiseError("ToneMappingError: Unsupprted type is specified.");
   }
  }
  return method;
}

} // namespace nanairo
