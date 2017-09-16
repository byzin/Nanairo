/*!
  \file spectra_image_interface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRA_IMAGE_INTERFACE_HPP
#define NANAIRO_SPECTRA_IMAGE_INTERFACE_HPP

// Standard C++ library
#include <cstddef>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"

namespace nanairo {

// Forward declaration
class HdrImage;
class SampledSpectra;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SpectraImageInterface
{
 public:
  //! Initialize the spectra image
  SpectraImageInterface(const uint width, const uint height) noexcept;

  //! Finalize the spectra image
  virtual ~SpectraImageInterface() noexcept;


  //! Add radiance from a sample
  virtual void addContribution(const uint x,
                               const uint y,
                               const SampledSpectra& contribution) noexcept = 0;

  //! Clear the buffer value to 0
  virtual void clear() noexcept = 0;

  //! Return the height resolution
  uint heightResolution() const noexcept;

  //! Convert the spectra image to the HDR image
  virtual void toHdrImage(System& system,
                          const uint64 cycle, 
                          HdrImage* hdr_image) const noexcept = 0;

  //! Return the spectra image type
  virtual RenderingColorMode type() const noexcept = 0;

  //! Return the width resolution
  uint widthResolution() const noexcept;

 private:
  uint width_,
       height_;
};

//! \} Core

} // namespace nanairo

#include "spectra_image_interface-inl.hpp"

#endif // NANAIRO_SPECTRA_IMAGE_INTERFACE_HPP
