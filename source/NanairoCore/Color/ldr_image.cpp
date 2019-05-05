/*!
  \file ldr_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "ldr_image.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/point.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "rgba_32.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
LdrImage::LdrImage(const uint width,
                   const uint height,
                   zisc::pmr::memory_resource* data_resource) noexcept :
    buffer_{data_resource}
{
  setResolution(width, height);
}

/*!
  */
LdrImage::LdrImage(const Index2d& resolution,
                   zisc::pmr::memory_resource* data_resource) noexcept :
    buffer_{data_resource}
{
  setResolution(resolution);
}

/*!
  */
void LdrImage::fill(const Rgba32 color) noexcept
{
  for (auto& pixel : data())
    pixel = color;
}

/*!
  */
std::size_t LdrImage::memoryUsage() const noexcept
{
  const auto& buffer = data();
  const std::size_t memory_usage = sizeof(buffer[0]) * buffer.size();
  return memory_usage;
}

/*!
  */
void LdrImage::setResolution(const uint width, const uint height) noexcept
{
  setResolution(Index2d{zisc::cast<uint32>(width),
                        zisc::cast<uint32>(height)});
}

/*!
  */
void LdrImage::setResolution(const Index2d& resolution) noexcept
{
  {
    ZISC_ASSERT(resolution[0] != 0, "The width resolution is 0.");
    ZISC_ASSERT(resolution[1] != 0, "The height resolution is 0.");
    resolution_ = resolution;
  }
  {
    auto& buffer = data();
    buffer.resize(resolution[0] * resolution[1]);
  }
  {
    const Rgba32 black;
    fill(black);
  }
}

} // namespace nanairo
