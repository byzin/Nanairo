/*!
  \file rgba_32-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RGBA_32_INL_HPP
#define NANAIRO_RGBA_32_INL_HPP

#include "rgba_32.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
Rgba32::Rgba32() noexcept :
    row_data_{kAlphaMask}
{
}

/*!
  */
inline
Rgba32::Rgba32(const uint8 r, const uint8 g, const uint8 b) noexcept :
    row_data_{kAlphaMask}
{
  setRgb(r, g, b);
}

/*!
  */
inline
Rgba32::Rgba32(const uint8 r, const uint8 g, const uint8 b, const uint8 a) noexcept :
    row_data_{0}
{
  setRgba(r, g, b, a);
}

/*!
  */
inline
Rgba32::Rgba32(const Float r, const Float g, const Float b) noexcept :
    row_data_{kAlphaMask}
{
  setRgbFloat(r, g, b);
}

/*!
  */
inline
Rgba32::Rgba32(const Float r, const Float g, const Float b, const Float a) noexcept :
    row_data_{0}
{
  setRgbaFloat(r, g, b, a);
}

/*!
  */
inline
uint8 Rgba32::alpha() const noexcept
{
  const uint8 a = component<kAlphaMask, kAlphaOffset>();
  return a;
}

/*!
  */
inline
Float Rgba32::alphaFloat() const noexcept
{
  const Float a = toFloatColor(alpha());
  return a;
}

/*!
  */
inline
uint8 Rgba32::blue() const noexcept
{
  const uint8 b = component<kBlueMask, kBlueOffset>();
  return b;
}

/*!
  */
inline
Float Rgba32::blueFloat() const noexcept
{
  const Float b = toFloatColor(blue());
  return b;
}

/*!
  */
inline
const uint32& Rgba32::rowData() const noexcept
{
  return row_data_;
}

/*!
  */
inline
uint8 Rgba32::green() const noexcept
{
  const uint8 g = component<kGreenMask, kGreenOffset>();
  return g;
}

/*!
  */
inline
Float Rgba32::greenFloat() const noexcept
{
  const Float g = toFloatColor(green());
  return g;
}

/*!
  */
inline
uint8 Rgba32::red() const noexcept
{
  const uint8 r = component<kRedMask, kRedOffset>();
  return r;
}

/*!
  */
inline
Float Rgba32::redFloat() const noexcept
{
  const Float r = toFloatColor(red());
  return r;
}

/*!
  */
inline
void Rgba32::setAlpha(const uint8 a) noexcept
{
  setComponent<kAlphaMask, kAlphaOffset>(a);
}

/*!
  */
inline
void Rgba32::setAlphaFloat(const Float a) noexcept
{
  const uint32 a32 = toIntColor(a);
  setComponent<kAlphaMask, kAlphaOffset>(a32);
}

/*!
  */
inline
void Rgba32::setBlue(const uint8 b) noexcept
{
  setComponent<kBlueMask, kBlueOffset>(b);
}

/*!
  */
inline
void Rgba32::setBlueFloat(const Float b) noexcept
{
  const uint32 b32 = toIntColor(b);
  setComponent<kBlueMask, kBlueOffset>(b32);
}

/*!
  */
inline
void Rgba32::setRowData(const uint32 row_data) noexcept
{
  row_data_ = row_data;
}

/*!
  */
inline
void Rgba32::setGreen(const uint8 g) noexcept
{
  setComponent<kGreenMask, kGreenOffset>(g);
}

/*!
  */
inline
void Rgba32::setGreenFloat(const Float g) noexcept
{
  const uint32 g32 = toIntColor(g);
  setComponent<kGreenMask, kGreenOffset>(g32);
}

/*!
  */
inline
void Rgba32::setRed(const uint8 r) noexcept
{
  setComponent<kRedMask, kRedOffset>(r);
}

/*!
  */
inline
void Rgba32::setRedFloat(const Float r) noexcept
{
  const uint32 r32 = toIntColor(r);
  setComponent<kRedMask, kRedOffset>(r32);
}

/*!
  */
inline
void Rgba32::setRgb(const uint8 r, const uint8 g, const uint8 b) noexcept
{
  setRed(r);
  setGreen(g);
  setBlue(b);
}

/*!
  */
inline
void Rgba32::setRgba(const uint8 r, const uint8 g, const uint8 b, const uint8 a)
    noexcept
{
  setAlpha(a);
  setRed(r);
  setGreen(g);
  setBlue(b);
}

/*!
  */
inline
void Rgba32::setRgbFloat(const Float r,
                         const Float g,
                         const Float b) noexcept
{
  setRedFloat(r);
  setGreenFloat(g);
  setBlueFloat(b);
}

/*!
  */
inline
void Rgba32::setRgbaFloat(const Float r,
                          const Float g,
                          const Float b,
                          const Float a) noexcept
{
  setAlphaFloat(a);
  setRedFloat(r);
  setGreenFloat(g);
  setBlueFloat(b);
}

/*!
  */
template <uint32 kMask, uint32 kOffset> inline
uint8 Rgba32::component() const noexcept
{
  const uint8 c = zisc::cast<uint8>((rowData() & kMask) >> kOffset);
  return c;
}

/*!
  */
template <uint32 kMask, uint32 kOffset> inline
void Rgba32::setComponent(const uint8 c) noexcept
{
  setComponent<kMask, kOffset>(zisc::cast<uint32>(c));
}

/*!
  */
template <uint32 kMask, uint32 kOffset> inline
void Rgba32::setComponent(const uint32 c) noexcept
{
  const uint32 c32 = c << kOffset;
  const uint32 row_data = (rowData() & (~kMask)) | c32;
  setRowData(row_data);
}

/*!
  */
inline
uint32 Rgba32::toIntColor(const Float c) const noexcept
{
  constexpr Float k = zisc::cast<Float>(255.0);
  const uint32 v = zisc::cast<uint32>(k * c);
  ZISC_ASSERT(zisc::isInClosedBounds(v, 0u, 255u),
              "The color is out of range [0, 255]");
  return v;
}

/*!
  */
inline
Float Rgba32::toFloatColor(const uint8 c) const noexcept
{
  constexpr Float k = zisc::cast<Float>(1.0 / 255.0);
  const Float v = k * zisc::cast<Float>(c);
  ZISC_ASSERT(zisc::isInClosedBounds(v, 0.0, 1.0),
              "The color is ouf of range [0, 1]");
  return v;
}

/*!
  */
inline
bool operator==(const Rgba32 lhs, const Rgba32 rhs) noexcept
{
  return lhs.rowData() == rhs.rowData();
}

/*!
  */
inline
bool operator!=(const Rgba32 lhs, const Rgba32 rhs) noexcept
{
  return lhs.rowData() != rhs.rowData();
}

/*!
  */
inline
bool operator<(const Rgba32 lhs, const Rgba32 rhs) noexcept
{
  return lhs.rowData() < rhs.rowData();
}

/*!
  */
inline
bool operator<=(const Rgba32 lhs, const Rgba32 rhs) noexcept
{
  return lhs.rowData() <= rhs.rowData();
}

/*!
  */
inline
bool operator>(const Rgba32 lhs, const Rgba32 rhs) noexcept
{
  return rhs < lhs;
}

/*!
  */
inline
bool operator>=(const Rgba32 lhs, const Rgba32 rhs) noexcept
{
  return rhs <= lhs;
}

} // namespace nanairo

#endif // NANAIRO_RGBA_32_INL_HPP
