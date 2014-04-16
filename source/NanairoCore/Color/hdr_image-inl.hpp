/*!
  \file hdr_image-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_HDR_IMAGE_INL_HPP_
#define _NANAIRO_HDR_IMAGE_INL_HPP_

#include "hdr_image.hpp"
// Standard C++ library
#include <vector>
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
XyzColor& HdrImage::operator[](const uint index)
{
  return buffer_[index];
}

/*!
  \details
  No detailed.
  */
inline
const XyzColor& HdrImage::operator[](const uint index) const
{
  return buffer_[index];
}

/*!
  \details
  No detailed.
  */
inline
const XyzColor& HdrImage::get(const uint x, const uint y) const
{
  const uint index = x + y * width_;
  return buffer_[index];
}

/*!
  \details
  No detailed.
  */
inline
uint HdrImage::heightResolution() const
{
  return height_;
}

/*!
  \details
  No detailed.
  */
inline
void HdrImage::set(const uint x, const uint y, const XyzColor& color)
{
  const uint index = x + y * width_;
  buffer_[index] = color;
}

/*!
  \details
  No detailed.
  */
inline
uint HdrImage::widthResolution() const
{
  return width_;
}

} // namespace nanairo

#endif // _NANAIRO_HDR_IMAGE_INL_HPP_
