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
#include "Denoiser/denoiser.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "RenderingMethod/rendering_method.hpp"
#include "Sampling/sample_statistics.hpp"
#include "Sampling/Sampler/sampler.hpp"
#include "Setting/rendering_method_setting_node.hpp"
#include "Setting/setting_node_base.hpp"
#include "Setting/system_setting_node.hpp"
#include "ToneMappingOperator/tone_mapping_operator.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
System::System(const SettingNodeBase* settings) noexcept :
    memory_manager_list_{zisc::cast<std::size_t>(castNode<SystemSettingNode>(settings)->numOfThreads() + 2)},
    sampler_list_{&dataMemoryManager()}
{
  initialize(settings);
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
  tone_mapping_operator_.reset();
  xyz_color_matching_function_.reset();
  denoiser_.reset();
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
  // Image resolution
  {
    image_resolution_[0] = system_settings->imageWidthResolution();
    image_resolution_[1] = system_settings->imageHeightResolution();
  }
  // Sampler
  {
    sampler_type_ = system_settings->samplerType();
    sampler_seed_ = system_settings->samplerSeed();
    const uint32 num_of_pixels = imageWidthResolution() * imageHeightResolution();
    sampler_list_.reserve(num_of_pixels + 1);
    for (uint32 index = 0; index <= num_of_pixels; ++index) {
      const uint32 seed = samplerSeed() + zisc::Fnv1aHash32::hash(index);
      sampler_list_.emplace_back(Sampler::make(samplerType(), seed, &data_resource));
    }
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
  // Tone mapping
  tone_mapping_operator_ = ToneMappingOperator::makeOperator(*this, system_settings);

  // Sample statistics
  {
    const auto pos = zisc::cast<std::size_t>(SampleStatistics::Type::kExpectedValue);
    statistics_flag_.set(pos, true);
  }
  // Denoiser
  {
    if (system_settings->isDenoisingEnabled())
      denoiser_ = Denoiser::makeDenoiser(*this, system_settings);
  }

  // Check type properties
  static_assert(sizeof(std::unique_ptr<int*>) == sizeof(int*),
                "The size of a unique pointer doesn't match the size of a pointer.");
}

} // namespace nanairo
