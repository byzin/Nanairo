/*!
  \file spectra_image_interface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRA_IMAGE_INTERFACE_INL_HPP
#define NANAIRO_SPECTRA_IMAGE_INTERFACE_INL_HPP

#include "spectra_image_interface.hpp"
// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "rgb_spectra_image.hpp"
#include "spectra_image.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SpectraImageInterface::SpectraImageInterface(const uint width, const uint height) noexcept :
    width_{width},
    height_{height}
{
}

/*!
  \details
  No detailed.
  */
inline
uint SpectraImageInterface::heightResolution() const noexcept
{
  return height_;
}

/*!
  \details
  No detailed.
  */
inline
uint SpectraImageInterface::widthResolution() const noexcept
{
  return width_;
}

} // namespace nanairo

#endif // NANAIRO_SPECTRA_IMAGE_INTERFACE_INL_HPP
