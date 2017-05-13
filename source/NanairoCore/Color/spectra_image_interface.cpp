/*!
  \file spectra_image_interface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectra_image_interface.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SpectraImageInterface::SpectraImageInterface(const uint width,
                                             const uint height) noexcept :
    width_{width},
    height_{height}
{
}

SpectraImageInterface::~SpectraImageInterface() noexcept
{
}

} // namespace nanairo
