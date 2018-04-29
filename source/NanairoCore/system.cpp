/*!
  \file system.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "system.hpp"
// Standard C++ library
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/memory_manager.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
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
System::System(const SettingNodeBase* system_settings) noexcept :
    memory_manager_list_{zisc::cast<std::size_t>(castNode<SystemSettingNode>(system_settings)->numOfThreads() + 2)},
    sampler_list_{&dataMemoryManager()}
{
  initialize(system_settings);
}

/*!
  \details
  No detailed.
  */
System::~System() noexcept
{
  // Destroy before the memory managers are destroyed
  sampler_list_.clear();
  thread_manager_.reset();
  xyz_color_matching_function_.reset();
}

/*!
  \details
  No detailed.
  */
void System::initialize(const SettingNodeBase* settings) noexcept
{
  const auto system_settings = castNode<SystemSettingNode>(settings);

  auto& data_resource = dataMemoryManager();
  // Thread pool
  {
    const auto num_of_threads = system_settings->numOfThreads();
    thread_manager_ = zisc::UniqueMemoryPointer<zisc::ThreadManager>::make(
        &data_resource,
        num_of_threads,
        &data_resource);
  }
  // Sampler
  {
    const auto num_of_sampler = system_settings->numOfThreads() + 1;
    sampler_list_.reserve(num_of_sampler);
    for (uint i = 0; i < num_of_sampler; ++i) {
      const auto seed = zisc::cast<uint64>(system_settings->randomSeed()) + i;
      sampler_list_.emplace_back(seed);
    }
  }
  // Image resolution
  {
    image_resolution_[0] = system_settings->imageWidthResolution();
    image_resolution_[1] = system_settings->imageHeightResolution();
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
    xyz_color_matching_function_ =
        zisc::UniqueMemoryPointer<XyzColorMatchingFunction>::make(&data_resource);
  }

  // Check type properties
  static_assert(sizeof(std::unique_ptr<int*>) == sizeof(int*),
                "The size of a unique pointer doesn't match the size of a pointer.");
}

} // namespace nanairo
