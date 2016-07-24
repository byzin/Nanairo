/*!
  \file system-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SYSTEM_INL_HPP
#define NANAIRO_SYSTEM_INL_HPP

#include "system.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/thread_pool.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/sampler.hpp"
#include "Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
MemoryPool& System::globalMemoryPool() noexcept
{
  return memory_pool_list_[0];
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
  \details
  No detailed.
  */
inline
uint System::imageHeightResolution() const noexcept
{
  return image_height_resolution_;
}

/*!
  \details
  No detailed.
  */
inline
uint System::imageWidthResolution() const noexcept
{
  return image_width_resolution_;
}

/*!
  \details
  No detailed.
  */
inline
zisc::ThreadPool& System::threadPool() noexcept
{
  return *thread_pool_;
}

/*!
  \details
  No detailed.
  */
inline
const zisc::ThreadPool& System::threadPool() const noexcept
{
  return *thread_pool_;
}

/*!
  \details
  No detailed.
  */
inline
MemoryPool& System::threadMemoryPool(const uint thread_number) noexcept
{
  return memory_pool_list_[thread_number + 1];
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
  \details
  No detailed.
  */
inline
uint32 System::colorSpace() const noexcept
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
bool System::isRgbRenderingMode() const noexcept
{
  return is_rgb_rendering_mode_;
}

/*!
  \details
  No detailed.
  */
inline
const RgbColorMatchingFunction& System::rgbColorMatchingFunction() const noexcept
{
  return *rgb_color_matching_function_;
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
