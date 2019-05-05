/*!
  \file rgba_32.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RGBA_32_HPP
#define NANAIRO_RGBA_32_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
class Rgba32
{
 public:
  static constexpr uint32 kAlphaMask = 0xff000000;
  static constexpr uint32 kAlphaOffset = 24;
  static constexpr uint32 kRedMask = 0x00ff0000;
  static constexpr uint32 kRedOffset = 16;
  static constexpr uint32 kGreenMask = 0x0000ff00;
  static constexpr uint32 kGreenOffset = 8;
  static constexpr uint32 kBlueMask = 0x000000ff;
  static constexpr uint32 kBlueOffset = 0;


  //! Initialize RGBA as visible black (0, 0, 0, 255)
  Rgba32() noexcept;

  //! Initialize RGBA as visible color (red, green, blue, 255)
  Rgba32(const uint8 r, const uint8 g, const uint8 b) noexcept;

  //! Initialize RGBA as color (alpha, red, green, blue)
  Rgba32(const uint8 r, const uint8 g, const uint8 b, const uint8 a) noexcept;

  //! Initialize RGBA as visible color
  Rgba32(const Float r, const Float g, const Float b) noexcept;

  //! Initialize RGBA as color
  Rgba32(const Float r, const Float g, const Float b, const Float a) noexcept;


  //! Return the alpha
  uint8 alpha() const noexcept;

  //! Return the alpha as [0, 1] float value
  Float alphaFloat() const noexcept;

  //! Return the blue
  uint8 blue() const noexcept;

  //! Return the blue as [0, 1] float value
  Float blueFloat() const noexcept;

  //! Return the RGBA row data
  const uint32& rowData() const noexcept;

  //! Return the green
  uint8 green() const noexcept;

  //! Return the green as [0, 1] float value
  Float greenFloat() const noexcept;

  //! Return the red
  uint8 red() const noexcept;

  //! Return the red as [0, 1] float value
  Float redFloat() const noexcept;

  //! Set the alpha
  void setAlpha(const uint8 a) noexcept;

  //! Set the alpha
  void setAlphaFloat(const Float a) noexcept;

  //! Set the blue
  void setBlue(const uint8 b) noexcept;

  //! Set the blue
  void setBlueFloat(const Float b) noexcept;

  //! Set the RGBA row data
  void setRowData(const uint32 row_data) noexcept;

  //! Set the green
  void setGreen(const uint8 g) noexcept;

  //! Set the green
  void setGreenFloat(const Float g) noexcept;

  //! Set the red
  void setRed(const uint8 r) noexcept;

  //! Set the red
  void setRedFloat(const Float r) noexcept;

  //! Set RGB without changing alpha
  void setRgb(const uint8 r, const uint8 g, const uint8 b) noexcept;

  //! Set RGBA
  void setRgba(const uint8 r, const uint8 g, const uint8 b, const uint8 a) noexcept;

  //! Set RGB without changing alpha
  void setRgbFloat(const Float r, const Float g, const Float b) noexcept;

  //! Set RGBA
  void setRgbaFloat(const Float r,
                    const Float g,
                    const Float b,
                    const Float a) noexcept;

 private:
  //! Set a component of RGBA
  template <uint32 kMask, uint32 kOffset>
  uint8 component() const noexcept;

  //! Set a component of RGBA
  template <uint32 kMask, uint32 kOffset>
  void setComponent(const uint8 c) noexcept;

  //! Set a component of RGBA
  template <uint32 kMask, uint32 kOffset>
  void setComponent(const uint32 c) noexcept;

  //! Convert float color to integer color
  uint32 toIntColor(const Float c) const noexcept;

  //! Convert integer color to float color
  Float toFloatColor(const uint8 c) const noexcept;


  uint32 row_data_;
};

//! Compare two RGBA32 objects
bool operator==(const Rgba32 lhs, const Rgba32 rhs) noexcept;

//! Compare two RGBA32 objects
bool operator!=(const Rgba32 lhs, const Rgba32 rhs) noexcept;

//! Compare two RGBA32 objects
bool operator<(const Rgba32 lhs, const Rgba32 rhs) noexcept;

//! Compare two RGBA32 objects
bool operator<=(const Rgba32 lhs, const Rgba32 rhs) noexcept;

//! Compare two RGBA32 objects
bool operator>(const Rgba32 lhs, const Rgba32 rhs) noexcept;

//! Compare two RGBA32 objects
bool operator>=(const Rgba32 lhs, const Rgba32 rhs) noexcept;

static_assert(sizeof(Rgba32) == 4, "The Rgba32 class isn't 32bit.");

} // namespace nanairo

#include "rgba_32-inl.hpp"

#endif // NANAIRO_RGBA_32_HPP
