/*!
  \file spectra_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRA_IMAGE_HPP
#define NANAIRO_SPECTRA_IMAGE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "spectral_distribution.hpp"
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
class SpectraImage : public SpectraImageInterface
{
 public:
  //! Create a spectra image
  SpectraImage(const uint width, const uint height) noexcept;


  //! Add radiance from a sample
  void addContribution(const uint x,
                       const uint y,
                       const SampledSpectra& contribution) noexcept override;

  //! Clear the spectra image buffer to 0
  void clear() noexcept override;

  //! Convert the spectra image to the HDR image
  void toHdrImage(System& system,
                  const uint64 cycle,
                  HdrImage* hdr_image) const noexcept override;

  //! Return the spectra image type
  RenderingColorMode type() const noexcept override;

 private:
  //! Initialize
  void initialize() noexcept;


  std::vector<SpectralDistribution> buffer_;
  std::vector<SpectralDistribution> compensation_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SPECTRA_IMAGE_HPP
