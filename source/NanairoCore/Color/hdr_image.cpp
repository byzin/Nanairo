/*!
  \file hdr_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "hdr_image.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/point.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
HdrImage::HdrImage(const uint width, const uint height) noexcept :
    resolution_{Index2d{zisc::cast<uint32>(width), zisc::cast<uint32>(height)}}
{
  initialize();
}

/*!
  */
HdrImage::HdrImage(const Index2d& resolution) noexcept :
    resolution_{resolution}
{
  initialize();
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
