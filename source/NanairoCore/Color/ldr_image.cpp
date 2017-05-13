/*!
  \file ldr_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
#include "zisc/utility.hpp"
// Nanairo
#include "rgba_32.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
LdrImage::LdrImage(const uint width, const uint height) noexcept
{
  setResolution(width, height);
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
  {
    ZISC_ASSERT(width != 0, "The width resolution is 0.");
    ZISC_ASSERT(height != 0, "The height resolution is 0.");
    resolution_[0] = width;
    resolution_[1] = height;
  }
  {
    auto& buffer = data();
    buffer.resize(width * height);
  }
  {
    const Rgba32 black;
    fill(black);
  }
}

} // namespace nanairo
