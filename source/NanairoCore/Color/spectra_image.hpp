/*!
  \file spectra_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SPECTRA_IMAGE_HPP_
#define _NANAIRO_SPECTRA_IMAGE_HPP_

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
template <uint> class SampledSpectra;
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
  SpectraImage(const uint width, const uint height);


  //! Add radiance from a sample
  template <uint kSampleSize>
  void addSpectraContribution(
      const uint x,
      const uint y,
      const SampledSpectra<kSampleSize>& contribution);

  //! Return the buffer memory size
  std::size_t bufferMemorySize() const override;

  //! Clear the spectra image buffer to 0
  void clear() override;

  //! Save the spectra buffer
  void save(const quint64 cycle, const QString& file_path) const override;

  //! Convert the spectra image to the HDR image
  void toHdrImage(System& system,
                  const quint64 cycle, 
                  HdrImage* hdr_image) const override;

  //! Return the spectra image type
  SpectraImageType type() const override;

 private:
  //! Initialize
  void initialize();


  std::vector<SpectralDistribution> buffer_;
  std::vector<SpectralDistribution> compensation_;
};

//! \} Core

} // namespace nanairo

#include "spectra_image-inl.hpp"

#endif // _NANAIRO_SPECTRA_IMAGE_HPP_
