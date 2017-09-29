/*!
  \file system.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "system.hpp"
// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/memory_pool.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "Color/xyz_color_matching_function.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/sampler.hpp"
#include "Setting/setting_node_base.hpp"
#include "Setting/system_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
System::System(const SettingNodeBase* system_settings) noexcept
{
  initialize(system_settings);
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
void System::initialize(const SettingNodeBase* settings) noexcept
{
  const auto system_settings = castNode<SystemSettingNode>(settings);

  // Thread pool
  {
    thread_pool_ = std::make_unique<zisc::ThreadPool>(system_settings->numOfThreads());
  }
  // Sampler
  {
    const auto num_of_sampler = system_settings->numOfThreads() + 1;
    sampler_list_.reserve(num_of_sampler);
    for (uint i = 0; i < num_of_sampler; ++i)
      sampler_list_.emplace_back(zisc::cast<uint64>(system_settings->randomSeed() + i));
  }
  // Memory pool
  {
    const auto num_of_memory_pool = system_settings->numOfThreads() + 1;
    memory_pool_list_.reserve(num_of_memory_pool);
    for (uint i = 0; i < num_of_memory_pool; ++i)
      memory_pool_list_.emplace_back(CoreConfig::memoryPoolSize());
  }
  // Image resolution
  {
    image_width_resolution_ = system_settings->imageWidthResolution();
    image_height_resolution_ = system_settings->imageHeightResolution();
  }
  // Rendering color mode
  {
    color_mode_ = system_settings->colorMode();
  }
  // Color space
  {
    color_space_ = system_settings->colorSpace();
  }
  // Gamma correction
  {
    gamma_ = system_settings->gammaCorrection();
  }
  // Color matching function
  {
    xyz_color_matching_function_ = std::make_unique<XyzColorMatchingFunction>();
  }
}

} // namespace nanairo
