/*!
  \file rgb_spectra_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RGB_SPECTRA_IMAGE_HPP
#define NANAIRO_RGB_SPECTRA_IMAGE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "rgb_color.hpp"
#include "spectra_image_interface.hpp"
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
class RgbSpectraImage : public SpectraImageInterface
{
 public:
  //! Initialize the RGB image
  RgbSpectraImage(const uint width, const uint height) noexcept;

  //! Initialize the RGB image
  RgbSpectraImage(const Index2d& resolution) noexcept;


  //! Add radiance from a sample
  void addContribution(const uint pixel_index,
                       const SampledSpectra& contribution) noexcept override;

  //! Clear RGB image buffer
  void clear() noexcept override;

  //! Convert RGB to XYZ
  void toHdrImage(System& system,
                  const uint64 cycle,
                  HdrImage* hdr_image) const noexcept override;

  //! Return the rgb spectra image type
  RenderingColorMode type() const noexcept override;

 private:
  //! Initialize
  void initialize() noexcept;


  std::vector<RgbColor> buffer_;
  std::vector<RgbColor> compensation_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_RGB_SPECTRA_IMAGE_HPP
