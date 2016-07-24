/*!
  \file rgb_spectra_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RGB_SPECTRA_IMAGE_HPP
#define NANAIRO_RGB_SPECTRA_IMAGE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Qt
#include <QtGlobal>
// Nanairo
#include "rgb_color.hpp"
#include "spectra_image_interface.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

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
class RgbSpectraImage : public SpectraImageInterface
{
 public:
  //! Initialize the RGB image
  RgbSpectraImage(const uint width, const uint height) noexcept;


  //! Add radiance from a sample
  template <uint kSampleSize>
  void addRgbContribution(const uint x,
                          const uint y,
                          const SampledSpectra<kSampleSize>& contribution) noexcept;

  //! Return the buffer memory size
  std::size_t bufferMemorySize() const noexcept override;

  //! Clear RGB image buffer
  void clear() noexcept override;

  //! Save the RGB buffer
  void save(const quint64 cycle, const QString& file_path) const noexcept override;

  //! Convert RGB to XYZ
  void toHdrImage(System& system,
                  const quint64 cycle, 
                  HdrImage* hdr_image) const noexcept override;

  //! Return the rgb spectra image type
  SpectraImageType type() const noexcept override;

 private:
  //! Initialize
  void initialize() noexcept;


  std::vector<RgbColor> buffer_;
  std::vector<RgbColor> compensation_;
};

//! \} Core

} // namespace nanairo

#include "rgb_spectra_image-inl.hpp"

#endif // NANAIRO_RGB_SPECTRA_IMAGE_HPP
