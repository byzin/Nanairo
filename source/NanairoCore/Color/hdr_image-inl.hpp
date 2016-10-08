/*!
  \file hdr_image-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_HDR_IMAGE_INL_HPP
#define NANAIRO_HDR_IMAGE_INL_HPP

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
XyzColor& HdrImage::operator[](const uint index) noexcept
{
  return buffer_[index];
}

/*!
  \details
  No detailed.
  */
inline
const XyzColor& HdrImage::operator[](const uint index) const noexcept
{
  return buffer_[index];
}

/*!
  \details
  No detailed.
  */
inline
const XyzColor& HdrImage::get(const uint x, const uint y) const noexcept
{
  const uint index = x + y * width_;
  return buffer_[index];
}

/*!
  \details
  No detailed.
  */
inline
uint HdrImage::heightResolution() const noexcept
{
  return height_;
}

/*!
  */
inline
uint HdrImage::numOfPixels() const noexcept
{
  return widthResolution() * heightResolution();
}

/*!
  \details
  No detailed.
  */
inline
void HdrImage::set(const uint x, const uint y, const XyzColor& color) noexcept
{
  const uint index = x + y * width_;
  buffer_[index] = color;
}

/*!
  \details
  No detailed.
  */
inline
uint HdrImage::widthResolution() const noexcept
{
  return width_;
}

} // namespace nanairo

#endif // NANAIRO_HDR_IMAGE_INL_HPP
