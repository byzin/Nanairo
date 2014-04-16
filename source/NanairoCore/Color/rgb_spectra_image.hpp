/*!
  \file rgb_spectra_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RGB_SPECTRA_IMAGE_HPP_
#define _NANAIRO_RGB_SPECTRA_IMAGE_HPP_

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
  RgbSpectraImage(const uint width, const uint height);


  //! Add radiance from a sample
  template <uint kSampleSize>
  void addRgbContribution(const uint x,
                          const uint y,
                          const SampledSpectra<kSampleSize>& contribution);

  //! Return the buffer memory size
  std::size_t bufferMemorySize() const override;

  //! Clear RGB image buffer
  void clear() override;

  //! Save the RGB buffer
  void save(const quint64 cycle, const QString& file_path) const override;

  //! Convert RGB to XYZ
  void toHdrImage(System& system,
                  const quint64 cycle, 
                  HdrImage* hdr_image) const override;

  //! Return the rgb spectra image type
  SpectraImageType type() const override;

 private:
  //! Initialize
  void initialize();


  std::vector<RgbColor> buffer_;
  std::vector<RgbColor> compensation_;
};

//! \} Core

} // namespace nanairo

#include "rgb_spectra_image-inl.hpp"

#endif // _NANAIRO_RGB_SPECTRA_IMAGE_HPP_
