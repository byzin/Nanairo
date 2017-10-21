/*!
  \file spectra_image_interface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectra_image_interface.hpp"
// Zisc
#include "zisc/point.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SpectraImageInterface::SpectraImageInterface(const uint width,
                                             const uint height) noexcept :
    resolution_{Index2d{width, height}}
{
}

/*!
  \details
  No detailed.
  */
SpectraImageInterface::SpectraImageInterface(const Index2d& resolution) noexcept :
    resolution_{resolution}
{
}

SpectraImageInterface::~SpectraImageInterface() noexcept
{
}

} // namespace nanairo
