/*!
  \file spectra_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRA_IMAGE_HPP
#define NANAIRO_SPECTRA_IMAGE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Qt
#include <QtGlobal>
// Zisc
#include "zisc/arithmetic_array.hpp"
// Nanairo
#include "spectral_distribution.hpp"
#include "spectra_image_interface.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QString;

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

  //! Save the spectra buffer
  void save(const quint64 cycle, const QString& file_path) const noexcept override;

  //! Convert the spectra image to the HDR image
  void toHdrImage(System& system,
                  const quint64 cycle,
                  HdrImage* hdr_image) const noexcept override;

  //! Return the spectra image type
  SpectraImageType type() const noexcept override;

 private:
  //! Initialize
  void initialize() noexcept;


  std::vector<SpectralDistribution> buffer_;
  std::vector<SpectralDistribution> compensation_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SPECTRA_IMAGE_HPP
