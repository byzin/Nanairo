/*!
  \file ldr_image-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LDR_IMAGE_INL_HPP
#define NANAIRO_LDR_IMAGE_INL_HPP

#include "ldr_image.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "rgba_32.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
std::vector<Rgba32>& LdrImage::data() noexcept
{
  return buffer_;
}

/*!
  */
inline
const std::vector<Rgba32>& LdrImage::data() const noexcept
{
  return buffer_;
}

/*!
  */
inline
Rgba32& LdrImage::operator[](const uint index) noexcept
{
  return get(index);
}

/*!
  */
inline
const Rgba32& LdrImage::operator[](const uint index) const noexcept
{
  return get(index);
}

/*!
  */
inline
Rgba32& LdrImage::get(const uint index) noexcept
{
  ZISC_ASSERT(index < size(), "The index is out of range.");
  auto& buffer = data();
  return buffer[index];
}

/*!
  */
inline
const Rgba32& LdrImage::get(const uint index) const noexcept
{
  ZISC_ASSERT(index < size(), "The index is out of range.");
  const auto& buffer = data();
  return buffer[index];
}

/*!
  */
inline
Rgba32& LdrImage::get(const uint x, const uint y) noexcept
{
  return get(toIndex(x, y));
}

/*!
  */
inline
const Rgba32& LdrImage::get(const uint x, const uint y) const noexcept
{
  return get(toIndex(x, y));
}

/*!
  */
inline
uint LdrImage::heightResolution() const noexcept
{
  const auto& r = resolution();
  return r[1];
}

/*!
  */
inline
const std::array<uint, 2>& LdrImage::resolution() const noexcept
{
  return resolution_;
}

/*!
  */
inline
uint LdrImage::size() const noexcept
{
  const auto& buffer = data();
  return zisc::cast<uint>(buffer.size());
}

/*!
  */
inline
uint LdrImage::widthResolution() const noexcept
{
  const auto& r = resolution();
  return r[0];
}

/*!
  */
inline
uint LdrImage::toIndex(const uint x, const uint y) const noexcept
{
  const uint index = x + y * widthResolution();
  ZISC_ASSERT(index < size(), "The index is out of range.");
  return index;
}

} // namespace nanairo

#endif // NANAIRO_LDR_IMAGE_INL_HPP
