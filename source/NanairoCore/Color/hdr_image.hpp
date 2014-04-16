/*!
  \file hdr_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_HDR_IMAGE_HPP_
#define _NANAIRO_HDR_IMAGE_HPP_

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
  HdrImage(const uint width, const uint height);

  //! Returh the pixel color by index
  XyzColor& operator[](const uint index);

  //! Returh the pixel color by index
  const XyzColor& operator[](const uint index) const;


  //! Return the buffer memory size
  uint bufferMemorySize() const;

  //! Return the pixel color
  const XyzColor& get(const uint x, const uint y) const;

  //! Return the height resolution
  uint heightResolution() const;

  //! Set pixel color
  void set(const uint x, const uint y, const XyzColor& color);

  //! Return the height resolution
  uint widthResolution() const;

 private:
  //! Initialize
  void initialize();


  std::vector<XyzColor> buffer_;
  uint width_,
       height_;
};

//! \} Core

} // namespace nanairo

#include "hdr_image-inl.hpp"

#endif // _NANAIRO_HDR_IMAGE_HPP_
