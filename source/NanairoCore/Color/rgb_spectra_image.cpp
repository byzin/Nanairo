/*!
  \file rgb_spectra_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rgb_spectra_image.hpp"
// Standard C++ library
#include <cstddef>
#include <future>
#include <vector>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "color_conversion.hpp"
#include "color_space.hpp"
#include "hdr_image.hpp"
#include "rgb_color.hpp"
#include "spectra_image_interface.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RgbSpectraImage::RgbSpectraImage(const uint width,
                                 const uint height,
                                 zisc::pmr::memory_resource* data_resource) noexcept :
    SpectraImageInterface(width, height),
    buffer_{data_resource},
    compensation_{data_resource}
{
  initialize();
}

/*!
  \details
  No detailed.
  */
RgbSpectraImage::RgbSpectraImage(const Index2d& resolution,
                                 zisc::pmr::memory_resource* data_resource) noexcept :
    SpectraImageInterface(resolution),
    buffer_{data_resource},
    compensation_{data_resource}
{
  initialize();
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::addContribution(
    const uint pixel_index,
    const SampledSpectra& contribution) noexcept
{
  ZISC_ASSERT(!contribution.hasNan(), "The contribution has nan.");
  ZISC_ASSERT(!contribution.hasInf(), "The contribution has nan.");

  auto& pixel = buffer_[pixel_index];
  auto& compensation = compensation_[pixel_index];
  for (uint index = 0; index < SampledSpectra::size(); ++index) {
    const Float value = contribution.intensity(2 - index);
    const Float t = pixel[index] + value;
    compensation[index] += (zisc::abs(value) < zisc::abs(pixel[index]))
        ? (pixel[index] - t) + value
        : (value - t) + pixel[index];
    pixel[index] = t;
  }
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::clear() noexcept
{
  for (auto& pixel : buffer_)
    pixel = RgbColor{};
  for (auto& compensation : compensation_)
    compensation = RgbColor{};
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::toHdrImage(System& system,
                                 const uint64 cycle,
                                 HdrImage* hdr_image) const noexcept
{
  ZISC_ASSERT(hdr_image != nullptr, "The HDR image is null.");

  using zisc::cast;

  const Float averager = zisc::invert(cast<Float>(cycle));
  auto to_hdr_image = [this, &system, hdr_image, averager](const int thread_id)
  {
    // Set the calculation range
    const auto range = system.calcThreadRange(hdr_image->numOfPixels(), thread_id);
    // Write to HDR image buffer
    const auto to_xyz_matrix = getRgbToXyzMatrix(system.colorSpace());
    for (uint index = range[0]; index < range[1]; ++index) {
      const auto rgb = averager * buffer_[index];
      (*hdr_image)[index] = ColorConversion::toXyz(rgb, to_xyz_matrix);
    }
  };

  {
    auto& threads = system.threadManager();
    auto& work_resource = system.globalMemoryManager();
    constexpr uint start = 0;
    const uint end = threads.numOfThreads();
    auto result = threads.enqueueLoop(to_hdr_image, start, end, &work_resource);
    result.get();
  }
}

/*!
  \details
  No detailed.
  */
RenderingColorMode RgbSpectraImage::type() const noexcept
{
  return RenderingColorMode::kRgb;
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::initialize() noexcept
{
  const auto& r = resolution();
  const uint buffer_size = r[0] * r[1];
  buffer_.resize(buffer_size);
  compensation_.resize(buffer_size);
}

} // namespace nanairo
