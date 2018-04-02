/*!
  \file spectra_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectra_image.hpp"
// Standard C++ library
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "hdr_image.hpp"
#include "spectral_distribution.hpp"
#include "xyz_color.hpp"
#include "xyz_color_matching_function.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SpectraImage::SpectraImage(const uint width, const uint height) noexcept :
    SpectraImageInterface(width, height)
{
  initialize();
}

/*!
  \details
  No detailed.
  */
SpectraImage::SpectraImage(const Index2d& resolution) noexcept :
    SpectraImageInterface(resolution)
{
  initialize();
}

/*!
  \details
  No detailed.
  */
void SpectraImage::addContribution(
    const uint pixel_index,
    const SampledSpectra& contribution) noexcept
{
  ZISC_ASSERT(!contribution.hasNan(), "The contribution has nan.");
  ZISC_ASSERT(!contribution.hasInf(), "The contribution has nan.");

  auto& pixel = buffer_[pixel_index];
  auto& compensation = compensation_[pixel_index];
  for (uint i = 0; i < SampledSpectra::size(); ++i) {
    const uint index = getIndex(contribution.wavelength(i));

    const Float value = contribution.intensity(i);
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
void SpectraImage::clear() noexcept
{
  for (auto& pixel : buffer_)
    pixel.fill(0.0);
  for (auto& compensation : compensation_)
    compensation.fill(0.0);
}

/*!
  \details
  No detailed.
  */
void SpectraImage::toHdrImage(System& system,
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
    const auto& cmf = system.xyzColorMatchingFunction();
    for (uint index = range[0]; index < range[1]; ++index)
      (*hdr_image)[index] = cmf.toXyzForEmitter(buffer_[index]) * averager;
  };

  {
    auto& threads = system.threadManager();
    constexpr uint start = 0;
    const uint end = threads.numOfThreads();
    auto result = threads.enqueueLoop(to_hdr_image, start, end);
    result.get();
  }
}

/*!
  \details
  No detailed.
  */
RenderingColorMode SpectraImage::type() const noexcept
{
  return RenderingColorMode::kSpectra;
}

/*!
  \details
  No detailed.
  */
void SpectraImage::initialize() noexcept
{
  const auto& r = resolution();
  const uint buffer_size = r[0] * r[1];
  buffer_.resize(buffer_size);
  compensation_.resize(buffer_size);
}

} // namespace nanairo
