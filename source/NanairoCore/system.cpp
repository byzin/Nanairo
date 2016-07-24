/*!
  \file system.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "system.hpp"
// Standard C++ library
#include <vector>
// Qt
#include <QJsonObject>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "Color/rgb_color_matching_function.hpp"
#include "Color/xyz_color_matching_function.hpp"
#include "NanairoCommon/keyword.hpp"
#include "Sampling/sampler.hpp"
#include "Utility/unique_pointer.hpp"
#include "Utility/scene_value.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
System::System(const QJsonObject& settings) noexcept
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
System::~System() noexcept
{
}

/*!
  \details
  No detailed.
  */
void System::initialize(const QJsonObject& settings) noexcept
{
  using zisc::cast;

  // Thread pool
  auto initialize_system = [this](const QJsonObject& node)
  {
    // Threading
    const auto num_of_threads = intValue<uint>(node, keyword::numOfThreads);
    thread_pool_ = new zisc::ThreadPool{num_of_threads};
  
    // Random number generator
    const auto random_seed = intValue<uint32>(node, keyword::randomSeed);
    sampler_list_.reserve(num_of_threads + 1);
    for (uint i = 0; i < (num_of_threads + 1); ++i)
      sampler_list_.emplace_back(random_seed + cast<uint32>(i));
  
    // Memory pool
    memory_pool_list_.resize(num_of_threads + 1);
  
    // Image resolution
    const auto image_resolution = arrayValue(node, keyword::imageResolution);
    image_width_resolution_ = intValue<uint>(image_resolution[0]);
    image_height_resolution_ = intValue<uint>(image_resolution[1]);
  };
  const auto system_settings = objectValue(settings, keyword::system);
  initialize_system(system_settings);

  // Color
  auto initialize_color_system = [this](const QJsonObject& node)
  {
    // RGB rendering
    is_rgb_rendering_mode_ = (stringValue(node, keyword::colorMode) == keyword::rgb);
    // Color space
    color_space_ = keyword::toHash32(stringValue(node, keyword::colorSpace));
    gamma_ = floatValue<Float>(node, keyword::gamma);
    // Color matching function
    rgb_color_matching_function_ = new RgbColorMatchingFunction{};
    xyz_color_matching_function_ = new XyzColorMatchingFunction{node};
  };
  const auto color_settings = objectValue(settings, keyword::color);
  initialize_color_system(color_settings);
}

} // namespace nanairo
