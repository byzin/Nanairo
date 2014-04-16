/*!
  \file hdr_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "hdr_image.hpp"
// Standard C++ library
#include <vector>
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
HdrImage::HdrImage(const uint width, const uint height) :
    width_{width},
    height_{height}
{
  initialize();
}

/*!
  \details
  No detailed.
  */
uint HdrImage::bufferMemorySize() const
{
  return width_ * height_ * sizeof(XyzColor);
}

/*!
  \details
  No detailed.
  */
void HdrImage::initialize()
{
  const uint buffer_size = width_ * height_;
  buffer_.resize(buffer_size);
}

} // namespace nanairo
