/*!
  \file system.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "system.hpp"
// Standard C++ library
#include <vector>
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
#include "Utility/scene_settings.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
System::System(const SceneSettings& settings) noexcept
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
void System::initialize(const SceneSettings& settings) noexcept
{
  using zisc::cast;

  // System
  QString prefix{keyword::system};
  // Thread pool
  auto key = prefix + "/" + keyword::numOfThreads;
  const auto num_of_threads = cast<uint>(settings.intValue(key));
  thread_pool_ = new zisc::ThreadPool{num_of_threads};

  // Random generator
  key = prefix + "/" + keyword::randomSeed;
  const auto random_seed = cast<uint32>(settings.intValue(key));
  sampler_list_.reserve(thread_pool_->numOfThreads() + 1);
  for (uint i = 0; i < thread_pool_->numOfThreads() + 1; ++i)
    sampler_list_.emplace_back(random_seed + cast<uint32>(i));

  // Memory pool
  memory_pool_list_.resize(thread_pool_->numOfThreads() + 1);

  // Image resolution
  key = prefix + "/" + keyword::imageWidthResolution;
  image_width_resolution_ = cast<uint>(settings.intValue(key));
  key = prefix + "/" + keyword::imageHeightResolution;
  image_height_resolution_ = cast<uint>(settings.intValue(key));

  // Color
  prefix = keyword::color;
  // RGB rendering
  key = prefix + "/" + keyword::rgbRendering;
  is_rgb_rendering_mode_ = settings.booleanValue(key);
  // Color space
  key = prefix + "/" + keyword::colorSpace;
  color_space_ = keyword::toHash32(settings.stringValue(key));
  key = prefix + "/" + keyword::gamma;
  gamma_ = cast<Float>(settings.realValue(key));
  // Color matching function
  rgb_color_matching_function_ = new RgbColorMatchingFunction{};
  xyz_color_matching_function_ = new XyzColorMatchingFunction{settings};
}

} // namespace nanairo
