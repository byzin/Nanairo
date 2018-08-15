/*!
  \file system-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SYSTEM_INL_HPP
#define NANAIRO_SYSTEM_INL_HPP

#include "system.hpp"
// Standard C++ library
#include <array>
#include <memory>
#include <type_traits>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_manager.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/stopwatch.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "Color/color_space.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/Sampler/sampler.hpp"

namespace nanairo {

//! Calculate the range of indices
template <typename Integer> inline
std::array<Integer, 2> System::calcTaskRange(const Integer range,
                                             const uint num_of_tasks,
                                             const uint task_id) noexcept
{
  return zisc::ThreadManager::calcThreadRange(range, num_of_tasks, task_id);
}

//! Calculate the range of indices
template <typename Integer> inline
std::array<Integer, 2> System::calcTaskRange(const Integer range,
                                             const uint task_id) const noexcept
{
  return calcTaskRange(range, threadManager().numOfThreads(), task_id);
}

/*!
  */
inline
auto System::dataMemoryManager() noexcept -> MemoryManager&
{
  return memory_manager_list_[0];
}

/*!
  \details
  No detailed.
  */
inline
auto System::globalMemoryManager() noexcept -> MemoryManager&
{
  return memory_manager_list_[1];
}

/*!
  */
inline
Sampler& System::globalSampler() noexcept
{
  auto& global_sampler = sampler_list_.back();
  return *global_sampler;
}

/*!
  */
inline
const Index2d& System::imageResolution() const noexcept
{
  return image_resolution_;
}

/*!
  \details
  No detailed.
  */
inline
uint System::imageHeightResolution() const noexcept
{
  const auto& image_resolution = imageResolution();
  return zisc::cast<uint>(image_resolution[1]);
}

/*!
  \details
  No detailed.
  */
inline
uint System::imageWidthResolution() const noexcept
{
  const auto& image_resolution = imageResolution();
  return zisc::cast<uint>(image_resolution[0]);
}

/*!
  */
inline
Sampler& System::localSampler(const uint index) noexcept
{
  auto& sampler = sampler_list_[index];
  return *sampler;
}

/*!
  \details
  No detailed.
  */
inline
zisc::ThreadManager& System::threadManager() noexcept
{
  return *thread_manager_;
}

/*!
  \details
  No detailed.
  */
inline
const zisc::ThreadManager& System::threadManager() const noexcept
{
  return *thread_manager_;
}

/*!
  \details
  No detailed.
  */
inline
auto System::threadMemoryManager(const uint thread_number) noexcept -> MemoryManager&
{
  return memory_manager_list_[thread_number + 2];
}

// Color

/*!
  */
inline
RenderingColorMode System::colorMode() const noexcept
{
  return color_mode_;
}

/*!
  \details
  No detailed.
  */
inline
ColorSpaceType System::colorSpace() const noexcept
{
  return color_space_;
}

/*!
  */
inline
const Denoiser& System::denoiser() const noexcept
{
  ZISC_ASSERT(hasDenoiser(), "The system has no denoiser.");
  return *denoiser_;
}

/*!
  \details
  No detailed.
  */
inline
Float System::gamma() const noexcept
{
  return gamma_;
}

/*!
  */
inline
bool System::hasDenoiser() const noexcept
{
  return (denoiser_.get() != nullptr);
}

/*!
  \details
  No detailed.
  */
inline
bool System::isRgbMode() const noexcept
{
  return colorMode() == RenderingColorMode::kRgb;
}

/*!
  */
inline
bool System::isSpectraMode() const noexcept
{
  return colorMode() == RenderingColorMode::kSpectra;
}

/*!
  */
inline
SamplerType System::samplerType() const noexcept
{
  return sampler_type_;
}

/*!
  */
inline
uint32 System::samplerSeed() const noexcept
{
  return sampler_seed_;
}

/*!
  */
inline
auto System::sampleStatisticsFlag() noexcept -> SampleStatisticsFlag&
{
  return statistics_flag_;
}

/*!
  */
inline
auto System::sampleStatisticsFlag() const noexcept -> const SampleStatisticsFlag&
{
  return statistics_flag_;
}

/*!
  */
inline
zisc::Stopwatch& System::stopwatch() noexcept
{
  return stopwatch_;
}

/*!
  */
inline
const zisc::Stopwatch& System::stopwatch() const noexcept
{
  return stopwatch_;
}

/*!
  */
inline
const ToneMappingOperator& System::toneMappingOperator() const noexcept
{
  return *tone_mapping_operator_;
}

/*!
  \details
  No detailed.
  */
inline
const XyzColorMatchingFunction& System::xyzColorMatchingFunction() const noexcept
{
  return *xyz_color_matching_function_;
}

} // namespace nanairo

#endif // NANAIRO_SYSTEM_INL_HPP
