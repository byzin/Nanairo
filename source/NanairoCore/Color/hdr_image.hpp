/*!
  \file hdr_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_HDR_IMAGE_HPP
#define NANAIRO_HDR_IMAGE_HPP

// Standard C++ library
#include <vector>
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class HdrImage
{
 public:
  //! Create a hdr image
  HdrImage(const uint width, const uint height) noexcept;

  //! Returh the pixel color by index
  XyzColor& operator[](const uint index) noexcept;

  //! Returh the pixel color by index
  const XyzColor& operator[](const uint index) const noexcept;


  //! Return the buffer memory size
  uint bufferMemorySize() const noexcept;

  //! Return the pixel color
  const XyzColor& get(const uint x, const uint y) const noexcept;

  //! Return the height resolution
  uint heightResolution() const noexcept;

  //! Return the num of pixels
  uint numOfPixels() const noexcept;

  //! Set pixel color
  void set(const uint x, const uint y, const XyzColor& color) noexcept;

  //! Return the height resolution
  uint widthResolution() const noexcept;

 private:
  //! Initialize
  void initialize() noexcept;


  std::vector<XyzColor> buffer_;
  uint width_,
       height_;
};

//! \} Core

} // namespace nanairo

#include "hdr_image-inl.hpp"

#endif // NANAIRO_HDR_IMAGE_HPP
