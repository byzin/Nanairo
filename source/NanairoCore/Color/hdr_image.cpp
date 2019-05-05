/*!
  \file hdr_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "hdr_image.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/point.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
HdrImage::HdrImage(const uint width,
                   const uint height,
                   zisc::pmr::memory_resource* data_resource) noexcept :
    buffer_{data_resource},
    resolution_{Index2d{zisc::cast<uint32>(width), zisc::cast<uint32>(height)}}
{
  initialize();
}

/*!
  */
HdrImage::HdrImage(const Index2d& resolution,
                   zisc::pmr::memory_resource* data_resource) noexcept :
    buffer_{data_resource},
    resolution_{resolution}
{
  initialize();
}

/*!
  */
void HdrImage::toHdr(System& system,
                     const uint64 num_of_samples,
                     const zisc::pmr::vector<SpectralDistribution::SpectralDistributionPointer>& sample_table) noexcept
{
  using zisc::cast;
  const Float inv_n = zisc::invert(cast<Float>(num_of_samples));
  auto to_hdr = [this, &system, inv_n, &sample_table](const uint task_id)
  {
    // Set the calculation range
    const auto range = system.calcTaskRange(numOfPixels(), task_id);
    // Convert to HDR
    for (uint index = range[0]; index < range[1]; ++index) {
      const auto& sample_p = sample_table[index];
      buffer_[index] = sample_p->toXyzForEmitter(system) * inv_n;
    }
  };

  {
    auto& threads = system.threadManager();
    auto& work_resource = system.globalMemoryManager();
    constexpr uint start = 0;
    const uint end = threads.numOfThreads();
    auto result = threads.enqueueLoop(to_hdr, start, end, &work_resource);
    result->wait();
  }
}

/*!
  \details
  No detailed.
  */
void HdrImage::initialize() noexcept
{
  const uint buffer_size = widthResolution() * heightResolution();
  buffer_.resize(buffer_size);
}

} // namespace nanairo
