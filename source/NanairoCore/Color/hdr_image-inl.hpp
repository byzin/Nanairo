/*!
  \file hdr_image-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_HDR_IMAGE_INL_HPP
#define NANAIRO_HDR_IMAGE_INL_HPP

#include "hdr_image.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/point.hpp"
#include "zisc/utility.hpp"
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
  return get(index);
}

/*!
  \details
  No detailed.
  */
inline
const XyzColor& HdrImage::operator[](const uint index) const noexcept
{
  return get(index);
}

/*!
  */
inline
std::vector<XyzColor>& HdrImage::data() noexcept
{
  return buffer_;
}

/*!
  */
inline
const std::vector<XyzColor>& HdrImage::data() const noexcept
{
  return buffer_;
}

/*!
  \details
  No detailed.
  */
inline
XyzColor& HdrImage::get(const uint index) noexcept
{
  return buffer_[index];
}

/*!
  \details
  No detailed.
  */
inline
const XyzColor& HdrImage::get(const uint index) const noexcept
{
  return buffer_[index];
}

/*!
  \details
  No detailed.
  */
inline
XyzColor& HdrImage::get(const uint x, const uint y) noexcept
{
  const uint index = toIndex(x, y);
  return get(index);
}

/*!
  \details
  No detailed.
  */
inline
const XyzColor& HdrImage::get(const uint x, const uint y) const noexcept
{
  const uint index = toIndex(x, y);
  return get(index);
}

/*!
  \details
  No detailed.
  */
inline
XyzColor& HdrImage::get(const Index2d& index) noexcept
{
  const uint i = toIndex(index[0], index[1]);
  return get(i);
}

/*!
  \details
  No detailed.
  */
inline
const XyzColor& HdrImage::get(const Index2d& index) const noexcept
{
  const uint i = toIndex(index[0], index[1]);
  return get(i);
}

/*!
  \details
  No detailed.
  */
inline
uint HdrImage::heightResolution() const noexcept
{
  const auto& r = resolution();
  return zisc::cast<uint>(r[1]);
}

/*!
  */
inline
std::size_t HdrImage::memorySize() const noexcept
{
  return zisc::cast<std::size_t>(sizeof(buffer_[0]) * buffer_.size());
}

/*!
  */
inline
uint HdrImage::numOfPixels() const noexcept
{
  return widthResolution() * heightResolution();
}

/*!
  */
inline
const Index2d& HdrImage::resolution() const noexcept
{
  return resolution_;
}

/*!
  */
inline
uint HdrImage::size() const noexcept
{
  return zisc::cast<uint>(buffer_.size());
}

/*!
  \details
  No detailed.
  */
inline
void HdrImage::set(const uint index, const XyzColor& color) noexcept
{
  buffer_[index] = color;
}

/*!
  \details
  No detailed.
  */
inline
void HdrImage::set(const uint x, const uint y, const XyzColor& color) noexcept
{
  const uint index = toIndex(x, y);
  set(index, color);
}

/*!
  \details
  No detailed.
  */
inline
void HdrImage::set(const Index2d& index, const XyzColor& color) noexcept
{
  const uint i = toIndex(index[0], index[1]);
  set(i, color);
}

/*!
  \details
  No detailed.
  */
inline
uint HdrImage::widthResolution() const noexcept
{
  const auto& r = resolution();
  return zisc::cast<uint>(r[0]);
}

/*!
  */
inline
uint HdrImage::toIndex(const uint x, const uint y) const noexcept
{
  return x + y * widthResolution();
}

} // namespace nanairo

#endif // NANAIRO_HDR_IMAGE_INL_HPP
