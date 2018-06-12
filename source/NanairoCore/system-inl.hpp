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
#include "zisc/thread_manager.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "Color/color_space.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/sampler.hpp"

namespace nanairo {

//! Calculate the range of indices
template <typename Integer> inline
std::array<Integer, 2> System::calcThreadRange(const Integer range,
                                               const uint num_of_threads,
                                               const int thread_id) noexcept
{
  static_assert(std::is_integral<Integer>::value, "The Integer isn't integer type.");
  // Set the calculation range 
  const Integer threads = zisc::cast<Integer>(num_of_threads);
  const Integer id = zisc::cast<Integer>(thread_id);
  ZISC_ASSERT(0 < range, "The range is minus.");
  ZISC_ASSERT(threads < range, "The num of threads is more than the range.");
  const Integer range_per_thread = range / threads;
  const Integer begin = id * range_per_thread;
  const Integer end = (begin + range_per_thread) + ((id + 1 == threads)
      ? (range % threads)
      : 0);
  return std::array<Integer, 2>{{begin, end}};
}

//! Calculate the range of indices
template <typename Integer> inline
std::array<Integer, 2> System::calcThreadRange(const Integer range,
                                               const int thread_id) const noexcept
{
  return calcThreadRange(range, threadManager().numOfThreads(), thread_id);
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
  \details
  No detailed.
  */
inline
Sampler& System::globalSampler() noexcept
{
  return sampler_list_[0];
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

/*!
  \details
  No detailed.
  */
inline
Sampler& System::threadSampler(const uint thread_number) noexcept
{
  return sampler_list_[thread_number + 1];
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
  \details
  No detailed.
  */
inline
Float System::gamma() const noexcept
{
  return gamma_;
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
auto System::sampleStatisticsFlag() const noexcept -> SampleStatisticsFlag
{
  return statistics_flag_;
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
